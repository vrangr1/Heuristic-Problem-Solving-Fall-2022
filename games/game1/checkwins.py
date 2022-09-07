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

    def simulate(self):
        # Length of max sum
        n = math.ceil(math.sqrt(self.s))
        p1 = [i for i in range(1,self.k+1)]
        p2 = [i for i in range(1,self.k+1)]
        self.subset_sum(p1+p2, self.s)

    def subset_sum(self, numbers, target, partial=[]):
        s = sum(partial)

        # check if the partial sum is equals to target
        if s == target: 
            self.solution.add(tuple(sorted(partial)))
            # print("sum(%s)=%s" % (partial, target))
        if s >= target:
            return  # if we reach the number why bother to continue

        for i in range(len(numbers)):
            n = numbers[i]
            remaining = numbers[i+1:]
            self.subset_sum(remaining, target, partial + [n]) 


if __name__ == "__main__":
    s = int(input("Enter the sum: "))
    k = int(input("Enter the number of nim cards: "))
    if s <= k or s > k*(k+1)/2:
        print("No solution")
    current_time = time.time()        
    simulator = Simulate(s, k)
    print(simulator.solution)
    print("Time taken: ", time.time() - current_time)
