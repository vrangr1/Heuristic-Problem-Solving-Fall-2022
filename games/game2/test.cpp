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
#define forn(i, n) for (int i = 0; i < n; i++)
#define forsn(i, st_val, n) for (int i = st_val; i <= n; ++i)
#define forr(i, n) for (int i = n - 1; i >= 0; --i)
#define forsr(i, st_val, n) for (int i = st_val; i >= n; --i)
#define pb1(a) push_back(a)
#define pb2(a,b) push_back({a, b})
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define pb(...) GET_MACRO(__VA_ARGS__, pb1, pb2)(__VA_ARGS__)
#define pass (void)0
#define print_var(x) cout << #x << ": " << x << "\n";
#define print_iter(x) cout << "*" << #x << ": " << *x << endl;
#define space " "

#define debug_mode true

typedef long long int ll;
typedef unsigned long long int ull;
template <typename type> void print(const vector<vector<type> > &arr);
template <typename type> void print(const vector<type> &arr);
template <typename type> void off_print(const vector<type> &arr);
template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr);
template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr);
template <typename t1, typename t2> void print(const pair<t1, t2> &p);

vector<string> split(const string &str, char del = ' '){
    vector<string> tokens;
    size_t n = str.size(), start = 0;
    while(start < n && str[start] == ' ') start++;
    if (start >= n) return tokens;
    size_t end = str.find(del, start);
    while(end != string::npos){
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        while(start < n && str[start] == ' ') start++;
        if (start >= n) break;
        end = str.find(del, start);
    }
    if (start < n) tokens.push_back(str.substr(start));
    return tokens;
}

class something{
    public:
        const vector<string> lines;
        something(const vector<string> &in_lines) : lines(in_lines){
            // print(this->lines);
            cout << this->lines.size() << endl;
        }
};

void solve(){
    // string str = "     The              sole      remaining question     in   life. To be or    not to be?  ";
    // string str = " ";
    // string str = "";
    // print(split(str));
    // ifstream file("given_info.txt");
    // string line;
    // vector<string> lines;
    // while(!file.eof()){
    //     getline(file, line);
    //     lines.push_back(line);
        // print_var(line);
        // cout << "something\n";
        // break;
    // }
    // something smth(lines);
    // print(smth.lines);
    // vector<something> sth;
    // sth.push_back(smth);
    // vector<something>::iterator iter = sth.begin();
    // print(iter->lines);
    unordered_map<int, vector<int> > x;
    if (x.find(0) == x.end()) cout << "0 does not exist\n";
    else cout << "0 exists!\n";
    x[0];
    if (x.find(0) == x.end()) cout << "0 does not exist\n";
    else cout << "0 exists!\n";
}

int main(){
	fastIO;
	TEST1;
	return 0;
}


template <typename type> void print(const vector<vector<type> > &arr){
	cout << "\n[";
	forn(i, arr.size()){
		cout << "[";
		forn(j, arr[i].size() - 1)
			cout << arr[i][j] << ", ";
		cout << arr[i][arr[i].size() - 1] << "]";
		if (i != arr.size() - 1)
			cout << "," << endl;
	}
	cout << "], 2D Vector\n";
}


template <typename type> void print(const vector<type> &arr){
	cout << "\n[";
	forn(i, arr.size()){
		cout << arr[i];
		if (i != arr.size() - 1)
			cout << ", ";
	}
	cout << "], 1D Vector\n";
}

template <typename type> void off_print(const vector<type> &arr){
	forn(i, arr.size())
		cout << arr[i] << " ";
	cout << endl;
}


template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr){
	int n = arr.size();
	cout << "\n[";
	forn(i, n - 1){
		cout << "{" << arr[i].first << "," << arr[i].second << "}, ";
	}
	cout << "{" << arr[n - 1].first << "," << arr[n - 1].second << "}], 1D Vector of Pairs\n";
}


template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr){
	cout << "\n[";
	forn(i, arr.size()){
		cout << "[";
		forn(j, arr[i].size()){
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
