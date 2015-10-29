# Received from Max Menges
set terminal jpeg
set output sprintf("./data/iteration_%s.jpeg",out)
set size ratio 0.71
set view map
set cbrange [0:127]
set title sprintf("State: %s",in)
unset key

splot sprintf("./data/iteration_%s.dat",in) using 1:2:3 with points pt 5 ps 1 palette 
