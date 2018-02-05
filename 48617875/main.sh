#!/bin/bash

awk -F: '$3!=p{print ""}{p=$3}{print}' sys.log
