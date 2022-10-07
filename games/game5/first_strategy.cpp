#if 0
    me=`basename $0 .cpp`
    g++ $me.cpp -o $me
    ./$me
    rm $me
    exit
#endif

/***************************************************
* AUTHOR : Anav Prasad
* Nick : vrangr
****************************************************/
#include "setup.hpp"

const Player PLAYER = GAMBLER;
const int s = 100, k = 10;
const int winning_slot = ceil((((double)rand())/ ((double)RAND_MAX)) * ((double)k));

void casino_player(BanditGame &game, const int &tokens, bool gambler_switched_slots){
	// Casino's logic goes here
	game.casino_move(winning_slot);
}

void gambler_player(BanditGame &game){
	int bet, slot;
	// Gambler's Logic goes here
	slot = (winning_slot % game.s) + 1;
	// slot = winning_slot;
	bet = 1;
	game.gambler_move(bet, slot);
}

void solve(){
	assert(winning_slot > 0 && winning_slot <= s);
	BanditGame game(s, k);
	int counter = 100;
	while(true){
		casino_player(game, s, game.changed_gambling_slot());
		gambler_player(game);
	}
}

int main(){
	fastIO;
	TEST1;
	return 0;
}


