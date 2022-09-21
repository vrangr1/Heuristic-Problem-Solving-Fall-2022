import sys
from collections import defaultdict
from timeit import default_timer as timer

class DisjointSet:
    def __init__(self, n):
        self.parent = [i for i in range(n)]
        self.rank = [1 for _ in range(n)]
    
    # make a and b part of the same component
    # union by rank optimization
    def union(self, a, b):
        pa = self.find(a)
        pb = self.find(b)
        if pa == pb: return
        if self.rank[pa] > self.rank[pb]:
            self.parent[pb] = pa
            self.rank[pa] += self.rank[pb]
        else:
            self.parent[pa] = pb
            self.rank[pb] += self.rank[pa]
    
    # find the representative of the 
    # path compression optimization
    def find(self, a):
        if self.parent[a] == a:
            return a
        
        self.parent[a] = self.find(self.parent[a])
        return self.parent[a]

class Path:
    def __init__(self, start_time, lines):
        self.start_time = start_time
        self.sites = 0
        self.days = 0
        self.get_sites_and_days(lines)
        assert self.sites > 0
        assert self.days > 0
        print(self.sites," ", self.days)
        self.mst = defaultdict(list)
        self.x = [0 for _ in range(self.sites+1)]
        self.y = [0 for _ in range(self.sites+1)]
        self.val = [0. for _ in range(self.sites+1)]
        self.time = [0 for _ in range(self.sites+1)]
        self.start = [[0 for _ in range(self.days+1)] for _ in range(self.sites+1)]
        self.end = [[0 for _ in range(self.days+1)] for _ in range(self.sites+1)]
        self.latest_start = [[0 for _ in range(self.days+1)] for _ in range(self.sites+1)]
        self.get_input(lines)

    def get_sites_and_days(self, lines):
        flag = 0
        for sentence in lines:
            line = sentence.split() 
            if not line:
                continue
            elif flag == 0:
                flag = 1
            elif flag == 1:
                if not line[0][0].isdigit():
                    flag = 2
                    continue
                self.sites = max(self.sites, int(line[0]))
            elif flag == 2:
                self.days = max(self.days, int(line[1]))

    def get_input(self, lines):
        flag = 0
        for sentence in lines:
            line = sentence.split() 
            if not line:
                continue
            elif flag == 0:
                flag = 1
            elif flag == 1:
                if not line[0][0].isdigit():
                    flag = 2
                    continue
                self.x[int(line[0])] = int(line[1])
                self.y[int(line[0])] = int(line[2])
                self.time[int(line[0])] = int(line[3])
                self.val[int(line[0])] = float(line[4])
            elif flag == 2:
                site = int(line[0])
                day = int(line[1])
                self.start[site][day] = int(line[2])
                self.end[site][day]  = int(line[3])
                self.latest_start[site][day] = (self.end[site][day] - self.time[site]/60) \
                if (self.end[site][day] - self.time[site]/60 >= self.start[site][day]) else None
    
    def set_up_mst(self):
        edges = []
        for src in range(1, self.sites+1):
            for dest in range(src+1, self.sites+1):
                dist = abs(self.x[dest] - self.x[src]) + abs(self.y[dest] - self.y[src])
                edges.append((src, dest, dist))

        edges.sort(key=lambda x: x[2])

        ds = DisjointSet(self.sites+1)
        for src, dest, dist in edges:
            if ds.find(src) != ds.find(dest):
                ds.union(src, dest)
                self.mst[src].append(dest)
                self.mst[dest].append(src)

    def calculate_mst(self, start):
        def dfs(u):
            path.append(u)
            visited.add(u)
            for v in self.mst[u]:
                # Keep on adding till none can be added
                if v not in visited:
                    dfs(v)

        visited = set()
        path = []
        dfs(start)
        return path
    
    def get_valid_visit(self, visit_seq):
        total_visit = []
        total_value = 0
        day, i = 1, 0

        while day <= self.days and i < len(visit_seq):
            time = 0
            prev = None
            visit = []
            while time < 24 and i < len(visit_seq):
                site = visit_seq[i]
                
                # Fill gaps, doesn't increment i 
                if time < self.start[site][day]:
                    time = self.start[site][day]
                    continue
                
                # If starting site, otherwise travel time
                time_to_site = 0 if prev is None else (abs(self.x[site] - self.x[prev]) + abs(self.y[site] - self.y[prev]))/60
                time_at_site = self.time[site]/60
                total_site_time = time + time_to_site + time_at_site
                
                #TODO: Check this
                if not self.latest_start[site][day] or total_site_time > self.end[site][day]:
                    i += 1
                    break

                time = int(total_site_time)
                visit.append(site)
                total_value += self.val[site]
                
                i += 1
                prev = site
            
            total_visit.append(visit)
            day += 1
        
        return total_visit, total_value

    # TODO: Add another loop for 3-opt
    def two_opt(self, path):
        best, max_val, res = path, 0, None
        improved = True
        while improved:
            improved = False
            for i in range(len(path)):
                if (timer() - self.start_time) > 110: 
                    break
                for j in range(i + 1, len(path)):
                    if (timer() - self.start_time) > 110: 
                        break
                    if j - i == 0: continue
                    new_path = path[:i] + path[i:j+1][::-1] + path[j+1:]
                    ans, val = self.get_valid_visit(new_path)
                    if val > max_val:
                        best = new_path
                        max_val = val
                        res = ans
                        improved = True

            path = best
            print("Max:", max_val)

        return res

    def return_mst_path(self):
        best_path, max_val = None, 0
        # MST from each site
        for start in range(1, self.sites + 1):
            path = self.calculate_mst(start)
            ans, val = self.get_valid_visit(path)
            if val > max_val:
                best_path = path
                max_val = val
    
        return self.two_opt(best_path)
        
def main():
    start_time = timer()
    lines = sys.stdin.readlines()
    path = Path(start_time, lines)
    # path.get_sites_and_days(lines)
    # path.get_input(lines)
    path.set_up_mst()
    ans = path.return_mst_path()

    for day in ans:
        print(*day)

    end_time = timer()
    print("Time taken to run code:", end_time - start_time)

if __name__ == '__main__':
    main()
