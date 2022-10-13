#ifndef _GAMBLER_HPP_
#define _GAMBLER_HPP_
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


const STRATEGIES STRATEGY = UCB;

class Bet{
public:
    int bet, slot;
    bool won;
    Bet(int slot, int bet){
        this->bet = bet;
        this->slot = slot;
        this->won = false;
    }
    Bet(int slot, int bet, bool won){
        this->bet = bet;
        this->slot = slot;
        this->won = won;
    }
};

inline int get_random(int n){
    return rand() % n + 1;
}

class gambler{
private:
    static int slots, k_max, total_wealth, current_wealth, total_pull_budget, current_pull_budget, previous_wealth, pull_count;
    static vector<pair<int,int> > wins_losses; // wins_losses: ith slot -> (wins, losses),
    static vector<Bet*> pulls; // pulls: ith pull -> bet details

    // Basic Strategy:
    static set<int> remaining_slots;
    static int evaluation_count;

    static void call_strategy_function(void (*egreedy)(), void (*ucb)()){
        switch(STRATEGY){
            case EGREEDY:
                egreedy();
                break;
            case UCB:
                ucb();
                break;
        }
    }

    static Bet* call_strategy_function(Bet* (*egreedy)(), Bet* (*ucb)()){
        switch(STRATEGY){
            case EGREEDY:
                return egreedy();
                break;
            case UCB:
                return ucb();
                break;
        }
    }

    static void log_data(const int &player_wealth){
        previous_wealth = current_wealth;
        current_wealth = player_wealth;
        pulls[pull_count]->won = (current_wealth > previous_wealth);
        if (pulls[pull_count]->won) wins_losses[pulls[pull_count]->slot].first += 1;
        else wins_losses[pulls[pull_count]->slot].second += 1;
        assert(pulls.size() == pull_count);
    }

    static void log_pull(Bet *current_bet){
        pulls.push_back(current_bet);
        pull_count++;
    }

    static void common_beginning_setup(const int &player_wealth, const int &slot_count, const int &pull_budget){
        previous_wealth = NOT_DEFINED;
        total_wealth = player_wealth;
        current_wealth = player_wealth;
        
        slots = slot_count;
        
        total_pull_budget = pull_budget;
        current_pull_budget = pull_budget;
        pull_count = 0;
        pulls.clear();
        
        wins_losses.clear();
        wins_losses.resize(slots, make_pair(0, 0));
    }

    static void egreedy_setup(){
        for (int i = 0; i < slots; ++i)
            remaining_slots.insert(i);
    }

    static void ucb_strategy_setup(){}

    static void update(bool &init, const int &player_wealth, const int &slot_count, const int &pull_budget){
        if (!init) return log_data(player_wealth);
        
        srand(time(0));
        init = false;
        
        common_beginning_setup(player_wealth, slot_count, pull_budget);

        call_strategy_function(&egreedy_setup, &ucb_strategy_setup);

        evaluation_count = max(15, pull_budget / slot_count);
        print_var(evaluation_count);
    }

    static Bet* egreedy_strategy(){
        // basic epsilon-greedy strategy while not accounting for slot shifts
        return nullptr;
    }

    static Bet* ucb_strategy(){
        // return make_pair(1, 1);
        return nullptr;
    }

    static pair<int,int> strategy_move(){
        Bet *current_bet;
        current_bet = call_strategy_function(&egreedy_strategy, &ucb_strategy);
        log_pull(current_bet);
        return make_pair(current_bet->slot, current_bet->bet);
    }

public:
    static pair<int, int> get_return_data(int player_wealth, int slot_count, int pull_budget){
        static bool init = true;
        update(init, player_wealth, slot_count, pull_budget);
        return strategy_move();
    }
};


int gambler::slots, gambler::k_max, gambler::total_wealth, gambler::current_wealth, gambler::total_pull_budget;
int gambler::current_pull_budget, gambler::previous_wealth, gambler::pull_count;
vector<pair<int, int> > gambler::wins_losses;
vector<Bet*> gambler::pulls;

int gambler::evaluation_count;
set<int> gambler::remaining_slots;
#endif