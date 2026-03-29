#!/bin/sh
# Run all Unity test binaries passed as arguments and print colored results.

GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
RESET='\033[0m'

failed=0

printf "${YELLOW}========== Running Unity tests ==========${RESET}\n"

for bin in "$@"; do
    printf "${YELLOW}--- %s ---${RESET}\n" "$bin"
    output=$("$bin" 2>&1)
    status=$?
    if [ $status -ne 0 ]; then
        printf "${RED}%s${RESET}\n" "$output"
        failed=$((failed + 1))
    else
        printf "${GREEN}%s${RESET}\n" "$output"
    fi
done

printf "${YELLOW}=========================================${RESET}\n"

if [ $failed -ne 0 ]; then
    printf "${RED}FAIL: %d test binary(ies) reported failures.${RESET}\n" "$failed"
    exit 1
else
    printf "${GREEN}All tests passed.${RESET}\n"
fi