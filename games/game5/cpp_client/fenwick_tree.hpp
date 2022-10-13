#ifndef _FENWICK_TREE_HPP_
#define _FENWICK_TREE_HPP_
#include "gambler_helper.hpp"

void update_tree(vector<double> &tree, int index, double val){
    int n = tree.size();
    index++;
    
    while(index <= n){
        tree[index] += val;
        index += index & (-index);
    }
}

void construct_fenwick_tree(vector<double> &frequencies, vector<double> &fenwick_tree){
    int n = frequencies.size();

    for (int i = 0; i < n; ++i)
        update_tree(fenwick_tree, i, frequencies[i]);
}

double get_sum(const vector<double> &tree, int index){
    double sum = 0;
    index++;

    while(index > 0){
        sum += tree[index];
        index -= index & (-index);
    }
    return sum;
}


int lower_bound_on_fenwick_tree(const vector<double> &tree, int low, int high, double value){
    if (low > high) return low;
    
    int mid = low + (high - low) / 2;
    double mid_val = get_sum(tree, mid);

    if (mid_val >= value)
        return lower_bound_on_fenwick_tree(tree, low, mid - 1, value);
 
    return lower_bound_on_fenwick_tree(tree, mid + 1, high, value);
}

int binary_search_on_fenwick_tree(const vector<double> &tree, int left, int right, double value){
    if (right >= left) {
        int mid = left + (right - left) / 2;

        double mid_val = get_sum(tree, mid);
        if (mid_val == value) return mid;
        
        if (mid_val > value)
            return binary_search_on_fenwick_tree(tree, left, mid - 1, value);
        
        return binary_search_on_fenwick_tree(tree, mid + 1, right, value);
    }
    return -1;
}

#endif