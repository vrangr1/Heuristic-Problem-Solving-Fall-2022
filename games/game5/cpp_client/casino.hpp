#ifndef _CASINO_HPP_
#define _CASINO_HPP_
#include "gambler_helper.hpp"

class casino{
private:
    static int turn_number; // This is also number of pulls
    static int turns_since_casino_switch;
    static int starting_wealth;
    static int prev_turn_wealth;
    static int expected_wealth;
    static int number_of_wins;
    static int number_of_loss;
    static int winning_slot;
    static int number_of_pulls;
    static int total_casino_switch;
    // static int slots;
public:

    static int init_func(int switch_budget, int slot_count, int player_wealth){
        // Init with required values
        turn_number = 0;
        total_casino_switch = switch_budget;
        starting_wealth = player_wealth;
        expected_wealth = player_wealth;
        prev_turn_wealth = player_wealth;
        number_of_pulls = 100 * slot_count;
        // TODO: Greater than slot_count/10
        winning_slot = min((double)slot_count, ceil((((double)rand()+1)/ ((double)RAND_MAX)) * ((double)slot_count)));
        return winning_slot;
    }
    
    static int my_func(int switch_budget, int slot_count, int player_wealth, int player_switched){
        return simple_my_func(switch_budget, slot_count, player_wealth, player_switched);
        turn_number++;
        int change = player_wealth - prev_turn_wealth;
        int tokens_bet = abs(change);
        if (change > 0){ // Won
            number_of_wins++;
            expected_wealth = tokens_bet*((slot_count-1)*0.47 + (1) * 0.6)/100;
        }
        else{ // Lost
            number_of_loss++;
            expected_wealth = -tokens_bet*((slot_count-1)*0.53 + (1) * 0.4)/100;
        }
        int bias = player_wealth - expected_wealth;

        return 0;
    }

    static int simple_my_func(int switch_budget, int slot_count, int player_wealth, int player_switched){
        turn_number++;
        if (turn_number%100 == 0){
            cout<< "command "<< turn_number << "\n";
        }
        int diff = floor(((double)number_of_pulls)/((double)total_casino_switch));
        if (turn_number % diff == 0){
            cout << "command Turn_number "<< turn_number << " Diff is " << diff << "\n";
            return min((double)slot_count, ceil((((double)rand()+1)/ ((double)RAND_MAX)) * ((double)slot_count))); 
        }
        return 0;
    }
};

int casino::turn_number; 
int casino::turns_since_casino_switch;
int casino::starting_wealth;
int casino::prev_turn_wealth;
int casino::expected_wealth;
int casino::number_of_wins;
int casino::number_of_loss;
int casino::winning_slot;
int casino::number_of_pulls;
int casino::total_casino_switch;

#endif