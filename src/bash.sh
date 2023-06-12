#!/bin/bash
sudo modprobe msr

declare -a ALG=("bubbleSort" "selectionSort" "insertionSort" "radixSort")
declare -a N=("10000" "20000" "30000")
declare REP=10

#create result file
touch final.csv

#for each input size execute all algorithms
for a in ${N[@]}; do
  for r in ${ALG[@]}; do
    chmod ug+x exe/$r
    sudo ./main "exe/$r $a" $REP 
    chmod 755 exe/$r.py
    sudo ./main "exe/$r.py $a" $REP 
  done
done

#concatenate the results to the result file
cat exe/*.J >> final.csv

#delete all duplicated lines and auxiliary files
sort -u -r final.csv -o final.csv
rm -rf exe/*.J

#move result file to the "resultados" directory
mv final.csv resultados/
