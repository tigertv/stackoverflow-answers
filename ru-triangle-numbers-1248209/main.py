#!/usr/bin/python3

from math import sqrt

def get_xy(i):
	d = int(sqrt(1 + (i - 1) * 8 ) + 1) // 2 - 1
	r = i - d * (d + 1) // 2 - 1
	return (r, d - r) if d & 1 else (d - r, r)

i = 44444444447777777777777
p = get_xy(i)
print(i, p)

for i in range(1, 30):
	p = get_xy(i)
	print(i, p, end=", ")
	
