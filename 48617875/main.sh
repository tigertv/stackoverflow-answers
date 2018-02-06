#!/bin/bash

Year='$2'
Month='$2$3'
Day='$2$3$4'
Hour='$2$3$4$5'
Minute='$2$3$4$5$6'
Second='$2$3$4$5$6$7'

filter=$Minute

awk -F'[ :-]' $filter'!=p{print ""}{p='$filter'}{print}' sys.log
