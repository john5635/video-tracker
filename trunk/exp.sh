#!/bin/bash
#lanciare lo script con i seguenti argomenti: 
# ./exp.sh nomevideo numeromodulo numeroQ numeroSample
if [ "$1" = "" ]; then
		echo "Devi inserire correttamente i parametri: "
		echo "./exp.sh nomevideo numeromodulo numeroQ numeroSample"
		exit
fi
if [ "$2" = "" ]; then
		echo "Devi inserire correttamente i parametri: "
		echo "./exp.sh nomevideo numeromodulo numeroQ numeroSample"
		exit
fi
if [ "$3" = "" ]; then
		echo "Devi inserire correttamente i parametri: "
		echo "./exp.sh nomevideo numeromodulo numeroQ numeroSample"
		exit
fi
if [ "$4" = "" ]; then
		echo "Devi inserire correttamente i parametri: "
		echo "./exp.sh nomevideo numeromodulo numeroQ numeroSample"
		exit
fi
mkdir -p esperimenti/$1/"mod_"$2-"Q_"$3-"S_"$4
cp coordinateReali.txt coordinateCondensation.txt coordinateKalman.txt distanzaKalman.txt distanzaCondensation.txt Risultati.txt plot-distances-png plot-distances plot-png plot-window gplot.sh esperimenti/$1/"mod_"$2-"Q_"$3-"S_"$4
cd esperimenti/$1/"mod_"$2-"Q_"$3-"S_"$4
gnuplot -persist plot-png > plot.png
gnuplot -persist plot-distances-png > plot-distances.png
