#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

#
#	Solved Problems in Geostatistics
#
# ------------------------------------------------
#	Script for lesson 8.3
#	"Indicator Simulation for Categorical Data"
# ------------------------------------------------

from numpy import *
from geo import *
from matplotlib import *
from pylab import *
from scipy import *

#---------------------------------------------------
#	Problem:
#
#	Drawing 100 realizations from the cdf yields 100 realizations of the rock type category. A distribution of uncertainty constructed for these realizations should look similar to the distributions of he conditional pdf.
#
# ----------------------------------------------------

# rock type
rock_type = array([2, 2, 3, 3, 1])

# number of realizations
n = 100

array_hist = zeros( (n), dtype = int)

for i in xrange(n):
	index = random.randint(0, len(rock_type), 1)
	array_hist[i] = rock_type[index]

prob = []
value = []

for i in range(min(rock_type), max(rock_type)+1):
	val = array_hist.compress((array_hist == i).flat)
	prob.append((float(len(val)) / (len(array_hist))))
	value.append(i)

# Histogram of the conditional pdf
figure()
for i in range(3):
	bar(value[i], prob[i], width = 0.33)
title("pdf")
xlabel("Rock Type")
ylabel("f")

# Histogram of the cdf derived from indicator kriging of the rock type data
figure()
hist(array_hist, cumulative = True, normed = True)
title("cdf")
xlabel("Rock Type")
ylabel("F")

show()
