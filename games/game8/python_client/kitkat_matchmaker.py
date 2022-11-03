import socket
import random
import time
import sys
from signal import signal, SIGPIPE, SIG_DFL  
import numpy as np
import utils
import copy
signal(SIGPIPE,SIG_DFL)



class Matchmaker:
    def __init__(self, lam = 0.005):
        self.clientsocket = socket.socket()
        self.initial_candidates = []
        self.lam = lam
        self.used_mini_index = []
        random.seed(None)
        
        self.clientsocket.connect(('localhost', 20001))
        self.socketfile = self.clientsocket.makefile('rw')
        self.socketfile.write('Matchmaker\n')
        self.socketfile.flush()
        
        self.N = 0
            
        # Start Read from the 20 Random Candidates and scores from Server (First Round)
        for i in range(20):
            inputLine = self.socketfile.readline()
            print("Server says:" + inputLine)
            tmp = inputLine.split(":")
            # Check N and set
            if i == 0:
                # Set
                self.N = len(tmp) - 1
                if (self.N < 2 or self.N > 200):
                    print("invalid N")
                    sys.exit()
            candidates = tmp[:-1]
            score = tmp[-1]
            candidates = [float(x) for x in candidates]
            score = float(score)
            self.initial_candidates.append((candidates, score))
            '''
            TO DO: You could use the candidates and score to implement algorithm
            '''
        self.parse_initial_data(self.initial_candidates)

    def play(self):
        # Start Write Candidates to P and Read from the Scores (Next Rounds)
        score = 2
        candidate = []
        # for i in range(20, 40):
        for i in range(20):
            candidate = self.generate_candidates(candidate, i, score)
            self.socketfile.write(utils.floats_to_msg4(candidate))
            # self.socketfile.write(candidate)
            self.socketfile.flush() 
            inputline = self.socketfile.readline()
            print("Server says:" + inputline)
            if inputline == "END\n":
                sys.exit()
            tmp = inputline
            # print("inputline: " + str(inputline))
            # print(tmp[-1])
            # tmp = tmp[-1].strip('`')
            score = float(tmp.split(":")[-1].strip().strip('`').split(",")[0])
            # print(stri.split(":")[-1].strip().strip('`').split(",")[0])
            # score = float(tmp[-1])
            # score = float(inputline.split(":")[-1])
            time.sleep(.2)
        
        print("Server says:" + self.socketfile.readline())
        self.clientsocket.close()

    def parse_initial_data(self, data):
        self.candidates = copy.deepcopy(data)
        self.estimate = np.zeros(self.N)
        self.train()

    def train(self):

        # Add more training data in which score is 0, so we know which side is bad
        for x in np.arange(-1.00, 1.05, 0.05):
            candidate = np.ones(self.N) * x
            score = 0.0
            self.candidates.append((candidate, score))


        self.A = np.array([candidate[0] for candidate in self.candidates], dtype = float)

        # convert score also in 2d array, so we have same shape
        self.B = np.array([np.array(candidate[1]) for candidate in self.candidates], dtype = float)

        # Regularization matrix
        self.R = np.identity(self.A.shape[1]) * self.lam

        # Apply Tikhonov/Ridge Regression
        # Dot of transpose
        sub = np.dot(self.A.T, self.A) + np.dot(self.R.T, self.R)
        self.estimate = np.dot(np.linalg.inv(sub), np.dot(self.A.T, self.B)).reshape(self.N)

        # self.truncate_estimate()

    # def truncate_estimate(self):
    #     self.estimate = np.trunc(self.estimate * 100) / 100.0

    def guess(self, candidate, score, index):
        if index != 0:
            self.re_estimate(candidate, score, index)

        return self.find_candidate()

    def re_estimate(self, candidate, score,index):
        assert len(candidate) == self.N, "len(candidate): " + str(len(candidate)) + "; self.N: " + str(self.N) + "index: " + str(index)

        self.candidates.append((candidate, score))

        self.A = np.vstack((self.A, np.array([candidate], dtype = float)))

        self.B = np.append(self.B, np.array(score, dtype = float))

        self.R = np.identity(self.A.shape[1]) * self.lam
        # print(self.A.shape, self.R.shape, self.B.shape)

        sub = np.dot(self.A.T, self.A) + np.dot(self.R.T, self.R)
        one = np.linalg.inv(sub)
        two = np.dot(self.A.T, self.B)
        self.estimate = np.dot(one, two).reshape(self.N)

        # self.truncate_estimate()

    def find_candidate(self):
        while 1:
            cand = np.array(self.estimate > 0.0, dtype = float).reshape(self.N)

            cand = cand % 1.0000001

            if self.check_uniqueness(cand) == False:
                cand = self.add_noise()

                if self.check_uniqueness(cand) == True:
                    return cand
            else:
                return cand

    def check_uniqueness(self, cand = []):
        for c in self.candidates:
            if ((c[0] == cand).all()):
                return False
        return True

    def add_noise(self):
        cWeights = self.estimate.copy()

        minis = utils.get_abs_min_neg_pos(cWeights, self.used_mini_index)

        cWeights[minis[0]] = -1 * cWeights[minis[0]]
        cWeights[minis[1]] = -1 * cWeights[minis[1]]

        self.used_mini_index.append(minis[0])
        self.used_mini_index.append(minis[1])

        # noise = np.random.normal(size = self.N)
        # change = np.asarray(np.abs(noise) > 2.0, dtype = int)
        #
        # for i in range(len(change)):
        #     if change[i] > 0.0:
        #         if random.random() > 0.5:
        #             cWeights[i] += cWeights[i] * 0.2
        #         else:
        #             cWeights[i] -= cWeights[i] * 0.2

        cand = np.array(cWeights > 0, dtype = float).reshape(self.N)

        cand = cand % 1.000001

        return cand

    def generate_candidates(self, candidate, i, score):
        '''
        TO DO: Change candidates to your own algorithm
        DON'T FORGET THE \n
        '''
        candidate = self.guess(candidate, score, i)
        return candidate
        if i < 30:
            candidates = "0." + ":0.".join(str(e)
                                        for e in [str(int(random.random()*10000))
                                                    for i in range(self.N)])+"\n"
        else:                                       
            candidates = "1" + ":0"*(self.N-1) + "\n"
        return candidates

if __name__ == '__main__':
    matchmaker = Matchmaker()
    matchmaker.play()
    pass
