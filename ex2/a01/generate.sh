#! /usr/bin/env bash

# Dennis: So verstehe ich "Messen Sie die Zeit, die sie für eine Iteration durschnittlich benötigen, wenn sie ein Feld der Grös-
# sen 100x100, 500x500, 1000x1000 berechnen (waehlen Sie eine sinnvolle Anzahl an Iteratio-
# nen). Sammeln Sie die Datenwerte das Feldes zu verschiedenen Zeitpunkten und visualisieren Sie
# diese (z.B. als Bild). Stellen Sie die Ergebnisse graphisch dar und interpretieren Sie diese. Expe-
# rimentieren sie auch mit Compiler Parametern. Testen Sie Ihr Programm!" Das mein Code läuft, prüf ich, wenn ich ihn schreib...


echo "Crating the Animation."

make clean
make all # create the animation
make clean



echo "Creating the timing results"



make relaxation BOX_SIZE=100 STEPS=200 OPT=0
echo "BOX_SIZE=100, STEPs=200, -O0"
echo "BOX_SIZE=100, STEPs=200, -O0" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=500 STEPS=200 OPT=0
echo "BOX_SIZE=500, STEPs=200, -O0"
echo "BOX_SIZE=500, STEPs=200, -O0" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=1000 STEPS=200 OPT=0
echo "BOX_SIZE=1000, STEPs=200, -O0"
echo "BOX_SIZE=1000, STEPs=200, -O0" >> timing_data
./relaxation -t >> timing_data
make clean




make relaxation BOX_SIZE=100 STEPS=200 OPT=1
echo "BOX_SIZE=100, STEPs=200, -O1"
echo "BOX_SIZE=100, STEPs=200, -O1" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=500 STEPS=200 OPT=1
echo "BOX_SIZE=500, STEPs=200, -O1"
echo "BOX_SIZE=500, STEPs=200, -O1" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=1000 STEPS=200 OPT=1
echo "BOX_SIZE=1000, STEPs=200, -O1"
echo "BOX_SIZE=1000, STEPs=200, -O1" >> timing_data
./relaxation -t >> timing_data
make clean




make relaxation BOX_SIZE=100 STEPS=200 OPT=2
echo "BOX_SIZE=100, STEPs=200, -O2"
echo "BOX_SIZE=100, STEPs=200, -O2" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=500 STEPS=200 OPT=2
echo "BOX_SIZE=500, STEPs=200, -O2"
echo "BOX_SIZE=500, STEPs=200, -O2" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=1000 STEPS=200 OPT=2
echo "BOX_SIZE=1000, STEPs=200, -O2"
echo "BOX_SIZE=1000, STEPs=200, -O2" >> timing_data
./relaxation -t >> timing_data
make clean



make relaxation BOX_SIZE=100 STEPS=200 OPT=3
echo "BOX_SIZE=100, STEPs=200, -O3"
echo "BOX_SIZE=100, STEPs=200, -O3" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=500 STEPS=200 OPT=3
echo "BOX_SIZE=500, STEPs=200, -O3"
echo "BOX_SIZE=500, STEPs=200, -O3" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=1000 STEPS=200 OPT=3
echo "BOX_SIZE=1000, STEPs=200, -O3"
echo "BOX_SIZE=1000, STEPs=200, -O3" >> timing_data
./relaxation -t >> timing_data
make clean



make relaxation BOX_SIZE=100 STEPS=200 OPT=s
echo "BOX_SIZE=100, STEPs=200, -Os"
echo "BOX_SIZE=100, STEPs=200, -Os" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=500 STEPS=200 OPT=s
echo "BOX_SIZE=500, STEPs=200, -Os"
echo "BOX_SIZE=500, STEPs=200, -Os" >> timing_data
./relaxation -t >> timing_data
make clean

make relaxation BOX_SIZE=1000 STEPS=200 OPT=s
echo "BOX_SIZE=1000, STEPs=200, -Os"
echo "BOX_SIZE=1000, STEPs=200, -Os" >> timing_data
./relaxation -t >> timing_data
make clean
