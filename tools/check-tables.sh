#!/bin/bash
# Regenerates every tablebase in tables/ in a temporary directory, and checks
# that it is identical to the tracked one. Run from the repository root.
set -e

root=$PWD
tmp=$(mktemp -d)
trap 'rm -rf "$tmp"' EXIT
cd "$tmp"

failed=false
for table in "$root"/tables/egtb-*.bit; do
    name=$(basename "$table")
    sizes=${name#egtb-}
    sizes=${sizes%.bit}
    "$root/egtb" "${sizes%-*}" "${sizes#*-}" > /dev/null
    if cmp "$name" "$table"; then
        echo "$name is identical."
    else
        failed=true
    fi
done

if $failed; then
    echo "egtb no longer generates the tables in tables/."
    exit 1
fi
