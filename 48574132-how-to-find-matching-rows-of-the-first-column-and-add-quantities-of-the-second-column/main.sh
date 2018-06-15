#!/bin/bash

declare -A astr

while IFS=, read -r col1 col2
do
	if [ "$col1" != "SKU" ] && [ "$col1" != "" ]
	then
		(( astr[$col1] += col2 ))
	fi
done < 2asg.csv

echo "SKU,QTY"
for i in "${!astr[@]}"
do   
	echo "$i,${astr[$i]}"
done | sort -t : -k 2n
