# [Question](https://stackoverflow.com/questions/48604474/compare-first-two-columns-of-two-csv-files)
I have two csv files, 1a.csv and 2a.csv

1a.csv looks like this:
```csv
SKU, QTY
KA005-001,17
KA006-001,46
KA010-001,25
KA014-001,42`
```
2a.csv looks like this:

```csv
SKU, QTY
KA006-001,81
KA009-001,25
KA010-001,18
KA014-001,45`
```
I need to get a new file like this 3a.csv:
```csv
SKU, QTY
KA005-001,17 
KA006-001,81 (i.e. 2a.csv QTY)
KA009-001,25
KA010-001,18 (i.e. 2a.csv QTY)
KA014-001,45 (i.e. 2a.csv QTY)`
```
Could someone help me with this problem as well ? Thank you very much.

# [Answer](https://stackoverflow.com/a/48609420/9210255)

For Bash 4:
```bash
#!/bin/bash

declare -A astr

getDataFromFile(){
	while IFS=, read -r col1 col2
	do
		if [ "$col1" != "SKU" ] && [ "$col1" != "" ]
		then
			(( astr[$col1] = col2 ))
		fi
	done < $1 
}

getDataFromFile 1a.csv
getDataFromFile 2a.csv

echo "SKU,QTY"
for i in "${!astr[@]}"
do   
	echo "$i,${astr[$i]}"
done | sort -t : -k 2n

```
or you can use awk this way
```bash
awk -F, 'BEGIN{print "SKU,QTY"}NR>1{a[$1] = $2}END{for (i in a) if(i != "SKU")print i","a[i]}' 1a.csv 2a.csv
```
or Python:
```python
#!/usr/bin/python

a={}
def getDataFromFile(fname):
    f = open(fname)
    for line in f:
       fields = line.strip().split(",")
       a[fields[0]] = fields[1]
							
getDataFromFile('1a.csv')
getDataFromFile('2a.csv')

for key, value in a.iteritems():
    print key+","+value 
```
