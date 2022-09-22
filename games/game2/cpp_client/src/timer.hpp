#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <iostream>
#include <chrono>

using namespace std;

typedef long long int ll;
typedef chrono::time_point<chrono::high_resolution_clock> time_pt;
typedef chrono::microseconds time_ms;
typedef chrono::seconds time_s;

time_pt time_elapsed(bool in_seconds = true, bool print = false){
    static bool first = true;
    static time_pt start = chrono::high_resolution_clock::now();
    if (first){
        first = false;
        return start;
    }
    time_pt stop = chrono::high_resolution_clock::now();
    if (!in_seconds){
        time_ms duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        if (print) cout << "Time elapsed: " << duration.count() << " microseconds" << endl;
        start = chrono::high_resolution_clock::now();
        return start;
    }
    time_s duration = chrono::duration_cast<time_s>(stop-start);
    if (print) cout << "Time elapsed: " << duration.count() << " seconds" << endl;
    start = chrono::high_resolution_clock::now();
    return start;
}

ll time_elapsed(time_pt start, bool in_seconds = true, bool print = false){
    time_pt stop = chrono::high_resolution_clock::now();
    if (!in_seconds){
        time_ms duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        if (print) cout << "Time elapsed: " << duration.count() << " microseconds" << endl;
        start = chrono::high_resolution_clock::now();
        return duration.count();
    }
    time_s duration = chrono::duration_cast<time_s>(stop-start);
    if (print) cout << "Time elapsed: " << duration.count() << " seconds" << endl;
    start = chrono::high_resolution_clock::now();
    return duration.count();
}

#endif