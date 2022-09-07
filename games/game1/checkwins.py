import math
import time
class Simulate():
    s = 0
    k = 0
    solution = set()

    def __init__(self, s, k):
        self.s = s
        self.k = k
        self.simulate()

    def yield_make(self, k):
        for i in range(1,k+1):
            yield i
            yield i

    def simulate(self):
        # Length of max sum
        for answer in self.subset_sum_iter(self.yield_make(self.k), self.s):
            self.solution.add(tuple(answer))

    def subset_sum_iter(self, array, target):
        array = sorted(array)
        # Checkpoint A

        last_index = {0: [-1]}
        for i, value in enumerate(array):
            for s in list(last_index.keys()):
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
    if s <= k or s > k*(k+1)/2:
        print("No solution")
    current_time = time.time()        
    simulator = Simulate(s, k)
    print("Time taken: ", time.time() - current_time)
    print(simulator.solution)
