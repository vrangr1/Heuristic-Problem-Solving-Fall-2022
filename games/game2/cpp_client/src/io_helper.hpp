#ifndef IO_HELPER_HPP_
#define IO_HELPER_HPP_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define INPUT_FILE "given_info.txt"
using namespace std;
#define forn(i, n) for (int i = 0; i < n; i++)
#define forsn(i, st_val, n) for (int i = st_val; i <= n; ++i)
#define forr(i, n) for (int i = n - 1; i >= 0; --i)
#define forsr(i, st_val, n) for (int i = st_val; i >= n; --i)
#define pb1(a) push_back(a)
#define pb2(a,b) push_back({a, b})
#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define pb(...) GET_MACRO(__VA_ARGS__, pb1, pb2)(__VA_ARGS__)
#define pass (void)0
template <typename type> void print(const vector<vector<type> > &arr);
template <typename type> void print(const vector<type> &arr);
template <typename type> void off_print(const vector<type> &arr);
template <typename type> void off_print(const vector<vector<type> > &arr);
template <typename t1, typename t2> void print(const vector<pair<t1,t2> > &arr);
template <typename t1, typename t2> void print(const vector<vector<pair<t1,t2> > > &arr);
template <typename t1, typename t2> void print(const pair<t1, t2> &p);

class IOClass{
    private:
        void get_sites_and_days(vector<string> &lines){
            int flag = 0;
            vector<string> line;
            for (int i = 0; i < lines.size(); ++i){
                line = split(lines[i]);
                if (line.size() == 0) continue;
                else if (flag == 0) flag = 1;
                else if (flag == 1){
                    if (!isdigit(line[0][0])){
                        flag = 2;
                        continue;
                    }
                    this->sites = max(this->sites, stoi(line[0]));
                }
                else if (flag == 2)
                    this->days = max(this->days, stoi(line[1]));
            }
        }
    public:
        int sites, days;
        vector<string> lines;
        IOClass() : lines(0), sites(0), days(0){
            ifstream file(INPUT_FILE);
            while(!file.eof()){
                string line;
                getline(file, line);
                this->lines.push_back(line);
            }
            get_sites_and_days(this->lines);
        }

        static vector<string> split(const string &str, char del = ' '){
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
};


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


template <typename type> void off_print(const vector<vector<type> > &arr){
	forn(i, arr.size())
		off_print(arr[i]);
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

#endif