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

using namespace std;

#define endl "\n"
#define fastIO ios_base::sync_with_stdio(false),cin.tie(0)
#define pass (void)0
#define print_var(x) cout << #x << ": " << x << "\n";
#define print_iter(x) cout << "*" << #x << ": " << *x << endl;
#define space " "

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
#define debug_mode false

class Simulation{
    
    public:
        static void timer_func(bool in_seconds = false){
            static bool first = true;
            static auto start = chrono::high_resolution_clock::now();
            if (first){
                first = false;
                return;
            }
            auto stop = chrono::high_resolution_clock::now();
            auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
            if (in_seconds)
                cout << "Time taken in memory allocation: " << duration.count() / 1e6 << " seconds" << endl;
            else
                cout << "Time taken by function: " << duration.count() << " microseconds" << endl;
            start = chrono::high_resolution_clock::now();
        }
        vector<si> seen;

        

        inline int smallest_choice(int num, int turn_num){
            if (turn_num % 2) num &= 0b111111111111111;
            else num >>= 15;
            int bits;
            for (bits = 0; num != 0; ++bits) num >>= 1;
            return 16 - bits;
        }

        inline static bool check_choice(int choices, int choice, int turn_num){
            if (choice > 15) return false;
            else if (turn_num % 2) return choices & (1 << (15-choice));
            return (choices & (1 << (30 - choice)));
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
                if (check_choice(choices, i, turn_number) && remainder - i >= 0){
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

        Simulation(int k, int s) : seen(1073741824, NOT_PROCESSED){
            Simulation::timer_func(true);
            can_i_win(k,s);
            Simulation::timer_func();
        }
};


class Player{
    private:
        int s, k, player_num;
        int cur_stones;
        int choices, turn_number;
        Simulation *simulator;
        bool debug_print;
    public:
        Player(int s, int k, int player_num){
            assert(s <= 200 && k <= 15);
            assert(player_num > 0 && player_num < 3);
            this->s = s;
            this->k = k;
            this->player_num = player_num;
            Simulation::timer_func();
            this->simulator = new Simulation(k, s);
            this->choices = pow(2, k) - 1;
            this->choices = (this->choices << (15 - k));
            this->choices = (this->choices << 15) | choices;
            this->cur_stones = this->s;
            this->turn_number = player_num - 3;
            this->debug_print = true;
        }
        ~Player(){
            delete this->simulator;
        }
        
        int get_next_move(int updated_stones){
            assert(this->simulator != nullptr);
            this->turn_number += 2;
            if (this->turn_number > 0)
            assert(updated_stones != this->cur_stones);
            int other_player_move = this->cur_stones - updated_stones;
            this->cur_stones = updated_stones;
            
            // Update other player's choice:
            if (this->turn_number - 1 >= 0)
            this->choices = this->simulator->get_new_choices(this->choices, other_player_move, this->turn_number - 1);

            assert(this->cur_stones > 0);
            int move = this->simulator->seen[this->choices];
            if (move > LOSS){
                assert(this->simulator->check_choice(this->choices, move, this->turn_number));
                this->cur_stones -= move;
                this->choices = this->simulator->get_new_choices(this->choices, move, this->turn_number);
                if (this->debug_print){
                    print_var(move);
                    print_var(this->cur_stones);
                }
                return move;
            }
            else if (move == NOT_PROCESSED){
                if (this->debug_print)
                    cout << "processing\n";
                this->simulator->can_win(this->choices, this->cur_stones, this->turn_number);
                move = this->simulator->seen[this->choices];
                assert(move >= LOSS);
                // TODO: Write code for move == LOSS
                assert(move != LOSS);
                assert(this->simulator->check_choice(this->choices, move, this->turn_number));
                this->cur_stones -= move;
                this->choices = this->simulator->get_new_choices(this->choices, move, this->turn_number);
                if (this->debug_print){
                    print_var(move);
                    print_var(this->cur_stones);
                }
                return move;
            }
            // TODO: Write for the case of move == LOSS;
            assert(move == LOSS);
            assert(false);
        }
};


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