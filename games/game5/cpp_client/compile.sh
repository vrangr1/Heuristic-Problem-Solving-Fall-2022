#!/bin/bash

rm -vf cpp_client.out
rm -rvf cpp_client.out.dSYM
g++ -o cpp_client.out client.cpp -O3 -g
