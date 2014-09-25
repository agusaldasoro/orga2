#!/bin/bash

RANGE=10 # 100

TP=../bin/tp2
FILTRO=$1
IMPL=$2
IMAGEN=$3
ITERACIONES=$4
shift 4
PARAMS=$@ 

rm mediciones.txt

for i in $(seq 1 $RANGE);
do
echo "$FILTRO ($i de $RANGE)" >> mediciones.txt
$TP $FILTRO -i $IMPL -t $ITERACIONES $IMAGEN $PARAMS;
done
