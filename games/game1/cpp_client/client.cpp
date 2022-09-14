
#include <iostream>
#include <string>
#include <sstream>

// these includes may need to be modified depending on your system
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fstream>
#include <vector>
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
#include <assert.h>

#define debug_mode False
#define WIN 0
#define LOSE 1
#define MAYBE 2

using namespace std;

typedef long long int ll;
typedef unsigned long long int ull;
typedef short int si;
template <typename type> void print(const vector<vector<type> > &arr);
template <typename type> void print(const vector<type> &arr);
template <typename type> void off_print(const vector<type> &arr);
template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr);
template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr);
template <typename t1, typename t2> void print(const pair<t1, t2> &p);


// change this to whatever you want
std::string bot_name = "Do Kitkat Addicts Dream of NP Problems?";

// here you can define custom solver logic
// for example: you could gut this struct and add arrays of cards, custom algorithms, and more
// this is consulted by primary loop in play_game 
class Bot {
    public:
        int last_card, max_cards;

        Bot(int player_num, int num_stones, int num_cards) {
            last_card = 0;
            max_cards = num_cards;
        }

        int calculate_move(int num_stones) {
            return ++last_card;
        }
};

// Everything below here is game logic and socket handling

int socket_id;
struct sockaddr_in server_address;

int get_state() {
	// send getstate request
	send(socket_id, "getstate", 8, 0);

	// read response
	char message[1024];
	read(socket_id, message, 1024);
	return std::stoi(message);
}

void send_move(int move) {
	std::string s = "sendmove " + std::to_string(move);
	send(socket_id, s.c_str(), s.length(), 0);
}

// this function contains the main game loop
void play_game() {
	int player_num, num_stones, num_cards;

	// read initial data
	char message[1024];
	read(socket_id, message, 1024);
	std::stringstream ss(message);
	ss >> player_num >> num_stones >> num_cards;
	std::cout << "Player number: " << player_num << " Number of stones: " << num_stones << " Number of Cards " << num_cards << std::endl;

	// create bot
	Bot b(player_num, num_stones, num_cards);

	// the first player can make a move without getting state first
	// the second needs to make an initial request
	if(player_num == 2) {
		num_stones = get_state();
	}

	// game loop (getstate, sendmove, check if game over, repeat)
	while(num_stones > 0) {
		send_move(b.calculate_move(num_stones));
		usleep(1000);
		num_stones = get_state();
	}
}

// this function handles the socket connection process
void socket_connect(int port) {
	// create socket
	socket_id = socket(AF_INET, SOCK_STREAM, 0);
	if(socket_id < 0) {
		std::cout << "Error creating socket\n";
		exit(-1);
	}
	// set additional required connection info
	server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
	// convert ip address to correct form
	inet_pton(AF_INET, "localhost", &server_address.sin_addr);
	// attempt connection
	if(connect(socket_id, (struct sockaddr*) &server_address, sizeof(server_address)) < 0) {
		std::cout << "Connection failed\n";
		exit(-1);
	}
	// send greeting
	send(socket_id, bot_name.c_str(), bot_name.length(), 0);
}

int main(int argc, char* argv[]) {
	// port is an optional command line argument
	socket_connect(argc == 2 ? std::stoi(argv[1]) : 5000);

	// main game loop
	play_game();
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