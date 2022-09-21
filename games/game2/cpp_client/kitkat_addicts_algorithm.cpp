#if 0
    me=`basename $0 .cpp`
    g++ $me.cpp -o $me
    ./$me
    rm $me
    exit
#endif

#include "kitkat_addicts_algorithm.hpp"


int main(){
    time_pt start_time = time_elapsed();
    IOClass io;
    Path path(io.sites, io.days, start_time, io.lines);
    path.set_up_mst();
    print(path.mst[2]);
    // vector<vector<int> > ans = path.return_mst_path();
    // print(ans);
    time_elapsed(false, true);
    return 0;
}