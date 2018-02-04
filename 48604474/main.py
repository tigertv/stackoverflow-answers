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
