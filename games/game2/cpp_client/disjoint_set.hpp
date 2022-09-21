#ifndef DISJOINT_SET_HPP_
#define DISJOINT_SET_HPP_

#include <vector>
using namespace std;


class DisjointSet{
    private:
        vector<int> parent, rank;
    public:
        DisjointSet(int n){
            for (int i = 0; i < n; ++i){
                this->parent.push_back(i);
                this->rank.push_back(i);
            }
        }

        // find the representative of the path compression optimization
        int find(int a){
            if (this->parent[a] == a) return a;
            this->parent[a] = this->find(this->parent[a]);
            return this->parent[a];
        }

        // make a and b part of the same component union by rank optimization
        void union_func(int a, int b){
            int pa = find(a), pb = find(b);
            if (pa == pb) return;
            if (this->rank[pa] > this->rank[pb]){
                this->parent[pb] = pa;
                this->rank[pa] += this->rank[pb];
            }
            else{
                this->parent[pa] = pb;
                this->rank[pb] += this->rank[pa];
            }
        }
};

#endif