#!/usr/bin/env bash

set -euo pipefail

workspace_dir="$(cd "$(dirname "${BASH_SOURCE[0]}")" >/dev/null 2>&1 && pwd)"
valgrind_cmd=valgrind


bins=(
  build/test/test_cellule
  build/test/test_particule
  build/test/test_performance
  build/test/test_univers
  build/test/test_univers_1D
  build/test/test_univers_tp4
  build/test/test_vector
)

status=0
for bin in "${bins[@]}"; do
  full_bin="$workspace_dir/$bin"
  echo
  echo "=== Running '$bin' ==="
  log="/tmp/valgrind-$(basename "$bin").log"

  if ! "$valgrind_cmd" \
    --leak-check=full \
    --show-leak-kinds=all \
    --errors-for-leak-kinds=definite,indirect \
    --error-exitcode=1 \
    --log-file="$log" \
    "$full_bin"; then
    echo "[ERROR] '$bin' returned non-zero status."
    status=1
  fi

  echo "--- Summary for '$bin' ---"
  grep -E 'definitely lost:|indirectly lost:|possibly lost:|still reachable:|suppressed:|ERROR SUMMARY:' "$log" || true

done

exit "$status"
