#!/bin/bash

rm -vf cpp_client.out
g++ -o cpp_client.out client.cpp -O3 -g
