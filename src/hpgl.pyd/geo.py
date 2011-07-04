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

def calc_mean(prop):
	l = len(prop[0].flat)
	sum = 0
	count = 0
	for i in xrange(l):
		if prop[1].flat[i] == 1:
			sum += prop[0].flat[i]
			count += 1
	return sum/count

def calc_ind_vpc(prop, marginal_probs):
	data = prop[0]
	mask = prop[1]
	ind_count = prop[2]
	result = []
	for i in xrange(ind_count):
		result.append(numpy.zeros(data.shape, dtype='float32', order='F'))
	dz = data.shape[2]
	for z in xrange(dz):
		counts = numpy.zeros(ind_count)
		mask_slice = mask[:,:,z]
		data_slice = data[:,:,z]
		for i in xrange(data.shape[0] * data.shape[1]):
			if (mask_slice.flat[i] == 1):
				counts[data_slice.flat[i]] += 1
		if counts.sum() == 0:
			for ind in xrange(ind_count):
				result[ind][:,:,z].fill(marginal_probs[ind])
		else:
			for ind in xrange(ind_count):
				result[ind][:,:,z].fill(counts[ind] / counts.sum())
	return result


def ordinary_kriging(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, nugget = None, angles=None, use_new_cov = True):
	out_prop = _clone_prop(prop)
	param = hpgl.create_ok_params()
	param.set_covariance_type(covariance_type)
	param.set_sill(sill)
	param.set_ranges(*ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	if (not nugget is None):
		param.set_nugget(nugget)
	if (not angles is None):
		param.set_angles(*angles)
	hpgl.ordinary_kriging(prop, out_prop, grid.grid, param, use_new_cov)
	return out_prop

def simple_kriging(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None, mean=None):
	out_prop = _clone_prop(prop)
	param = hpgl.create_sk_params()
	param.set_covariance_type(covariance_type)
	param.set_sill(sill)
	param.set_ranges(*ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours),
	if (not nugget is None):
		param.set_nugget(nugget)
	if (not angles is None):
		param.set_angles(*angles)
	if (not mean is None):
		param.set_mean(mean)
	hpgl.simple_kriging(prop, out_prop, grid.grid, param)
	return out_prop

def lvm_kriging(prop, grid, mean_data, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None):
	out_prop = _clone_prop(prop)
	param = hpgl.create_ok_params()
	param.set_covariance_type(covariance_type)
	param.set_sill(sill)
	param.set_ranges(*ranges)
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours),
	if (not nugget is None):
		param.set_nugget(nugget)
	if (not angles is None):
		param.set_angles(*angles)
	hpgl.lvm_kriging(prop, out_prop, grid.grid, param, mean_data) 
	return out_prop

def sgs_simulation(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, seed, nugget=None, angles=None, kriging_type="sk", mean=None, use_harddata = True, mean_data = None, cdf_data = None, mask=None):
	if use_harddata:
		out_prop = _clone_prop(prop)
	else:
		out_prop = _empty_clone(prop)
	param = hpgl.create_sgs_params()
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_covariance_type(covariance_type)
	param.set_ranges(*ranges)
	param.set_sill(sill)
	param.set_seed(seed)
	if (kriging_type == "ok"):
		param.set_kriging_kind(0)
	else:
		param.set_kriging_kind(1)
	if not nugget is None:
		param.set_nugget(nugget)
	if not angles is None:
		param.set_angles(*angles)
	#param.set_mean_kind("stationary_auto")
	if not (mean is None or mean_data is None):
		print "Warning: both stationary mean and varying mean specified. Using varying one."	
	if not mean_data is None:
		param.set_mean_kind("varying")
	elif not mean is None:
		param.set_mean(mean)	
		param.set_mean_kind("stationary")
	else:
		param.set_mean_kind("stationary_auto")
		
	if mean_data is None:
		hpgl.sgs_simulation(out_prop, grid.grid, param, cdf_data, mask)
	else:
		hpgl.sgs_lvm_simulation(out_prop, grid.grid, param, mean_data, cdf_data, mask)

	return out_prop

def indicator_kriging(prop, grid, data, use_vpc = False):
	out_prop = _clone_prop(prop)
	hpgl.indicator_kriging(prop, out_prop, grid.grid, data, use_vpc)
	return out_prop

def median_ik(prop, grid, marginal_probs, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None, values=None):
	out_prop = _clone_prop(prop)
	param = hpgl.create_median_ik_params()
	param.set_radiuses(*radiuses)
	param.set_max_neighbours(max_neighbours)
	param.set_covariance_type(covariance_type)
	param.set_ranges(*ranges)
	param.set_sill(sill)
	if not nugget is None:
		param.set_nugget(nugget)
	if not angles is None:
		param.set_angles(*angles)
	param.set_marginal_probs(*marginal_probs)
	hpgl.median_ik(prop, out_prop, grid.grid, param)
	return out_prop

def sis_simulation(prop, grid, data, seed, use_vpc = False, mean_data=None, use_corellogram=True, mask=None, use_harddata=True):
	if use_harddata:
		out_prop = _clone_prop(prop)
	else:
		out_prop = _empty_clone(prop)
	if use_vpc and not mean_data is None:
		raise RuntimeError, "SIS simulation can't use both VPC and mean_data."
	if (mean_data == None):
		hpgl.sis_simulation(out_prop, grid.grid, data, seed, use_vpc, use_corellogram, mask)			
	else:
		hpgl.sis_simulation_lvm(out_prop, grid.grid, data, seed, mean_data, use_corellogram, mask)
	
	return out_prop

def simple_cokriging_markI(prop, grid, 
		secondary_data, primary_mean, secondary_mean, secondary_variance, correlation_coef,
		radiuses, max_neighbours, covariance_type, ranges, sill, nugget=0, angles=(0.0, 0.0, 0.0)):
	out_prop = _clone_prop(prop)
	hpgl.simple_cokriging_markI(prop, grid.grid, secondary_data, 
			primary_mean, secondary_mean, secondary_variance, correlation_coef,
			radiuses, max_neighbours, covariance_type, ranges, sill, nugget, angles, out_prop)
	return out_prop

def simple_cokriging_markII(grid,
		primary_data,
		secondary_data,
		correlation_coef,
		radiuses,
		max_neighbours):
	out_prop = _clone_prop(primary_data["data"])
	if not primary_data.has_key("angles") or primary_data["angles"] == None:
		primary_data["angles"] = (0, 0, 0)
	if not secondary_data.has_key("angles") or secondary_data["angles"] == None:
		secondary_data["angles"] = (0, 0, 0)
	if not primary_data.has_key("nugget") or primary_data["nugget"] == None:
		primary_data["nugget"] = 0
	if not secondary_data.has_key("nugget") or secondary_data["nugget"] == None:
		secondary_data["nugget"] = 0
	hpgl.simple_cokriging_markII(grid.grid, primary_data, secondary_data, correlation_coef, radiuses, max_neighbours, out_prop);
	return out_prop

class IndProp:
	def __init__(self, id, undefined_value):
		self.id = id;
		self.undefined_value = undefined_value;

class ContProp:
	def __init__(self, id, undefined_value):
		self.id = id;
		self.undefined_value = undefined_value;

class MeanData:
	def __init__(self, id):
		self.id = id; 


def read_eclipse_file(filename, destinations):
	load_settings = hpgl.create_load_settings();
	for dest in destinations:
		if (dest is IndProp):
			dest.prop = hpgl.create_indicator_property();
			if (type(dest.id) == int):
				load_settings.add_ind_index(dest.id, dest.prop, dest.undefined_value)
			else:
				load_settings.add_ind_name(dest.id, dest.prop, dest.undefined_value)
		elif dest is ContProp:
			dest.prop = hpgl.create_cont_property();
			if (type(dest.id) == int):
				load_settings.add_cont_index(dest.id, dest.prop, dest.undefined_value)
			else:
				load_settings.add_cont_name(dest.id, dest.prop, dest.undefined_value)
		elif dest is MeanData:
			dest.data = hpgl.create_mean_data();
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
		param.set_nugget(nugget)
	if (not angles is None):
		param.set_angles(*angles)
	return hpgl.simple_kriging_weights(center_point, n_x, n_y, n_z, param)

def load_gslib_file(filename):
	dict = {}
	list_prop = []
	points = []

	f = open(filename, "r")
	name = f.readline()
	num_p = int(f.readline())
	#print num_p

	for i in xrange(num_p):
		list_prop.append(str(f.readline().strip()))
	#print list_prop

	for i in xrange(len(list_prop)):
		dict[ list_prop[i] ] = numpy.array([])

	for line in f:
		points = line.split()
		for j in xrange(len(points)):
			dict[ list_prop[j] ] = numpy.concatenate( (dict[ list_prop[j] ],  numpy.array([numpy.float64(points[j])])) )

	f.close()
	return dict


def write_gslib_file(prop_dict, filename, caption):
	f = open(filename, "w")
	# 1. Caption
	f.write(caption + '\n')
	# 2. Number of properties in file
	f.write(str(len(prop_dict)) + '\n')
	# 3. Properties names
	lens = numpy.array([])
	for i in prop_dict.keys():
		f.write(i + '\n')
		lens = numpy.append(lens, len(prop_dict[i]))
	
	# Check that all properties have the same length
	if(sum(lens - lens[0]) == 0):
		# 4. Writing data
		for i in xrange(int(lens[0])):
			for key in prop_dict.keys():
				f.write(str(prop_dict[key][i]) + "   ")
			f.write('\n')
	else:
		print "ERROR! All properties in GSLIB dictionary must have equal size"

	f.close()

def get_gslib_property(prop_dict, prop_name, undefined_value):
	prop = prop_dict[prop_name]
	informed_array = numpy.zeros(prop.shape, dtype=numpy.uint8)
	for i in xrange(prop.size):
		if(prop[i] == undefined_value):
			informed_array[i] = 0
		else:
			informed_array[i] = 1
	return (prop_dict[prop_name], informed_array)
