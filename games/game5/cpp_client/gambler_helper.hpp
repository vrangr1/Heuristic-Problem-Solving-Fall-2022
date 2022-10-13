#ifndef _GAMBLER_HELPER_HPP_
#define _GAMBLER_HELPER_HPP_
#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <set>
#include <cmath>
#include <assert.h>
#define NOT_DEFINED -97293

#define endl "\n"
#define print_var(x) cout << #x << ": " << x << endl;
#define print_itr(x) cout << #x << ": " << *x << endl;
#define debug_mode false

using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
template <typename type> void print(const vector<vector<type> > &arr);
template <typename type> void print(const vector<type> &arr);
template <typename type> void off_print(const vector<type> &arr);
template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr);
template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr);
template <typename t1, typename t2> void print(const pair<t1, t2> &p);

enum STRATEGIES{
    EGREEDY,
    UCB
};


const STRATEGIES STRATEGY = EGREEDY;
// const double epsilon = 0.4;


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

    friend ostream& operator<<(ostream& os, bet_data const & bet_data) {
        os << "bet: " << bet_data.bet << endl;
        os << "slot: " << bet_data.slot << endl;
        return os;
    }
};

inline int get_random_range(int n, bool print = true){ // returns a integer random number in [0, n)
    int rng = rand() % n;
    #if debug_mode
    if (print){
        cout << "random range called\n";
        print_var(n);
        print_var(rng);
    }
    #endif
    return rng;
}

inline double get_random(){
    double rng = rand() % (int)(1e5 + 1);
    return rng / ((double)1e5);
}

template <typename type> int get_index_highest_value(const vector<type> &arr){
    #if debug_mode
    cout << "get highest index\n";
    #endif
    int count = 1, best_index = 0;
    type prev_best = arr[0];
    for (int i = 1; i < arr.size(); ++i){
        if (arr[i] == prev_best) count++;
        if (arr[i] > prev_best) {
            count = 1;
            prev_best = arr[i];
        }
        if (arr[i] == prev_best && get_random_range(count, false) == 0) best_index = i;
    }
    #if debug_mode
    print_var(best_index);
    cout << endl;
    #endif
    return best_index;
}



template <typename type> void print(const vector<vector<type> > &arr){
	cout << "\n[";
	for (int i = 0; i < arr.size(); ++i){
		cout << "[";
		for (int j = 0; j < arr[i].size() - 1; ++j)
			cout << arr[i][j] << ", ";
		cout << arr[i][arr[i].size() - 1] << "]";
		if (i != arr.size() - 1)
			cout << "," << endl;
	}
	cout << "], 2D Vector\n";
}


template <typename type> void print(const vector<type> &arr){
	cout << "\n[";
	for (int i = 0; i < arr.size(); ++i){
		cout << arr[i];
		if (i != arr.size() - 1)
			cout << ", ";
	}
	cout << "], 1D Vector\n";
}

template <typename type> void off_print(const vector<type> &arr){
	for (int i = 0; i < arr.size(); ++i)
		cout << arr[i] << " ";
	cout << endl;
}


template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr){
	int n = arr.size();
	cout << "\n[";
	for (int i = 0; i < n - 1; ++i)
		cout << "{" << arr[i].first << "," << arr[i].second << "}, ";
	cout << "{" << arr[n - 1].first << "," << arr[n - 1].second << "}], 1D Vector of Pairs\n";
}


template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr){
	cout << "\n[";
	for (int i = 0; i < arr.size(); ++i){
		cout << "[";
		for (int j = 0; j < arr[i].size(); ++j){
			cout << "{" << arr[i][j].first << "," << arr[i][j].second << "}";
			if (j != arr[i].size() - 1)
				cout << ", ";
		}
		cout << "]";
		if (i != arr.size() - 1)
			cout << "," << endl;
	}
	cout << "], 2D Vector of Pairs\n";
}


template <typename t1, typename t2> void print(const pair<t1, t2> &p){
	cout << "\n{" << p.first << "," << p.second << "}, Pair\n";
}

#endif