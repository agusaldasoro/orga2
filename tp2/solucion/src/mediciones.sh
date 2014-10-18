#!/bin/bash

RANGE=1 # 100

filtros="cropflip sierpinski bandas mblur"

TP=./tp2
#FILTRO=$1
IMPL=$1
IMAGEN=$2
shift 2
PARAMS=$@ 

rm xsarasa
rm mediciones
#rm mediciones.cropflip
#rm mediciones.sierpinski
#rm mediciones.bandas
#rm mediciones.mblur

for FILTRO in cropflip sierpinski bandas mblur
do
echo "$FILTRO" >> mediciones 
for i in $(seq 1 $RANGE);
do
#echo "$FILTRO ($i de $RANGE)" >> mediciones
echo "$TP $FILTRO -i $IMPL -t 10 $IMAGEN $PARAMS" >> xsarasa;
$TP $FILTRO -i $IMPL -t 10 $IMAGEN $PARAMS;
echo "$TP $FILTRO -i $IMPL -t 100 $IMAGEN $PARAMS" >> xsarasa;
$TP $FILTRO -i $IMPL -t 100 $IMAGEN $PARAMS;
echo "$TP $FILTRO -i $IMPL -t 200 $IMAGEN $PARAMS" >> xsarasa;
$TP $FILTRO -i $IMPL -t 200 $IMAGEN $PARAMS;
echo "$TP $FILTRO -i $IMPL -t 1000 $IMAGEN $PARAMS" >> xsarasa;
$TP $FILTRO -i $IMPL -t 1000 $IMAGEN $PARAMS;
done
done
