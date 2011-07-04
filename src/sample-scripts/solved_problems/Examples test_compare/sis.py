from geo import *
from geo.routines import *
from matplotlib import *
from sys import *
from pylab import *
import os
import time
import pylab

size = (166, 141, 1)
grid = SugarboxGrid(166, 141, 1)

data = load_ind_property("IND_data.INC", -99, [0,1], size)

cov1 = CovarianceModel(type = 1, ranges = (20, 20, 1), sill = 1) 

ik_data =  [{
			"cov_model": cov1,
            "radiuses": (40, 40, 1),
            "max_neighbours": 12,
            },
            {
			"cov_model": cov1,
            "radiuses": (40, 40, 1),
            "max_neighbours": 12,
            }]

sis_result = sis_simulation(prop = data, grid = grid, data = ik_data, marginal_probs = (0.8, 0.2), seed = 3241347, force_single_thread = True)

figure()
pylab.imshow(data[0][:,:,0], vmin = 0, vmax = 2)
pylab.savefig("hard_data")

figure()
hist(data[0].compress((data[0]!=-99).flat), bins = 20)
title("Historgam of Harddata")

figure()
pylab.imshow(sis_result[0][:,:,0], vmin = 0, vmax = 2)
pylab.savefig("SIS_result")

figure()
hist(sis_result[0].compress((sis_result[0]!=-99).flat), bins = 20)
title("Historgam of SIS Result")
show()