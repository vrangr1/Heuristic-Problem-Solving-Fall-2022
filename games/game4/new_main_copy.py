import sys
import numpy as np
from sklearn.cluster import KMeans
from collections import Counter, defaultdict
import heapq
from itertools import permutations
import random
from timeit import default_timer as timer

timer_start = timer()

class AmbulancePickup:

    def __init__(self, file_path):

        # parsed
        self.x_loc = []
        self.y_loc = []
        self.rescue_time = []
        self.num_ambulance_at_hospital = []

        self.load_time_per_person = 1
        self.unload_time = 1
        self.max_cpu_time = 40

        # computed
        self.num_hospitals = 0
        self.num_persons = 0
        self.hospital_locations = []
        self.cluster = []
        self.people_locations = np.empty((0,2), int)
        self.ambulances = None
        
        # result
        self.result = []
        self.final_timings = []

        # function calls
        self.read_input(file_path)
        self.assign_cluster()
        self.generate_output()

    def read_input(self, file_path):
        with open(file_path) as fp:
            state = 0
            for _, line in enumerate(fp):
                if len(line.strip()) == 0:
                    continue
                if state == 0:
                    state = 1
                elif state == 1:
                    ln = line.strip().split(",")
                    if line[0].isdigit():
                        # read person's position and save time
                        self.x_loc.append(int(ln[0]))
                        self.y_loc.append(int(ln[1]))
                        self.rescue_time.append(int(ln[2]))
                        self.people_locations = np.append(self.people_locations, np.array([[int(ln[0]), int(ln[1])]]), axis=0)
                    else:
                        # hospital(numambulance)
                        state = 2
                elif state == 2:
                    # read number of ambulances at each hospital
                    ln = line.strip()
                    self.num_ambulance_at_hospital.append(int(ln))
        
        self.num_hospitals = len(self.num_ambulance_at_hospital)
        self.num_persons = len(self.x_loc)

    def assign_cluster(self):
        kmeans = KMeans(n_clusters=self.num_hospitals, random_state=0).fit(self.people_locations)
        print("Kmeans ", kmeans.labels_)
        
        # assign hospital locations
        hospital_locations = kmeans.cluster_centers_.astype(int)
        print("Hospital Locations initial",hospital_locations)

        sorted_ambulance = []
        for i, count in enumerate(self.num_ambulance_at_hospital):
            sorted_ambulance.append((-count, i))
        sorted_ambulance.sort(key = lambda x: x[0])

        hospital_locations_new = [None for _ in range(len(hospital_locations))]
        self.cluster = {sorted_ambulance[i][1]: np.where(kmeans.labels_ == i)[0].tolist() for i in range(self.num_hospitals)}
        counter = Counter(kmeans.labels_)
        for i, (hidx, _) in enumerate(counter.most_common()):
            hospital_locations_new[sorted_ambulance[i][1]] = hospital_locations[hidx]
        self.hospital_locations = hospital_locations_new

        ambulances = []
        for hospital_idx, count in enumerate(self.num_ambulance_at_hospital):
            x, y = self.hospital_locations[hospital_idx]
            for _ in range(count):
                ambulances.append((0, x, y, hospital_idx))
        
        self.ambulances = ambulances
        print("Ambulances", ambulances)

    def get_persons_savable(self, cur_selected, cur_time, cur_x, cur_y, allowed_destinations):
        savable = []
        num_persons = len(self.x_loc)

        for person_idx in range(num_persons):
            x, y, rescue_time = self.x_loc[person_idx], self.y_loc[person_idx], self.rescue_time[person_idx]
            if cur_time > rescue_time:
                continue
            destinations = []
            total_rescue_time_left = 0
            for hospital_idx in allowed_destinations:
                hospital_x, hospital_y = self.hospital_locations[hospital_idx]

                time_to_person = abs(x - cur_x) + abs(y - cur_y)                          # current location to person's location
                time_to_hospital = abs(x - hospital_x) + abs(y - hospital_y)              # person's location to hospital location

                estimated_time = cur_time + time_to_person + self.load_time_per_person + time_to_hospital + self.unload_time

                is_valid = True
                for idx in cur_selected:
                    if estimated_time > self.rescue_time[idx]:
                        is_valid = False
                        break
                
                if is_valid and estimated_time <= rescue_time:
                    total_rescue_time_left+= rescue_time
                    destinations.append(hospital_idx)
                    # savable.append((person_idx, hospital_idx))
        
            if len(destinations) > 0:
                savable.append((person_idx, destinations, total_rescue_time_left))

        return sorted(savable, key=lambda a: abs(self.x_loc[a[0]] - cur_x) + abs(self.y_loc[a[0]] - cur_y) + self.rescue_time[a[0]])
        # return sorted(savable, key=lambda x: x[2])

    def path_time(self, current_ambulance, path, hospital, best_score):
        cur_time, cur_x, cur_y = current_ambulance[0], current_ambulance[1], current_ambulance[2]
        hospital_x, hospital_y = self.hospital_locations[hospital]
        new_path = []
        for person in path:
            if cur_time > self.rescue_time[person]:
                continue
            x, y = self.x_loc[person], self.y_loc[person]
            time_to_person = abs(x - cur_x) + abs(y - cur_y)
            new_time = cur_time + time_to_person + self.load_time_per_person
            if new_time > self.rescue_time[person]:
                continue
            new_path.append(person)
            cur_time = new_time
            cur_x, cur_y = x, y

        time_to_hospital = abs(cur_x - hospital_x) + abs(cur_y - hospital_y)
        cur_time = cur_time + time_to_hospital + self.unload_time
        score = len(new_path) * 1000 - (cur_time)
        for person in new_path:
            if cur_time > self.rescue_time[person]:
                score-=1000
            else:
                score-=self.rescue_time[person]
        return new_path, score

    def get_path(self, current_ambulance, saved):
        num_persons = len(self.x_loc)
        best_score = -10000
        best_path = []
        best_hospital = 0
        destinations = [i for i in range(self.num_hospitals)]
        print(self.cluster[current_ambulance[3]])
        for i, j, k, l in permutations(self.cluster[current_ambulance[3]]+[301], 4): # The +1 is to not include it
            temp_path = []
            if i != 301 and i not in saved:
                temp_path.append(i)
            if j != 301 and j not in saved:
                temp_path.append(j)
            if k != 301 and k not in saved:
                temp_path.append(k)
            if l != 301 and l not in saved:
                temp_path.append(l)
            if len(temp_path) == 0:
                continue
            for hospital in destinations:
                new_path, new_score = self.path_time(current_ambulance, temp_path, hospital, best_score)
                if new_score > best_score:
                    best_path = new_path
                    best_score = new_score
                    best_hospital = hospital
            if (timer() - timer_start) > self.max_cpu_time: 
                break
        return best_path, best_hospital

    def calculate_time(self, current_ambulance, path, hospital):
        cur_time, cur_x, cur_y = current_ambulance[0], current_ambulance[1], current_ambulance[2]
        hospital_x, hospital_y = self.hospital_locations[hospital]

        for person in path:
            x, y = self.x_loc[person], self.y_loc[person]
            time_to_person = abs(x - cur_x) + abs(y - cur_y)
            cur_time = cur_time + time_to_person + self.load_time_per_person
            cur_x, cur_y = x, y

        time_to_hospital = abs(cur_x - hospital_x) + abs(cur_y - hospital_y)
        cur_time = cur_time + time_to_hospital + self.unload_time
        return cur_time

    def find_core_solution(self):
        self.final_timings = []
        result = []
        
        saved = set()
        score = 0
        
        ambulance_heap = self.ambulances
        heapq.heapify(ambulance_heap)
        idx = 0
        while len(ambulance_heap) > 0:
            print("Idx ", idx)
            idx+=1
            current_ambulance = heapq.heappop(ambulance_heap)
            cur_time, cur_x, cur_y = current_ambulance[0], current_ambulance[1], current_ambulance[2]
            start, end, path = current_ambulance[3], current_ambulance[3], []   # index of hospital of ambulance
            start_time = cur_time
            destinations = [i for i in range(self.num_hospitals)]
            path, hospital = self.get_path(current_ambulance, saved)
            if len(path) > 0:
                for person in path:
                    saved.add(person)
                    score+=1
                result.append((start, end, path, start_time))
                actual_time = self.calculate_time(current_ambulance, path, hospital)
                heapq.heappush(ambulance_heap, (actual_time, cur_x, cur_y, hospital))
            if (timer() - timer_start) > self.max_cpu_time: 
                break



            # for _ in range(10):
            #     found = False
            #     for person, destinations, _ in self.get_persons_savable(path, cur_time, cur_x, cur_y, destinations):
            #         hospital = destinations[0]
            #         if person not in saved:
            #             # print("Ambulance: {0}, Person: {1}, Destinations: {2}".format(ambulance_idx+1, person+1, destinations))
            #             found = True
            #             saved.add(person)
            #             path.append(person)
            #             score += 1
            #             x, y = self.x_loc[person], self.y_loc[person]
            #             hospital_x, hospital_y = self.hospital_locations[hospital]
            #             time_to_person = abs(x - cur_x) + abs(y - cur_y)
            #             cur_time = cur_time + time_to_person + self.load_time_per_person
            #             cur_x, cur_y = x, y
            #             end = hospital

            #             if len(path) == 4:
            #                 time_to_hospital = abs(cur_x - hospital_x) + abs(cur_y - hospital_y)
            #                 cur_time = cur_time + time_to_hospital + self.unload_time
            #                 cur_x, cur_y = hospital_x, hospital_y

            #                 result.append((start, end, path, start_time))
            #                 actual_time = self.calculate_time(current_ambulance, path, hospital)
            #                 heapq.heappush(ambulance_heap, (actual_time, cur_x, cur_y, hospital))
            #                 path, start = [], hospital
            #                 found = False # Exit loop, max 4 found
            #             break
                    
            #     if not found:
            #         break

            # if len(path) > 0:
            #     result.append((start, end, path, start_time))
            #     actual_time = self.calculate_time(current_ambulance, path, hospital)
            #     heapq.heappush(ambulance_heap, (actual_time, cur_x, cur_y, hospital))

        return score, result

    def find_best_solution_permutations(self):
        ambulances_list = [i for i in range(len(self.ambulances))]
        permute_amb = permutations(ambulances_list)
        # print("Length of permutations", permute_amb)
        max_score, best_result = 0, None
        for idx, ambulances in enumerate(permute_amb):
            # print(ambulances``)
            if (timer() - timer_start) > self.max_cpu_time: 
                break
            score, result = self.find_solution(ambulances)
            if score > max_score:
                max_score = score
                best_result = result
            print("Update is {0}, {1}".format(idx, score))
        return best_result

    def find_best_solution_two_opt(self):
        ambulances_list = [i for i in range(len(self.ambulances))]
        max_score, best_result = 0, None
        improved = True
      
        while improved:
            improved = False
            for i in range(len(ambulances_list)):
                if (timer() - timer_start) > self.max_cpu_time: break
                for j in range(i + 1, len(ambulances_list)):
                    if (timer() - timer_start) > self.max_cpu_time: break
                    if j - i == 0: continue
                    ambulances = self.swap_2opt(ambulances_list, i, j)
                    score, result  = self.find_solution(ambulances)
                    if score > max_score:
                        print("Improved")
                        max_score = score
                        best_result = result
                        improved = True
        
        return best_result

    def find_best_solution(self):
        max_score, best_result = 0, None
        score, result  = self.find_core_solution()
        best_result = result
        return best_result

    def swap_2opt(self, route, i, j):
      return route[:i] + route[i:j+1][::-1] + route[j+1:]

    def generate_output(self):
        with open("kitkataddicts.txt", "w") as f:

            # print hospitals info
            for hospital_idx in range(self.num_hospitals):
                hospital_x, hospital_y = self.hospital_locations[hospital_idx]
                num_ambulance = self.num_ambulance_at_hospital[hospital_idx]
                print("H{0}:{1},{2}".format(hospital_idx+1,hospital_x, hospital_y, num_ambulance), file=f)
            
            print("", file=f)

            # print path info
            # result = self.find_best_solution_two_opt()
            result = self.find_best_solution()
            # result.sort(key=lambda x: x[0])
            print(result)
            for start, end, path, time in result:
                # hospital_x, hospital_y = self.hospital_locations[start]
                print("{0} H{1} ".format(time, start+1), file=f, end='')

                for person_idx in path:
                    print("P{0} ".format(person_idx+1, self.rescue_time[person_idx]), file=f, end='')

                # hospital_x, hospital_y = self.hospital_locations[end]
                print("H{0} ".format(end+1), file=f)


if __name__ == "__main__":
    input_file = sys.argv[1]
    print("input_file", input_file)

    timer_start = timer()
    game = AmbulancePickup(input_file)
    timer_end = timer()
    print("Time: {0} s".format(timer_end - timer_start))