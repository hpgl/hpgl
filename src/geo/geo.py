#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

import hpgl
from hpgl import create_sugarbox_grid
import numpy

def append_mask(prop, mask):
	infs = prop[1]
	infs &= mask
	infs.choose(-99, prop[0], out=prop[0])

class covariance:
	spherical = 0
	exponential = 1
	gaussian = 2

class SugarboxGrid:
	def __init__(self, x, y, z):
		self.grid = create_sugarbox_grid(x, y, z)

class CovarianceModel:	
	def __init__(self, type = 0, ranges=(0,0,0), angles=(0.0,0.0,0.0), sill=1.0, nugget=0.0):
		self.type = type
		self.ranges = ranges
		self.angles = angles
		self.sill = sill
		self.nugget = nugget

		if nugget > sill:
			raise Exception("Nugget should be less or equal to Sill")

	

def _load_prop_cont_slow(filename, undefined_value):
	values = []
	mask = []
	f = open(filename)
	for line in f:
		if line.strip().startswith("--"):
			continue
		for part in line.split():
			try:
				val = float(part.strip())
				values.append(val)
				if (val == undefined_value):
					mask.append(0)
				else:
					mask.append(1)
			except:
				pass

	return (numpy.array(values, dtype="float32"), numpy.array(mask, dtype="uint8"))



def _load_prop_ind_slow(filename, undefined_value, ind_values):
	dict_map = {}	
	for i in xrange(len(ind_values)):
		dict_map[ind_values[i]] = i
	
	values = []
	mask = []

	f = open(filename)
	for line in f:
		if line.strip().startswith("--"):
			continue
		for part in line.split():
			try:
				val = int(part.strip())
				if (val == undefined_value):
					values.append(255)
					mask.append(0)
				else:
					values.append(dict_map[val])
					mask.append(1)
			except:
				pass

	return (numpy.array(values, dtype="uint8", order='F'), numpy.array(mask, dtype="uint8", order='F'), len(ind_values))

def _create_cont_prop(size):
	return numpy.zeros(size, dtype="float32"), numpy.zeros(size, dtype='uint8')

def _create_ind_prop(size, indicator_count):
	return numpy.zeros(size, dtype="uint8"), numpy.zeros(size, dtype='uint8'), indicator_count

def _empty_clone(prop):
	data2 = prop[0].copy('F')
	data2.fill(0)
	mask2 = prop[1].copy('F')
	mask2.fill(0)
	if len(prop) == 3:
		return data2, mask2, prop[2]
	else:
		return data2, mask2

def _clone_prop(prop):
	data2 = prop[0].copy('F')
	mask2 = prop[1].copy('F')
	if len(prop) == 3:
		return data2, mask2, prop[2]
	else:
		return data2, mask2

def _require_cont_data(data):
	return numpy.require(data, dtype='float32', requirements='F')

def _requite_ind_data(data):
	return numpy.require(data, dtype='uint8', requirements='F')
		
def write_property(prop_object, filename, prop_name, undefined_value, indicator_values=[]):
	if prop_object[0].dtype.kind == 'f':
		hpgl.write_cont_property(prop_object, filename, prop_name, undefined_value)
	else:
		hpgl.write_byte_property(prop_object, filename, prop_name, undefined_value, indicator_values)

def load_cont_property(filename, undefined_value, size=None):
	if size is None:			
		print "[WARNING]. load_cont_property: Size is not specified. Using slow and ineficcient method."
		return _load_prop_cont_slow(filename, undefined_value)
	else:
		return read_inc_file_float(filename, undefined_value, size)

def read_inc_file_float(filename, undefined_value, size):
	data = numpy.zeros((size), dtype='float32', order='F')
	mask = numpy.zeros((size), dtype='uint8', order='F')
	hpgl.read_inc_file_float(filename, undefined_value, size[0]*size[1]*size[2], data, mask)
	return (data, mask)

def read_inc_file_byte(filename, undefined_value, size, indicator_values):
	data = numpy.zeros(size, dtype='uint8', order='F')
	mask = numpy.zeros(size, dtype='uint8', order='F')
	hpgl.read_inc_file_byte(filename, undefined_value, size[0]*size[1]*size[2], data, mask, indicator_values)
	return (data, mask, len(indicator_values))

def load_ind_property(filename, undefined_value, indicator_values, size=None):
	if (size is None):
		print "[WARNING]. load_ind_property: Size is not specified. Using slow and ineficcient method."
		return _load_prop_ind_slow(filename, undefined_value, indicator_values)
	else:
		return read_inc_file_byte(filename, undefined_value, size, indicator_values)

def set_thread_num(num):
	hpgl.set_thread_num(num)

def get_thread_num():
	return hpgl.get_thread_num()

def calc_mean(prop):
	l = len(prop[0].flat)
	sum = 0
	count = 0
	for i in xrange(l):
		if prop[1].flat[i] == 1:
			sum += prop[0].flat[i]
			count += 1
	return sum/count

def ordinary_kriging(prop, grid, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)
	param = hpgl.create_ok_params()
	param.set_covariance_type(cov_model.type)
	param.set_sill(cov_model.sill)
	param.set_ranges(*cov_model.ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_nugget(cov_model.nugget)
	param.set_angles(*cov_model.angles)
	hpgl.ordinary_kriging(prop, out_prop, grid.grid, param, True)
	return out_prop

def simple_kriging(prop, grid, radiuses, max_neighbours, cov_model, mean=None):
	out_prop = _clone_prop(prop)
	param = hpgl.create_sk_params()
	param.set_covariance_type(cov_model.type)
	param.set_sill(cov_model.sill)
	param.set_ranges(*cov_model.ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_nugget(cov_model.nugget)
	param.set_angles(*cov_model.angles)
	if (not mean is None):
		param.set_mean(mean)
	hpgl.simple_kriging(prop, out_prop, grid.grid, param)
	return out_prop

def lvm_kriging(prop, grid, mean_data, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)
	param = hpgl.create_ok_params()
	param.set_covariance_type(cov_model.type)
	param.set_sill(cov_model.sill)
	param.set_ranges(*cov_model.ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_nugget(cov_model.nugget)
	param.set_angles(*cov_model.angles)
	hpgl.lvm_kriging(prop, out_prop, grid.grid, param, _require_cont_data(mean_data)) 
	return out_prop

def __prepare_sgs(prop, radiuses, max_neighbours, cov_model, seed, kriging_type="sk", mean=None, use_harddata=True, mask=None, min_neighbours=0):
	if use_harddata:
		out_prop = _clone_prop(prop)
	else:
		out_prop = _empty_clone(prop)
	if not mean is None and not numpy.isscalar(mean):
		mean = _require_cont_data(mean)
	if not mask is None:
		mask = _requite_ind_data(mask)
	param = hpgl.create_sgs_params()
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_covariance_type(cov_model.type)
	param.set_ranges(*cov_model.ranges)
	param.set_sill(cov_model.sill)
	param.set_seed(seed)
	param.set_min_neighbours(min_neighbours)
	if (kriging_type == "ok"):
		param.set_kriging_kind(0)
	else:
		param.set_kriging_kind(1)
	param.set_nugget(cov_model.nugget)
	param.set_angles(*cov_model.angles)
	#param.set_mean_kind("stationary_auto")	PyObject
	if mean is None: 
		param.set_mean_kind("stationary_auto")
	elif not numpy.isscalar(mean) is None:
		param.set_mean_kind("varying")
	else:
		param.set_mean(mean)	
		param.set_mean_kind("stationary")
	return out_prop, mean, mask, param

def sgs_simulation(prop, grid, radiuses, max_neighbours, cov_model, seed, kriging_type="sk", mean=None, use_harddata = True, cdf_data = None, use_regions=False, region_size = None, mask=None, force_single_thread=False, force_parallel=False, min_neighbours = 0, **params):
	"""Performs Sequential Gaussian Simulation

Parameters:
-----------
radiuses : tuple of 3 integers
    Search radiuses by X, Y and Z axes.
max_neighbours: integer
    Maximum number of neighbour points to use.
cov_model: CovarianceModel
    Model of covariance.
seed: integer
    Seed for random number generator.
kriging_type:"sk" or "ok", optional
    Selects either simple or ordinary kriging type. Defaults to Simple Kriging.
mean:None or integer or array_like, optional
    Stationary mean value or varying mean array. None - stationary mean value 
    will be calculated from source data. Default: None
use_harddata: bool, optional
    True - to use source data values for simulation. False - to ignore source data 
    values. Default: True.
cdf_data: None or array_like, optional:
    Data to use for calulating cumulative distribution data. If None use 
    source data instead. Default: None.
mask: None or array_like, optional:
    Array containing 1 in cell that need to be simulated and 0 in cell that aren't. 
    If None simulate all cells. Defualt: None.  
"""
	out_prop, mean, mask, param = __prepare_sgs(
		prop=prop, 
		radiuses=radiuses, 
		max_neighbours=max_neighbours, 
		cov_model=cov_model, 
		seed=seed, 
		kriging_type=kriging_type, 
		mean=mean, 
		use_harddata=use_harddata, 
		mask=mask, 
		min_neighbours=min_neighbours)

	is_single_threaded = force_single_thread or (get_thread_num() == 1 and not force_parallel)

	if mean is None or numpy.isscalar(mean):
		hpgl.sgs_simulation(out_prop, grid.grid, param, cdf_data, mask)
	else:
		hpgl.sgs_lvm_simulation(out_prop, grid.grid, param, mean, cdf_data, mask)

	return out_prop

def median_ik(prop, grid, marginal_probs, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)
	param = hpgl.create_median_ik_params()
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_covariance_type(cov_model.type)
	param.set_ranges(*cov_model.ranges)
	param.set_sill(cov_model.sill)
	param.set_nugget(cov_model.nugget)
	param.set_angles(*cov_model.angles)
	param.set_marginal_probs(*marginal_probs)
	hpgl.median_ik(prop, out_prop, grid.grid, param)
	return out_prop

def indicator_kriging(prop, grid, data, marginal_probs):
	for i in xrange(len(data)):
		data[i]['marginal_prob'] = marginal_probs[i]
	if(len(data) == 2):
		return median_ik(prop, grid, (data[0]["marginal_prob"], 1 - data[0]["marginal_prob"]), data[0]["radiuses"], data[0]["max_neighbours"],
			data[0]['cov_model'])	
	out_prop = _clone_prop(prop)
	hpgl.indicator_kriging(prop, out_prop, grid.grid, data)
	return out_prop

def __prepare_sis(prop, data, marginal_probs, mask, use_harddata):
	is_lvm = not numpy.isscalar(marginal_probs[0])
	if use_harddata:
		out_prop = _clone_prop(prop)
	else:
		out_prop = _empty_clone(prop)

	if is_lvm:
		marginal_probs = [_require_cont_data(m) for m in marginal_probs]
	for i in xrange(len(data)):
		if is_lvm:
			data[i]['marginal_prob'] = 0
		else:
			data[i]['marginal_prob'] = marginal_probs[i]
	if not mask is None:
		mask = _requite_ind_data(mask)

	return out_prop, is_lvm, marginal_probs, mask

def sis_simulation(prop, grid, data, seed, marginal_probs, use_correlogram=True, mask=None, force_single_thread=False, force_parallel=False, use_harddata=True, use_regions=False, region_size = (), min_neighbours = 0):
	
	out_prop, is_lvm, marginal_probs, mask = __prepare_sis(prop, data, marginal_probs, mask, use_harddata)	
	if not is_lvm:
		hpgl.sis_simulation(out_prop, grid.grid, data, seed, False, use_correlogram, mask)
	else:
		hpgl.sis_simulation_lvm(out_prop, grid.grid, data, seed, marginal_probs, use_correlogram, mask)		
	return out_prop

def simple_cokriging_markI(prop, grid, 
		secondary_data, primary_mean, secondary_mean, secondary_variance, correlation_coef,
		radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)
	hpgl.simple_cokriging_markI(prop, grid.grid, secondary_data, 
			primary_mean, secondary_mean, secondary_variance, correlation_coef,
			radiuses, max_neighbours, cov_model.type, cov_model.ranges, cov_model.sill, cov_model.nugget, cov_model.angles, out_prop)
	return out_prop

def simple_cokriging_markII(grid,
		primary_data,
		secondary_data,
		correlation_coef,
		radiuses,
		max_neighbours):
	out_prop = _clone_prop(primary_data["data"])
	hpgl.simple_cokriging_markII(grid.grid, primary_data, secondary_data, correlation_coef, radiuses, max_neighbours, out_prop)
	return out_prop

class IndProp:
	def __init__(self, id, undefined_value):
		self.id = id
		self.undefined_value = undefined_value

class ContProp:
	def __init__(self, id, undefined_value):
		self.id = id
		self.undefined_value = undefined_value

class MeanData:
	def __init__(self, id):
		self.id = id 


def read_eclipse_file(filename, destinations):
	load_settings = hpgl.create_load_settings()
	for dest in destinations:
		if (dest is IndProp):
			dest.prop = hpgl.create_indicator_property()
			if (type(dest.id) == int):
				load_settings.add_ind_index(dest.id, dest.prop, dest.undefined_value)
			else:
				load_settings.add_ind_name(dest.id, dest.prop, dest.undefined_value)
		elif dest is ContProp:
			dest.prop = hpgl.create_cont_property()
			if (type(dest.id) == int):
				load_settings.add_cont_index(dest.id, dest.prop, dest.undefined_value)
			else:
				load_settings.add_cont_name(dest.id, dest.prop, dest.undefined_value)
		elif dest is MeanData:
			dest.data = hpgl.create_mean_data()
			if (type(dest.id) == int):
				load_settings.add_mean_data_index(dest.id, dest.data)
			else:
				load_settings.add_mean_data_name(dest.id, dest.data)
	hpgl.read_eclipse_file(filename, load_settings)
	





	


def simple_kriging_weights(center_point, n_x, n_y, n_z, ranges = (100000,100000,100000), sill = 1, cov_type = covariance.exponential, nugget = None, angles = None):
	param = hpgl.create_sk_params()
	param.set_covariance_type(cov_type)
	param.set_sill(sill)
	param.set_ranges(*ranges)
	if (not nugget is None):
		if(nugget > sill):
			print "Error! Nugget value must be less or equal to Sill value"
			return None
		param.set_nugget(nugget)
	if (not angles is None):
		param.set_angles(*angles)
	return hpgl.simple_kriging_weights(center_point, n_x, n_y, n_z, param)

def get_gslib_property(prop_dict, prop_name, undefined_value):
	prop = prop_dict[prop_name]
	informed_array = numpy.zeros(prop.shape, dtype=numpy.uint8)
	for i in xrange(prop.size):
		if(prop[i] == undefined_value):
			informed_array[i] = 0
		else:
			informed_array[i] = 1
	return (prop_dict[prop_name], informed_array)
