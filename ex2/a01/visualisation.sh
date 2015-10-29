#! /usr/bin/env bash


echo Starting visualisation for BOX_SIZE $1 with $2 frames.
./relaxation -g

if [ "$?" -gt "$1" ];
then
	make clean
	make relaxation BOX_SIZE=$1
fi


echo Cleaning old image files:
rm ./data/iteration_*.jpeg
rm ./data/iteration_*.dat
echo "			... done"

echo Generating the raw data. This might take a moment or two:
./relaxation -e -s $2
echo "			... done"

width=$2
width=${#width}

for i in $(seq -f "%0"$width"g" 0 $2)
do
	echo Generating image $i:
	gnuplot -e "in='$i'; out='$i'" visualise.plt
echo "			... done"
done


echo Generating final GIF file:
convert -delay 30 ./data/*.jpeg -loop 0 animation.gif
echo "			... done"
