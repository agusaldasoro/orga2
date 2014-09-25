#!/bin/bash

RANGE=10 # 100

filtros="cropflip sierpinski bandas mblur"

TP=../bin/tp2
FILTRO=$1
IMPL=$2
IMAGEN=$3
ITERACIONES=$4
shift 4
PARAMS=$@ 

rm mediciones.txt
#rm mediciones.cropflip
#rm mediciones.sierpinski
#rm mediciones.bandas
#rm mediciones.mblur

for FILTRO in cropflip sierpinski bandas mblur
do
#echo $FILTRO
for i in $(seq 1 $RANGE);
do
echo "$FILTRO ($i de $RANGE)" >> mediciones.txt
echo $TP $FILTRO -i $IMPL -t $ITERACIONES $IMAGEN $PARAMS;
$TP $FILTRO -i $IMPL -t $ITERACIONES $IMAGEN $PARAMS;
done
done
