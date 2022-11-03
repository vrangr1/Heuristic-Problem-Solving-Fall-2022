import socket
import time
import sys
import utils
import numpy as np
from signal import signal, SIGPIPE, SIG_DFL  
signal(SIGPIPE,SIG_DFL)

class Dater:
    def __init__(self):
        self.clientsocket = socket.socket()
        self.clientsocket.connect(('localhost', 20000))
        self.socketfile = self.clientsocket.makefile('rw')
        self.socketfile.write('Person\n')
        self.socketfile.flush()
        input_line = self.socketfile.readline()
        print("Server says:" + input_line)
        self.N = (int) ((input_line.split(":"))[1])
        if (self.N < 2 or self.N > 200):
            print("invalid N")
            sys.exit()
        self.initial_weights = []

    def play(self):
        # # Generate the initial weights (First Round)
        time.sleep(.1)
        self.weights_init = self.generate_init_weight()
        self.socketfile.write(self.weights_init)
        self.socketfile.flush()
        self.weights_init = self.weights_init.split(":")
        self.weights_init = [float(x) for x in self.weights_init]
        # # Generate the adjusted weights (Next Rounds)
        for i in range(20):
            input_line = self.socketfile.readline()
            if input_line:
                print("Server says:" + input_line)
            candidate_values = input_line.split(":")
            time.sleep(.1)
            # assert len(candidate_values) == self.N, "candidate_values: " + str(candidate_values)
            # print(candidate_values)
            if len(candidate_values) == self.N:
                weights = self.generate_adjusted_weight(candidate_values)
            else:
                # weights = "1:-1" + ":0"*(self.N-2) + "\n"
                pass
            self.socketfile.write(weights)
            self.socketfile.flush()
            
        self.clientsocket.close()

    def get_valid_weights(self):
        # Set initial weight here
        while(True):
            self.initial_weights = utils.get_valid_weights(self.N)
            if utils.check_sum(self.initial_weights):
                return self.initial_weights
        assert False
        return self.initial_weights 
        
    def get_ideal_candidate(self):
        # Initial setup for ideal candidate, change accordingly
        return self.initial_weights > 0

    def get_anti_ideal_candidate(self):
        # Initial setup for anti ideal candidate, change accordingly
        return self.initial_weights <= 0

    def get_new_weights(self, response):
        # Use guess and self.initial_weights to calculate new data
        # send back initial weight for now
        # response = utils.parse_response(guess)
        # print("self.weights_init: " + str(self.weights_init))
        # self.weights_init = 
        # self.weights_init = [float(x) for x in self.weights_init]

        self.weights_init = np.array(self.weights_init, dtype = float)
        noise = utils.get_noise(self.weights_init, response)
        return self.weights_init + noise

    def generate_init_weight(self):
        '''
        TO DO: Change weights to your own algorithm
        DON'T FORGET THE \n
        '''
        initial_weights = self.get_valid_weights()
        return utils.floats_to_msg2(initial_weights)
        idealCandidate = self.get_ideal_candidate()
        antiIdealCandidate = self.get_anti_ideal_candidate()
        self.sock.sendall(utils.candidate_to_msg(idealCandidate))
        self.sock.sendall(utils.candidate_to_msg(antiIdealCandidate))

        weights = "1:-1" + ":0"*(self.N-2) + "\n"

        return weights


    def generate_adjusted_weight(self, candidate_values):
        '''
        N: int
        candidates_values: string array -> is it really?
        '''
        
        '''
        TO DO: Change weights to your own algorithm
        DON'T FORGET THE \n
        '''
        # print("candidate_values: ", end = '')
        # print(candidate_values)
        candidate_values = [float(x) for x in candidate_values]
        return utils.floats_to_msg2(self.get_new_weights(candidate_values))
        weights = "1:-1" + ":0"*(self.N-2) + "\n"

        return weights
  

if __name__ == '__main__':
    dater = Dater()
    dater.play()
