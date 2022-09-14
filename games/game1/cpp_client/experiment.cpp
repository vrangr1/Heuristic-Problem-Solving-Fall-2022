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
#include <assert.h>

using namespace std;

#define endl "\n"
#define fastIO ios_base::sync_with_stdio(false),cin.tie(0)
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

class int15{
public:
    unsigned int val : 15;
    int15(unsigned int val){
        this->val = val;
    }

    int15& operator =(unsigned int x){
        this->val = x;
        return *this;
    }
};

ostream& operator<<(ostream& os, const int15& x)
{
    os << x.val;
    return os;
}

int main(int argc, char *argv[]){
	fastIO;
    // cout << sizeof(int15);
    int15 y(32767);
    // y = 32767;
    // cout << y << endl;
    // unsigned int x = 0b11101101101111;
    // cout << __countr_one(x) << endl;
    vector<int> someting(5,0);
    vector<bool> arr(10, false);
    unordered_map<int15, bool> seen;
    seen[y] = true;
    int15 x(32767);
    print_var(seen[x]);
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