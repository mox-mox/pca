#
#set term postscript enh color
set terminal postscript eps enhanced color
set output "benchmarks.ps"

set macros
set format y "%1.0tx10^{%T}";
# Margins for each row resp. column
TMARGIN = "set tmargin at screen 0.90; set bmargin at screen 0.57"
BMARGIN = "set tmargin at screen 0.53; set bmargin at screen 0.20"
LMARGIN = "set lmargin at screen 0.15; set rmargin at screen 0.54"
RMARGIN = "set lmargin at screen 0.56; set rmargin at screen 0.95"



set logscale xy

set multiplot layout 2,2 rowsfirst title "Execution Times For The Matrix Multiplication"

@TMARGIN; @LMARGIN
set key left top
set title "Using single precission math"
unset xlabel
set xtics ( '' 100, '' 500, '' 1000, '' 10000 );
set ylabel "Execution Time [CPU ticks]"
plot "benchmark_data/benchmark_results_float.dat" using 1:2 with linespoints title "1 Thread ", \
     "benchmark_data/benchmark_results_float.dat" using 1:3 with linespoints title "2 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:4 with linespoints title "4 Threads", \
     "benchmark_data/benchmark_results_float.dat" using 1:5 with linespoints title "", \
     "benchmark_data/benchmark_results_float.dat" using 1:6 with linespoints title "", \
     "benchmark_data/benchmark_results_float.dat" using 1:7 with linespoints title ""
unset ylabel

@TMARGIN; @RMARGIN
set title "Using double precission math"
set xtics ( '' 100, '' 500, '' 1000, '' 10000 )
set format y ''
plot "benchmark_data/benchmark_results_double.dat" using 1:2 with linespoints title "", \
     "benchmark_data/benchmark_results_double.dat" using 1:3 with linespoints title "", \
     "benchmark_data/benchmark_results_double.dat" using 1:4 with linespoints title "", \
     "benchmark_data/benchmark_results_double.dat" using 1:5 with linespoints title "8 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:6 with linespoints title "16 Threads", \
     "benchmark_data/benchmark_results_double.dat" using 1:7 with linespoints title "32 Threads"



set nokey
unset logscale y

@BMARGIN; @LMARGIN
unset title
set xlabel "Matrix Dimension [floats]"
set ylabel "Speedup [T_s/T_p]"
set format y "%2.0f";
set ytics ( 0,1,2,3,4,5,6,7,8,9,10)
set yrange [0:10]
plot "benchmark_data/benchmark_results_float.dat" using ($1):($2/$2):xtic(1) with linespoints title "1 Thread ", \
     "benchmark_data/benchmark_results_float.dat" using ($1):($2/$3) with linespoints title "2 Threads", \
     "benchmark_data/benchmark_results_float.dat" using ($1):($2/$4) with linespoints title "4 Threads", \
     "benchmark_data/benchmark_results_float.dat" using ($1):($2/$5) with linespoints title "8 Threads", \
     "benchmark_data/benchmark_results_float.dat" using ($1):($2/$6) with linespoints title "16 Threads", \
     "benchmark_data/benchmark_results_float.dat" using ($1):($2/$7) with linespoints title "32 Threads"
unset ylabel


@BMARGIN; @RMARGIN
set xlabel "Matrix Dimension [doubles]"
set format y "";
set ytics ( 0,1,2,3,4,5,6,7,8,9,10)
set yrange [0:10]
plot "benchmark_data/benchmark_results_double.dat" using ($1):($2/$2):xtic(1) with linespoints title "1 Thread ", \
     "benchmark_data/benchmark_results_double.dat" using ($1):($2/$3) with linespoints title "2 Threads", \
     "benchmark_data/benchmark_results_double.dat" using ($1):($2/$4) with linespoints title "4 Threads", \
     "benchmark_data/benchmark_results_double.dat" using ($1):($2/$5) with linespoints title "8 Threads", \
     "benchmark_data/benchmark_results_double.dat" using ($1):($2/$6) with linespoints title "16 Threads", \
     "benchmark_data/benchmark_results_double.dat" using ($1):($2/$7) with linespoints title "32 Threads"
