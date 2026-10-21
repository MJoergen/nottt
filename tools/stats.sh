#!/bin/bash
# Runs a series of tournaments and prints the final results table of each.
cd "$(dirname "$0")/.." || exit 1

# Keep only the last results table, whatever the number of players.
results() { tac | sed '/^Results from round/q' | tac; }

rm -f log-*.txt
for size in 3 4 5 6; do
    for count in 1 2 3; do
        time ./nottt $count $size 3 100 log-$count-$size-3.txt | results
    done
done
