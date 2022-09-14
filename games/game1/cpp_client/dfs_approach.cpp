#if 0
    me=`basename $0 .cpp`
    g++ $me.cpp -o $me
    ./$me
    rm $me
    exit
#endif


#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <map>
#include <iterator>
#include <iomanip>
#include <algorithm>
#include <climits>
#include <numeric>
#include <cmath>
#include <queue>
#include <unordered_map>
#include <bit>
#include <bitset>
#include <chrono>
#include <assert.h>

// #include "int.hpp"
using namespace std;

#define endl "\n"
#define fastIO ios_base::sync_with_stdio(false),cin.tie(0)
#define pass (void)0
#define print_var(x) cout << #x << ": " << x << "\n";
#define print_iter(x) cout << "*" << #x << ": " << *x << endl;
#define space " "

#define debug_mode false

typedef long long int ll;
typedef unsigned long long int ull;
typedef short int si;
template <typename type> void print(const vector<vector<type> > &arr);
template <typename type> void print(const vector<type> &arr);
template <typename type> void off_print(const vector<type> &arr);
template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr);
template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr);
template <typename t1, typename t2> void print(const pair<t1, t2> &p);
#define NOT_PROCESSED -1
#define LOSS 0

class Simulation{
    
    public:
        vector<si> seen;

        Simulation() : seen(1073741824, NOT_PROCESSED){}

        inline int smallest_choice(int num, int turn_num){
            if (turn_num % 2) num &= 0b111111111111111;
            else num >>= 15;
            int bits;
            for (bits = 0; num != 0; ++bits) num >>= 1;
            return 16 - bits;
        }

        inline static bool check_choice(int num, int choice, int turn_num){
            if (choice > 15) return false;
            else if (turn_num % 2) return num & (1 << (15-choice));
            return (num & (1 << (30 - choice)));
        }

        inline static int get_new_choices(int choices, int index, int turn_num){
            if (turn_num%2) return choices ^ (1 << (15 - index));
            return choices ^ (1 << (30 - index));
        }

        bool can_win(int choices, int remainder, int turn_number){
            int match_key = choices;
            if (smallest_choice(choices, turn_number) > remainder){
                seen[match_key] = LOSS;
                return LOSS;
            }
            else if (check_choice(choices, remainder, turn_number)){
                seen[match_key] = remainder;
                return true;
            }
            else if (seen[match_key] != NOT_PROCESSED){
                return seen[match_key];
            }
            for (int i = 1; i <= 15; ++i){
                if (check_choice(choices, i, turn_number)){
                    int new_choices = get_new_choices(choices, i, turn_number);
                    if (!can_win(new_choices, remainder - i, turn_number + 1)){ //!seen[new_choice]
                        if (turn_number == 0) cout << i << endl;
                        seen[match_key] = i;
                        return true;
                    }
                }
            }
            seen[match_key] = LOSS;
            return LOSS;
        }

        bool can_i_win(int k, int s){
            int summed_choices = (k*(k+1))/2;
            if (summed_choices < s) return false;
            else if (2*summed_choices < s) return false;
            else if (2 * summed_choices == s) return false;

            int choices = pow(2, k) - 1;
            choices = (choices << (15 - k));
            choices = (choices << 15) | choices;
            bool answer = can_win(choices, s, 0);
            return answer;
        }
};


int main(int argc, char *argv[]){
	fastIO;
    int k = 15, s = 120;
    print_var(s);
    print_var(k);
    auto start = chrono::high_resolution_clock::now();
    Simulation simulation;
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken in memory allocation: " << duration.count() / 1e6 << " seconds" << endl;
    start = chrono::high_resolution_clock::now();
    cout << simulation.can_i_win(k,s) << endl;
    stop = chrono::high_resolution_clock::now();
    duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
    return 0;
    int turn_number = 0, p1, p2;
    int choices = pow(2, k) - 1;
    choices = (choices << (15 - k));
    choices = (choices << 15) | choices;
    int total = s;
    while(true){
        cin >> p1;
        choices = simulation.get_new_choices(choices, p1, turn_number);
        print_var(choices);
        cout << "choices: " << bitset<30>(choices) << endl;
        total -= p1;
        print_var(total);
        if (total <= 0) break;
        turn_number++;
        cout << "player 2 turn\n";
        if (simulation.seen[choices] > LOSS){
            if (simulation.check_choice(choices, simulation.seen[choices], turn_number)){
                print_var(simulation.seen[choices]);
                total -= simulation.seen[choices];
                choices = simulation.get_new_choices(choices, simulation.seen[choices], turn_number);
                cout << "new choices1: " << bitset<30>(choices) << endl;
            }
            // turn_number++;
            // print_var(total);
            // continue;
        }
        else if (simulation.seen[choices] == NOT_PROCESSED){
            cout << "processing!\n";
            simulation.can_win(choices, total, turn_number);
            assert(simulation.seen[choices] > LOSS);
            print_var(simulation.seen[choices]);
            total -= simulation.seen[choices];
            choices = simulation.get_new_choices(choices, simulation.seen[choices], turn_number);
            cout << "new choices2: " << bitset<30>(choices) << endl;
            // turn_number++;
            // print_var(total);
            // continue;
        }
        else{
            assert(false);
        }
        
        // for (int i = 1; i <= k; ++i){
        //     print_var(i);
        //     if (simulation.check_choice(choices, i, turn_number)){
        //         int new_choice = simulation.get_new_choices(choices, i, turn_number);
        //         print_var(new_choice);
        //         cout << "new_choice: " << bitset<30>(new_choice) << endl;
        //         if (simulation.seen[new_choice] == NOT_PROCESSED){
        //             simulation.can_win(new_choice, total-i, turn_number+1);
        //         }
        //         // if (simulation.seen.find(new_choice) == simulation.seen.end()){
        //         //     simulation.can_win(new_choice, total - i, turn_number+1);
        //         // }
        //         assert(simulation.seen[new_choice] != NOT_PROCESSED);
        //         if (simulation.seen[new_choice]){
        //             print_var(i);
        //             choices = new_choice;
        //             total -= i;
        //             break;
        //         }
        //     }
        // }
        turn_number++;
        print_var(total);
        if (total <= 0) break;
        if (turn_number > 200) break;
    }
	return 0;
}











template <typename type> void print(const vector<vector<type> > &arr){
	cout << "\n[";
	for(int i; i < arr.size(); ++i){
		cout << "[";
		for(int j; j < arr[i].size() - 1; ++j)
			cout << arr[i][j] << ", ";
		cout << arr[i][arr[i].size() - 1] << "]";
		if (i != arr.size() - 1)
			cout << "," << endl;
	}
	cout << "], 2D Vector\n";
}


template <typename type> void print(const vector<type> &arr){
	cout << "\n[";
	for(int i; i < arr.size(); ++i){
		cout << arr[i];
		if (i != arr.size() - 1)
			cout << ", ";
	}
	cout << "], 1D Vector\n";
}

template <typename type> void off_print(const vector<type> &arr){
	for(int i; i < arr.size(); ++i)
		cout << arr[i] << " ";
	cout << endl;
}


template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr){
	int n = arr.size();
	cout << "\n[";
	for(int i; i < n - 1; ++i)
		cout << "{" << arr[i].first << "," << arr[i].second << "}, ";
	cout << "{" << arr[n - 1].first << "," << arr[n - 1].second << "}], 1D Vector of Pairs\n";
}


template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr){
	cout << "\n[";
	for(int i; i < arr.size(); ++i){
		cout << "[";
		for(int j; j < arr[i].size(); ++j){
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