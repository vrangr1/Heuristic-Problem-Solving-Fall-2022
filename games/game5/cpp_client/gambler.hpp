#ifndef _GAMBLER_HPP_
#define _GAMBLER_HPP_
#include "gambler_helper.hpp"
#include "fenwick_tree.hpp"

class gambler{
private:
    static int slots, k_max, total_wealth, current_wealth, total_pull_budget, current_pull_budget, previous_wealth, pull_count;
    static vector<pair<int,int> > wins_losses; // wins_losses: ith slot -> (wins, losses),
    static vector<bet_data*> pulls; // pulls: ith pull -> bet details

    // Epsilon Greedy and Upper Confidence Bound Strategies:
    static vector<double> probabilities;
    static double epsilon;

    // Mod Strategy:
    static vector<pair<int, int> > short_history_data; // ith slot -> (wins, losses)
    static int depth, history_length;
    static double total_sum;
    static vector<double> fenwick_tree;
    static vector<double> short_history_probabilities, confidence;
    static vector<int> counts;

    static void call_strategy_function(void (*egreedy)(), void (*ucb)(), void (*mod)()){
        switch(STRATEGY){
            case EGREEDY:
                egreedy();
                break;
            case UCB:
                ucb();
                break;
            case MOD:
                mod();
                break;
        }
    }

    static bet_data* call_strategy_function(bet_data* (*egreedy)(), bet_data* (*ucb)(), bet_data* (*mod)()){
        switch(STRATEGY){
            case EGREEDY:
                return egreedy();
                break;
            case UCB:
                return ucb();
                break;
            case MOD:
                return mod();
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

    static void mod_strategy_setup(){
        // TODO: Tune these parameters
        history_length = 20;
        depth = max(15, total_pull_budget / (slots * k_max));
        print_var(depth);
        short_history_data.clear();
        short_history_data.resize(slots, make_pair(0, 0));
        
        fenwick_tree.clear();
        fenwick_tree.resize(slots + 1, 0.0);
        confidence.clear();
        confidence.resize(slots, 0.01L);
        counts.clear();
        counts.resize(slots, 0);
        vector<double> tree_construction(slots, (((double)depth)*0.01L));
        construct_fenwick_tree(tree_construction, fenwick_tree);
        total_sum = ((double)(depth * slots)) * 0.01L;
        print_var(total_sum);
        double temp_sum = get_sum(fenwick_tree, slots - 1);
        print_var(temp_sum);

        for (int i = 0; i < slots; ++i){
            temp_sum = get_sum(fenwick_tree, i);
            print_var(temp_sum);
            // print_var(get_sum(fenwick_tree, i));
        }
        // assert(total_sum == get_sum(fenwick_tree, slots - 1));
        assert(total_sum == temp_sum);
    }

    static bet_data* mod_strategy(){
        bet_data *bet = new bet_data();
        bet->bet =  1;
        double choice = get_random_range(total_sum);
        bet->slot = lower_bound_on_fenwick_tree(fenwick_tree, 0, slots - 1, choice);
        return bet;
    }

    // counts, short_history_data, total_sum, fenwick_tree, short_history_probabilities, confidence
    static void mod_logging(){
        if (pull_count == 0) return;
        bet_data *last_bet = pulls[pull_count - 1];
        int last_slot = last_bet->slot;
        
        //counts update
        counts[last_slot]++;

        // short_history_data update
        if (last_bet->won) short_history_data[last_slot].first++;
        else short_history_data[last_slot].second++;
        if (last_slot - history_length >= 0){
            bet_data *temp = pulls[last_slot - history_length];
            if (temp->won) short_history_data[temp->slot].first--;
            else short_history_data[temp->slot].second--;
        }

        // short_history_probabilities update:
        short_history_probabilities[last_slot] = get_win_probability(short_history_data, last_slot);

        // confidence_update:
        confidence[last_slot] = ((double)counts[last_slot]) / ((double)depth);

        // fenwick_tree update:
        double last_sum = get_sum(fenwick_tree, last_slot);
        double diff = (((double)(depth - counts[last_slot])) * confidence[last_slot]) - last_sum;
        update_tree(fenwick_tree, last_slot, diff);

        // total_sum update:
        total_sum = get_sum(fenwick_tree, slots - 1);

        probabilities[last_slot] = get_expected_reward(last_slot) + get_upper_confidence_bound(last_slot);
    }

    static void common_beginning_setup(const int &player_wealth, const int &slot_count, const int &pull_budget){
        previous_wealth = NOT_DEFINED;
        total_wealth = player_wealth;
        current_wealth = player_wealth;
        
        slots = slot_count;
        k_max = slots / 7;
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

        call_strategy_function(&egreedy_setup, &ucb_strategy_setup, &mod_strategy_setup);

    }

    static pair<int,int> strategy_move(){
        bet_data *current_bet;
        
        // Log data from previous pull
        call_strategy_function(&egreedy_logging, &ucb_logging, &mod_logging);
        
        // Decide current bet
        current_bet = call_strategy_function(&egreedy_strategy, &ucb_strategy, &mod_strategy);
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

vector<pair<int, int> > gambler::short_history_data;
int gambler::history_length, gambler::depth;
double gambler::total_sum;
vector<double> gambler::fenwick_tree, gambler::short_history_probabilities, gambler::confidence;
vector<int> gambler::counts;
#endif