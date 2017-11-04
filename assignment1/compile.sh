#!/bin/sh
gcc -o parser parser.c
g++ -std=c++11 -o indexer indexer.cpp
g++ -std=c++11 -o search search.cpp