#!/usr/bin/python
import os, sys

os.system ("awk -F, 'BEGIN{print \"SKU,QTY\"}NR>1{a[$1] = $2}END{for (i in a) if(i != \"SKU\") print i\",\"a[i]}' 1a.csv 2a.csv")
