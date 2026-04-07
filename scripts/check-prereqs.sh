#!/bin/sh
# Run on the Pi via: ssh pi "bash -s" < scripts/check-prereqs.sh

GREEN='\033[0;32m'
RED='\033[0;31m'
RESET='\033[0m'

passed=0
failed=0

check_tool()
{
    tool="$1"
    if command -v "$tool" > /dev/null 2>&1; then
        version=$(command "$tool" --version 2>&1 | head -n1)
        printf "${GREEN}✅ %-20s${RESET} %s\n" "$tool" "$version"
        passed=$((passed + 1))
    else
        printf "${RED}❌ %-20s not found${RESET}\n" "$tool"
        failed=$((failed + 1))
    fi
}

check_pico_sdk()
{
    if [ -n "$PICO_SDK_PATH" ] && [ -d "$PICO_SDK_PATH" ]; then
        printf "${GREEN}✅ %-20s${RESET} %s\n" "PICO_SDK_PATH" "$PICO_SDK_PATH"
        passed=$((passed + 1))
    else
        printf "${RED}❌ %-20s not set or missing${RESET}\n" "PICO_SDK_PATH"
        failed=$((failed + 1))
    fi
}

print_summary()
{
    printf "\n%d passed, %d failed\n" "$passed" "$failed"
    [ "$failed" -eq 0 ]
}

main()
{
    printf "=== Prerequisite check on %s ===\n\n" "$(hostname)"
    check_tool cmake
    check_tool arm-none-eabi-gcc
    check_tool ninja
    check_tool picotool
    check_tool openocd
    check_pico_sdk
    print_summary
}

main
