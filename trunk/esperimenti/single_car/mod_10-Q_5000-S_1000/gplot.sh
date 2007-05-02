#!/bin/bash
#Gplot for videotracker by lk :D

gnuplot -persist plot-png > plot.png
gnuplot -persist plot-window 
gnuplot -persist plot-distances
gnuplot -persist plot-distances-png > plot-distances.png