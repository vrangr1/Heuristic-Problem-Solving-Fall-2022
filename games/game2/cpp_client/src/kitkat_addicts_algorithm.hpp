#ifndef KITKAT_ADDICTS_ALGO_HPP_
#define KITKAT_ADDICTS_ALGO_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <assert.h>
#include <chrono>
#include <cmath>
#include <unordered_map>

#include "io_helper.hpp"
#include "timer.hpp"
#include "disjoint_set.hpp"

#define endl "\n"
#define print_var(x) cout << #x << ": " << x << endl;
#define print_iter(x) cout << #x << ": " << *x << endl;
#define NOT_DEFINED -13923

inline static bool comparator(const vector<int> &first, const vector<int> &second){
    return first[2] < second[2];
}

class Path{
    private:
        const vector<string> lines;
        const time_pt start_time;
        const int sites, days;
        vector<double> x,y;
        vector<double> time;
        // vector<vector<int> > start, end;
        vector<double> val;
        vector<vector<double> > start, end, latest_start;

        void get_input(){
            int flag = 0;
            vector<string> line;
            for (int i = 0; i < this->lines.size(); ++i){
                line = IOClass::split(this->lines[i]);
                if (line.size() == 0) continue;
                else if (flag == 0) flag = 1;
                else if (flag == 1){
                    if (!isdigit(line[0][0])){
                        flag = 2;
                        continue;
                    }
                    int num = stoi(line[0]);
                    this->x[num] = stoi(line[1]);
                    this->y[num] = stoi(line[2]);
                    this->time[num] = stoi(line[3]);
                    this->val[num] = stod(line[4]);
                }
                else if (flag == 2){
                    int site = stoi(line[0]), day = stoi(line[1]);
                    this->start[site][day] = stoi(line[2]);
                    this->end[site][day] = stoi(line[3]);
                    this->latest_start[site][day] = ((this->end[site][day] - this->time[site]/60.0 >= this->start[site][day])?\
                                                    (this->end[site][day] - this->time[site]/60.0) : NOT_DEFINED);
                }
            }
        }

        void dfs(vector<int> &path, vector<bool> &visited, int u){
            path.push_back(u);
            visited[u] = true;
            for (int i = 0; i < this->mst[u].size(); ++i){
                int v = this->mst[u][i];
                if (!visited[v]) dfs(path, visited, v);
            }
        }

        vector<int> calculate_mst(int start){
            vector<bool> visited(this->sites + 1, false);
            vector<int> path;
            dfs(path, visited, start);
            return path;
        }

        vector<vector<int> > get_valid_visit(vector<int> &visit_seq, double &total_value){
            vector<vector<int> > total_visit;
            int day = 1, i = 0, site, prev;
            double time, time_to_site, time_at_site, total_site_time;

            while(day <= this->days && i < visit_seq.size()){
                time = 0;
                prev = NOT_DEFINED;
                vector<int> visit;
                while(time < 24 && i < visit_seq.size()){
                    site = visit_seq[i];

                    // If starting site, otherwise travel time
                    time_to_site = ((prev == NOT_DEFINED)? 0 : \
                        ((abs(this->x[site] - this->x[prev]) + abs(this->y[site] - this->y[prev]))/60.0));
                    
                    time_at_site = this->time[site]/60.0;
                    total_site_time = time + time_to_site + time_at_site;

                    // TODO: Check this
                    if(this->latest_start[site][day] == NOT_DEFINED || (time + time_to_site) > this->latest_start[site][day]){
                        i++;
                        break;
                    }

                    if (time + time_to_site < this->start[site][day])
                        time = this->start[site][day];
                    else time += time_to_site;
                    // time = ((int)total_site_time);
                    time += time_at_site;

                    visit.push_back(site);
                    total_value += this->val[site];
                    i++;
                    prev = site;
                }
                total_visit.push_back(visit);
                day++;
            }
            return total_visit;
        }

        // TODO: Add another loop for 3-opt
        vector<vector<int> > two_opt(vector<int> &path){
            vector<int> best_path = path;
            double max_val = 0;
            vector<vector<int> > res;
            bool improved = true;

            while(improved){
                improved = false;
                for (int i = 0; i < path.size(); ++i){
                    if (time_elapsed(this->start_time) > 110) break;
                    for (int j = i + 1; j < path.size(); ++j){
                        if (time_elapsed(this->start_time) > 110) break;
                        if (j - i == 0) continue;
                        vector<int> new_path, temp(path.begin() + i, path.begin() + j + 1);
                        if (i > 0) new_path.insert(new_path.end(), path.begin(), path.begin() + i - 1);
                        reverse(temp.begin(), temp.end());
                        new_path.insert(new_path.end(), temp.begin(), temp.end());
                        new_path.insert(new_path.end(), path.begin() + j + 1, path.end());
                        double value = 0;
                        vector<vector<int> > ans = get_valid_visit(new_path, value);
                        if (value > max_val){
                            best_path = new_path;
                            max_val = value;
                            res = ans;
                            improved = true;
                        }
                    }
                }
                // print_var(max_val);
                path = best_path;
            }
            return res;
        }

    public:
        unordered_map<int, vector<int> > mst;
        Path(int in_sites, int in_days, time_pt in_time, vector<string> &in_lines) : sites(in_sites), days(in_days), \
                    start_time(in_time), x(in_sites + 1, 0), y(in_sites + 1, 0), val(in_sites + 1, 0), time(in_sites + 1, 0), \
                    start(in_sites + 1, vector<double>(in_days + 1, 0)), end(in_sites + 1, vector<double>(in_days + 1, 0)), \
                    latest_start(in_sites + 1, vector<double>(in_days + 1, 0)), lines(in_lines)
        {
            assert(this->sites > 0);
            assert(this->days > 0);
            this->mst.clear();
            get_input();
        }

        void set_up_mst(){
            vector<vector<int> > edges;
            for (int src = 1; src <= this->sites; ++src){
                for (int dest = src + 1; dest <= this->sites; ++dest){
                    int dist = abs(this->x[dest] - this->x[src]) + abs(this->y[dest] - this->y[src]);
                    vector<int> temp;
                    temp.push_back(src);
                    temp.push_back(dest);
                    temp.push_back(dist);
                    edges.push_back(temp);
                }
            }
            sort(edges.begin(), edges.end(), comparator);
            DisjointSet ds(this->sites + 1);
            vector<vector<int> >::iterator iter;
            int src, dest, dist;
            for (iter = edges.begin(); iter != edges.end(); iter++){
                src = iter->at(0);
                dest = iter->at(1);
                dist = iter->at(2);
                if (ds.find(src) != ds.find(dest)){
                    ds.union_func(src, dest);
                    this->mst[src];
                    this->mst[src].push_back(dest);
                    this->mst[dest];
                    this->mst[dest].push_back(src);
                }
            }
        }

        vector<vector<int> > return_mst_path(){
            int max_val = 0;
            vector<int> best_path, path;
            for (int start = 1; start <= this->sites; ++start){
                path = calculate_mst(start);
                double value = 0;
                vector<vector<int> > ans = get_valid_visit(path, value); // TODO: Remove ans; not used?
                if (value > max_val){
                    best_path = path;
                    max_val = value;
                }
            }
            return two_opt(best_path);
        }
};

#endif