#!/bin/bash

# Compile the program
gcc -Wall -Wextra -Werror powerset.c -o powerset || { echo "❌ Compilation failed"; exit 1; }

# Colors for output
GREEN="\033[0;32m"
RED="\033[0;31m"
NC="\033[0m" # No Color

# Temporary file to hold output
TMP_OUT="out.txt"

# Define test cases: Each entry is "input|expected output (unordered)"
TESTS=(
  "3 1 0 2 4 5 3|"
  "12 5 2 1 8 4 3 7 11|"
  "7 3 8 2"
  "0 1 -1|"
  "5 1 2 3 4 5|"
)

echo "Running powerset tests..."

ALL_PASS=1

i=1
for test in "${TESTS[@]}"; do
    INPUT="${test%%|*}"
    EXPECTED="${test#*|}"

    # Run program
    ./powerset $INPUT | sort > "$TMP_OUT"

    # Convert expected output to sorted form
    EXPECTED_SORTED=$(echo -e "$EXPECTED")

    OUTPUT=$(cat "$TMP_OUT")

    if [ "$OUTPUT" == "$EXPECTED_SORTED" ]; then
        echo -e "✅ Test $i: ${GREEN}PASSED${NC}"
    else
        echo -e "❌ Test $i: ${RED}FAILED${NC}"
        echo "Input: ./powerset $INPUT"
        echo "Expected (sorted):"
        echo "$EXPECTED_SORTED"
        echo "Got:"
        echo "$OUTPUT"
        ALL_PASS=0
    fi
    ((i++))
done

rm -f "$TMP_OUT"

if [ $ALL_PASS -eq 1 ]; then
    echo -e "${GREEN}All tests passed! ✅${NC}"
else
    echo -e "${RED}Some tests failed ❌${NC}"
fi
