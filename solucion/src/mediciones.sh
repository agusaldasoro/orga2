#!/bin/bash

range=10 # 100

TP=../bin/tp2
FILTRO=$1
IMPL=$2
IMAGEN=$3
shift 3 
PARAMS=$@ 
echo $PARAMS
for i in $(seq 1 $range);
do
$TP $FILTRO -i $IMPL -t 10 $IMAGEN $PARAMS;
done
