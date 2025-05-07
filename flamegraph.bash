#!bin/bash
/usr/lib/linux-tools/6.8.0-58-generic/perf record -g  -- ./main
/usr/lib/linux-tools/6.8.0-58-generic/perf script > out.perf
~/FlameGraph/stackcollapse-perf.pl out.perf > out.folded
~/FlameGraph/flamegraph.pl out.folded > perf.svg