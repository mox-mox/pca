#
set term postscript enh color
set output "benchmarks.ps"


set multiplot layout 1,2 rowsfirst



set label 2 "Using single precision math" at graph 0.1,0.5 rotate by 90
set xlabel "Matrix Dimension (float)"
set ylabel "Execution Time [CPU ticks]"
plot "benchmark_data/benchmark_results_float.dat" using 1:2:xtic(1) with linespoints title "1 Thread ", \
     "benchmark_data/benchmark_results_float.dat" using 1:3:xtic(1) with linespoints title "2 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:4:xtic(1) with linespoints title "4 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:5:xtic(1) with linespoints title "8 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:6:xtic(1) with linespoints title "16 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:7:xtic(1) with linespoints title "32 Threads"


set label 1 "Using double precision math" at graph 0.1,0.5 rotate by 90
set xlabel "Matrix Dimension (double)"
set ylabel "Execution Time [CPU ticks]"
plot "benchmark_data/benchmark_results_double.dat" using 1:2:xtic(1) with linespoints title "1 Thread ", \
     "benchmark_data/benchmark_results_double.dat" using 1:3:xtic(1) with linespoints title "2 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:4:xtic(1) with linespoints title "4 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:5:xtic(1) with linespoints title "8 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:6:xtic(1) with linespoints title "16 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:7:xtic(1) with linespoints title "32 Threads"


