#ifndef _GAMBLER_HELPER_HPP_
#define _GAMBLER_HELPER_HPP_
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <assert.h>
#define NOT_DEFINED -97293

#define endl "\n"
#define print_var(x) cout << #x << ": " << x << endl;

using namespace std;

enum STRATEGIES{
    EGREEDY,
    UCB
};


const STRATEGIES STRATEGY = EGREEDY;
const double epsilon = 0.4;


class bet_data{
public:
    int bet, slot;
    bool won;

    bet_data(){
        this->bet = -1;
        this->slot = -1;
        this->won = false;
    }
    bet_data(int slot, int bet){
        this->bet = bet;
        this->slot = slot;
        this->won = false;
    }
};

inline int get_random_range(int n){ // returns a integer random number in [0, n)
    return rand() % n;
}

inline double get_random(){
    double rng = rand() % (int)(1e5 + 1);
    return rng / ((double)1e5);
}

template <typename type> int get_index_highest_value(const vector<type> &arr){
    int count = 1, best_index = 0;
    type prev_best = arr[0];
    for (int i = 1; i < arr.size(); ++i){
        if (arr[i] == prev_best) count++;
        if (arr[i] > prev_best) {
            count = 1;
            prev_best = arr[i];
        }
        if (get_random_range(1) == 0) best_index = i;
    }
    return best_index;
}

#endif