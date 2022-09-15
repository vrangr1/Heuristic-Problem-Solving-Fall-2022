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

#include "client.hpp"
using namespace std;

int main(int argc, char *argv[]){
	fastIO;
    int k = 4, s = 9, player_num = 1; // computer player
    print_var(s);
    print_var(k);
    int cur_move, stones = s;
    if (player_num == 2){
        cout << "Player " << (player_num % 2 + 1) << " move: ";
        cin >> cur_move;
        stones -= cur_move;
    }
    if (stones == 0){
        cout << "Player 1 wins\n";
        return 0;
    }
    else if (stones < 0){
        cout << "Player 2 wins\n";
        return 0;
    }
    Player player(s, k, player_num);
    while(stones > 0){
        stones -= player.get_next_move(stones);
        if (stones == 0){
            cout << "player " << player_num << " wins\n";
            return 0;
        }
        else if (stones < 0){
            cout << "player " << (player_num % 2 + 1) << " wins\n";
            return 0;
        }
        cout << "player " << (player_num % 2 + 1) << " move: ";
        cin >> cur_move;
        stones -= cur_move;
        print_var(stones);
    }
    if (stones == 0){
        cout << "player " << (player_num % 2 + 1) << " wins\n";
        return 0;
    }
    else if (stones < 0){
        cout << "player " << player_num << " wins\n";
        return 0;
    }
	return 0;
}