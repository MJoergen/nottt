#!/bin/bash
# Plays tournaments between the bots and compares the results, and every move
# of every game, with the reference games in tests/bots/. The games are
# deterministic (fixed seeds), so any difference is a change in behaviour.
# With --update, the reference games are replaced instead.
# Run from the repository root.
set -e

# boardCount boardSize lineSize, chosen to cover every board size, every line
# size up to 5, and 1 to 3 boards. Line size 3 is the reference.
configs=(
    "1 2 2" "1 3 3" "1 4 3" "1 5 3" "1 5 5" "1 6 3"
    "2 3 2" "2 4 4" "2 5 3" "2 6 2"
    "3 4 2" "3 5 3" "3 5 4" "3 6 4"
)
rounds=2

update=false
if [ "$1" = "--update" ]; then
    update=true
elif [ $# -gt 0 ]; then
    echo "Usage: $0 [--update]"
    exit 1
fi

tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT

if $update; then
    mkdir -p tests/bots
    rm -f tests/bots/*.txt
fi

failed=false
for config in "${configs[@]}"; do
    read -r boardCount boardSize lineSize <<< "$config"
    name="$boardCount-$boardSize-$lineSize"
    ./nottt "$boardCount" "$boardSize" "$lineSize" "$rounds" "$tmp/log" > "$tmp/out"

    # The tournament results, followed by one line per game with the players,
    # the moves and the winner. The rest of the log shows the search, which
    # may change without changing the games.
    {
        cat "$tmp/out"
        echo
        echo "Games:"
        awk '
            /^Playing game between / {
                sub(/^Playing game between /, ""); sub(/ with seed .*/, ""); game = $0 ":"
            }
            / played: / { sub(/.* played: /, ""); game = game " " $0 }
            /^Player .* won!$/ { sub(/^Player /, ""); sub(/ won!$/, ""); print game " -> " $0 }
        ' "$tmp/log"
    } > "$tmp/$name.txt"

    if $update; then
        cp "$tmp/$name.txt" "tests/bots/$name.txt"
    elif ! diff -u "tests/bots/$name.txt" "$tmp/$name.txt"; then
        failed=true
    fi
done

if $failed; then
    echo
    echo "The bots played differently from the games in tests/bots/."
    echo "If the change is intended, run 'make update-bots' and commit tests/bots/."
    exit 1
fi
if $update; then
    echo "Updated the games in tests/bots/."
else
    echo "The bots played the same games as in tests/bots/."
fi
