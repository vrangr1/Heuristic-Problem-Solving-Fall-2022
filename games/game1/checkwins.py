import math
import time
class Simulate():
    s = 0
    k = 0
    solution = set()
    p1wins = 0
    p2wins = 0

    def __init__(self, s, k):
        self.s = s
        self.k = k
        self.simulate()

    def yield_make(self, k, extra_val = 0):
        for i in range(1,k+1):
            yield i
            yield i
            if i == extra_val:
                yield i

    def simulate(self):
        # Length of max sum
        # for i in range(self.k + 1): # Goes from 0 to k
        possible_answers = self.subset_sum_iter(self.yield_make(self.k, 0), self.s)
        for answer in possible_answers:
            candidate = tuple(answer)
            self.solution.add(candidate)

    # https://stackoverflow.com/a/64380474
    def subset_sum_iter(self, array, target):
        array = sorted(array)
        # Checkpoint A

        last_index = {0: [-1]}
        for i, value in enumerate(array):
            ref_list = list(last_index.keys())
            for s in ref_list:
                new_s = s + value
                if 0 < (new_s - target):
                    pass # Cannot lead to target
                elif new_s in last_index:
                    last_index[new_s].append(i)
                else:
                    last_index[new_s] = [i]
        # Checkpoint B
        # Now yield up the answers.
        def recur(new_target, max_i):
            for i in last_index[new_target]:
                if i == -1:
                    yield [] # Empty sum.
                elif max_i <= i:
                    break # Not our solution.
                else:
                    for answer in recur(new_target - array[i], i):
                        answer.append(array[i])
                        yield answer

        for answer in recur(target, len(array)):
            yield answer

if __name__ == "__main__":
    s = int(input("Enter the sum: "))
    k = int(input("Enter the number of nim cards: "))
    if s <= k or s > k*(k+1):
        print("No solution")
    current_time = time.time()        
    simulator = Simulate(s, k)
    print("Time taken: ", time.time() - current_time)
    temp = sorted(simulator.solution, key = lambda x: x[0])
    for i in temp:
        print(i)
    for i in simulator.solution:
        if len(i)%2==1:
            simulator.p1wins+=1
        else: 
            simulator.p2wins+=1
    print(simulator.p1wins, simulator.p2wins)
