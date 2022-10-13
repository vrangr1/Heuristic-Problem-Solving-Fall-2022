#ifndef _SETUP_HPP_
#define _SETUP_HPP_

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
#include <assert.h>

using namespace std;

#define endl "\n"
#define fastIO ios_base::sync_with_stdio(false),cin.tie(0)
#define TEST int T;cin>>T;while(T--)solve();
#define TEST1 solve();
#define pass (void)0
// #define print_var(x) cout << #x << ": " << x << "\n";
#define print_iter(x) cout << "*" << #x << ": " << *x << endl;
#define space " "
#define NOT_DEFINED1 -12923

#define debug_mode false


#define NORMAL_SLOT 0.47
#define WINNING_SLOT 0.6


class BanditGame{
private:
    int winning_slot, tokens, gambler_slot, pulls, previous_slot;
    const int s, k;
public:
    BanditGame(int s, int k) : s(s), k(k){
        assert(s <= 100);
        assert(k <= s/7);
        this->winning_slot = NOT_DEFINED1;
        this->gambler_slot = NOT_DEFINED1;
        this->previous_slot = NOT_DEFINED1;
        this->tokens = 500 * this->s;
        this->pulls = 0;
    }

    inline int get_tokens(){
        return this->tokens;
    }

    void casino_move(int choice, bool debug){
        // if (debug) print_var(choice);
        static int changes = 0;
        if (this->winning_slot == NOT_DEFINED1) this->winning_slot = choice; // game start
        else if (this->winning_slot != choice && changes++ < k) this->winning_slot = choice;
        else if (this->winning_slot != choice) assert(false); // Illegal move by casino
        else pass;// this->winning_slot == choice
    }

    void casino_move(int choice){
        static int changes = 0;
        if (this->winning_slot == NOT_DEFINED1) this->winning_slot = choice; // game start
        else if (this->winning_slot != choice && changes++ < k) this->winning_slot = choice;
        else if (this->winning_slot != choice) assert(false); // Illegal move by casino
        else pass;// this->winning_slot == choice
    }

    bool changed_gambling_slot(){
        if (this->previous_slot != NOT_DEFINED1)
            return this->gambler_slot != this->previous_slot;
        return false;
    }

    void gambler_move(int bet, int slot, bool debug){
        if (bet == 0) game_over();
        assert(bet > 0 && bet <= 3);
        assert(slot >= 1 && slot <= this->s);
        assert(this->winning_slot != NOT_DEFINED1);
        this->pulls++;
        this->previous_slot = this->gambler_slot;
        this->gambler_slot = slot;
        // if (debug) print_var(slot);
        double predict = rand();
        predict /= RAND_MAX;
        // 0 <= predict <= 1
        bet *= (predict <= (slot == this->winning_slot? WINNING_SLOT : NORMAL_SLOT)? 1 : -1);
        // if (debug){
        //     print_var(predict);
        //     print_var(bet);
        // }
        this->tokens += bet;
        if (debug){
            cout << "After Gambler's move, the tokens remaining are: " << this->tokens << endl;
            cout << endl;
        }
        if (this->pulls >= 100 * this->s) this->game_over();
    }

    void gambler_move(int bet, int slot){
        if (bet == 0) game_over();
        assert(bet > 0 && bet <= 3);
        assert(slot >= 1 && slot <= this->s);
        assert(this->winning_slot != NOT_DEFINED1);
        this->pulls++;
        this->previous_slot = this->gambler_slot;
        this->gambler_slot = slot;
        double predict = rand();
        predict /= RAND_MAX;
        // 0 <= predict <= 1
        bet *= (predict <= (slot == this->winning_slot? WINNING_SLOT : NORMAL_SLOT)? 1 : -1);
        this->tokens += bet;
        if (this->pulls >= 100 * this->s) this->game_over();
    }

    inline void game_over(){
        this->~BanditGame();
    }

    ~BanditGame(){
        cout << "Game over!\n";
        cout << "Gambler's tokens: " << this->tokens << endl;
        cout << "Exiting now...\n";
        exit(0);
    }
};

#endif