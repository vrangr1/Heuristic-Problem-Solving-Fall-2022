#if 0
    me=`basename $0 .cpp`
    g++ $me.cpp -o $me -g -O3
    ./$me
    rm $me
    exit
#endif

// #include "setup.hpp"
#include "gambler.hpp"

const int s = 100, k = 10;

class Casino{
	public:
	const int slots, k;

	Casino(int slots, int k) : slots(slots), k(k){}

	void casino_player(void (BanditGame::*func)(int), BanditGame &game){
		

		// Sample Code
		int current_wealth = game.get_tokens();
		bool gambler_slot_changed = game.changed_gambling_slot();
		static const int winning_slot = (rand() % s) + 1;
		assert(winning_slot > 0 && winning_slot <= s);


		// Command for sending data to architecture
		(game.*func)(winning_slot);
	}
};


class Gambler{
	public:
	const int slots, max_k;
	vector<int> pulls_record;
	Gambler(int given_slots) : slots(given_slots), max_k(given_slots / 7), pulls_record(given_slots + 1, 0){}

	void gambler_player(void (BanditGame::*function)(int, int), BanditGame &game){
		pair<int,int> current_bet = gambler::get_return_data(game.get_tokens(), s, 500*s);
		print(current_bet);
		(game.*function)(current_bet.second, current_bet.first + 1);
	}
};


void casino_player(BanditGame &game, const int &tokens, bool gambler_switched_slots){
	// Casino's logic goes here
	static const int winning_slot = (rand() % s) + 1;
	assert(winning_slot > 0 && winning_slot <= s);
	game.casino_move(winning_slot);
}


void solve(){
	// srand(time(0));
	BanditGame game(s, k);
	Casino casino(s, k);
	Gambler gambler(s);
	while(true){
		// casino_player(game, s, game.changed_gambling_slot());
		casino.casino_player(&BanditGame::casino_move, game);
		gambler.gambler_player(&BanditGame::gambler_move, game);
	}
}

int main(){
	fastIO;
	TEST1;
	return 0;
}


