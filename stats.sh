#!/bin/bash
rm log-*.txt
time nottt 1 3 3 100 log-1-3-3.txt | tail -n 9
time nottt 2 3 3 100 log-2-3-3.txt | tail -n 9
time nottt 3 3 3 100 log-3-3-3.txt | tail -n 9
time nottt 1 4 3 100 log-1-4-3.txt | tail -n 9
time nottt 2 4 3 100 log-2-4-3.txt | tail -n 9
time nottt 3 4 3 100 log-3-4-3.txt | tail -n 9
time nottt 1 5 3 100 log-1-5-3.txt | tail -n 9
time nottt 2 5 3 100 log-2-5-3.txt | tail -n 9
time nottt 3 5 3 100 log-3-5-3.txt | tail -n 9
time nottt 1 6 3 100 log-1-6-3.txt | tail -n 9
time nottt 2 6 3 100 log-2-6-3.txt | tail -n 9
time nottt 3 6 3 100 log-3-6-3.txt | tail -n 9
