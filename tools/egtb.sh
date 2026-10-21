#!/bin/bash
# Regenerates the tablebases in tables/. Run from the repository root.
cd tables || exit 1
rm *.bit
../egtb 2 2
../egtb 3 2
../egtb 3 3
../egtb 4 2
../egtb 4 3
../egtb 4 4
../egtb 5 2
../egtb 5 3
../egtb 5 4
../egtb 5 5
