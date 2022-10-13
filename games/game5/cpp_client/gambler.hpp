#ifndef _GAMBLER_HPP_
#define _GAMBLER_HPP_
#include "gambler_helper.hpp"

class gambler{
private:
    static int slots, k_max, total_wealth, current_wealth, total_pull_budget, current_pull_budget, previous_wealth, pull_count;
    static vector<pair<int,int> > wins_losses; // wins_losses: ith slot -> (wins, losses),
    static vector<bet_data*> pulls; // pulls: ith pull -> bet details

    // Epsilon Greedy Strategy:
    static vector<double> probabilities;
    static double epsilon;

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

    static bet_data* call_strategy_function(bet_data* (*egreedy)(), bet_data* (*ucb)()){
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
        assert(pulls.size() == pull_count);
        bet_data *last_bet = pulls[pull_count - 1];
        last_bet->won = (current_wealth > previous_wealth);
        if (last_bet->won) wins_losses[last_bet->slot].first += 1;
        else wins_losses[last_bet->slot].second += 1;
    }

    static void log_pull(bet_data *current_bet){
        pulls.push_back(current_bet);
        pull_count++;
    }

    static double get_expected_reward(int slot_index){ // TODO: Try the other expected reward function
        double wins = wins_losses[slot_index].first, losses = wins_losses[slot_index].second;
        double total = wins + losses;
        return (total + wins - losses)/ (2 * total); // in [0, 1] range
    }

    static double get_upper_confidence_bound(int slot_index){
        double total = wins_losses[slot_index].first + wins_losses[slot_index].second;
        return sqrt((2*log(pull_count)) / total);
    }

    static void egreedy_setup(){
        probabilities.clear();
        probabilities.resize(slots, 0.0);
        epsilon = 0.4;
    }

    static bet_data* egreedy_strategy(){
        // basic epsilon-greedy strategy while not accounting for slot shifts
        bet_data *current_bet = new bet_data();
        current_bet->bet = 1;
        double rng = get_random();
        #if debug_mode
        print(probabilities);
        print_var(rng);
        print_var(slots);
        #endif
        if (rng < epsilon)
            current_bet->slot = get_random_range(slots);
        else
            current_bet->slot = get_index_highest_value(probabilities);
        #if debug_mode
        if (pull_count > 30)
            current_bet->slot = 0, current_bet->bet = 0;
        #endif
        return current_bet;
    }

    static void egreedy_logging(){
        if (pull_count == 0) return;
        int last_slot = pulls[pull_count - 1]->slot;
        probabilities[last_slot] = get_expected_reward(last_slot);
    }

    static void ucb_strategy_setup(){
        probabilities.clear();
        probabilities.resize(slots, 1.0);
    }

    static bet_data* ucb_strategy(){
        bet_data *current_bet = new bet_data();
        #if debug_mode
        print(probabilities);
        #endif
        current_bet->bet = 1;
        current_bet->slot = get_index_highest_value(probabilities);
        #if debug_mode
        if (pull_count > 30)
            current_bet->slot = 0, current_bet->bet = 0;
        #endif
        return current_bet;
    }

    static void ucb_logging(){
        if (pull_count == 0) return;
        int last_slot = pulls[pull_count - 1]->slot;
        probabilities[last_slot] = get_expected_reward(last_slot) + get_upper_confidence_bound(last_slot);
    }

    static void common_beginning_setup(const int &player_wealth, const int &slot_count, const int &pull_budget){
        previous_wealth = NOT_DEFINED;
        total_wealth = player_wealth;
        current_wealth = player_wealth;
        
        slots = slot_count;
        #if debug_mode
        cout << "in common setup func\n";
        print_var(slots);
        #endif
        
        total_pull_budget = pull_budget;
        current_pull_budget = pull_budget;
        pull_count = 0;
        pulls.clear();
        
        wins_losses.clear();
        wins_losses.resize(slots, make_pair(0, 0));
        #if debug_mode
        cout << endl;
        #endif
    }

    static void update(bool &init, const int &player_wealth, const int &slot_count, const int &pull_budget){
        if (!init) return log_data(player_wealth);
        
        srand(time(0));
        init = false;
        
        common_beginning_setup(player_wealth, slot_count, pull_budget);

        call_strategy_function(&egreedy_setup, &ucb_strategy_setup);

    }

    static pair<int,int> strategy_move(){
        bet_data *current_bet;
        
        // Log data from previous pull
        call_strategy_function(&egreedy_logging, &ucb_logging);
        
        // Decide current bet
        current_bet = call_strategy_function(&egreedy_strategy, &ucb_strategy);
        // print_itr(current_bet);
        
        // Log the current bet
        log_pull(current_bet);
        return make_pair(current_bet->slot + 1, current_bet->bet);
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
vector<bet_data*> gambler::pulls;

vector<double> gambler::probabilities;
double gambler::epsilon;
#endif