#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

import hpgl
from hpgl import create_sugarbox_grid
from hpgl import load_mean_data
from hpgl import load_indicator_mean_data
from hpgl import calc_vpc
from hpgl import write_mean_data
from hpgl import calc_mean

load_secondary_data = load_mean_data

class covariance:
	spherical = 0
	exponential = 1
	gaussian = 2

class SugarboxGrid:
	def __init__(self, x, y, z):
		self.grid = create_sugarbox_grid(x, y, z)
		
def write_property(prop_object, filename, prop_name, undefined_value, indicator_values=[]):
	if type(prop_object) is hpgl.cont_property_array:
		hpgl.write_cont_property(prop_object, filename, prop_name, undefined_value)
	else:
		hpgl.write_byte_property(prop_object, filename, prop_name, undefined_value, indicator_values)

def write_gslib_property(prop_object, filename, prop_name, undefined_value, x, y, z, indicator_values=[]):
	if type(prop_object) is hpgl.cont_property_array:
		hpgl.write_cont_gslib_property(prop_object, filename, prop_name, undefined_value, x, y, z)
	else:
		hpgl.write_byte_gslib_property(prop_object, filename, prop_name, undefined_value, x, y, z, indicator_values)	

#def load_property(type, filename, undefined_value, indicator_values = None):
#	hpgl.load_property(type, filename, undefined_value)

def load_cont_property(filename, undefined_value):
	return hpgl.load_cont_property(filename, undefined_value)

def load_ind_property(filename, undefined_value, indicator_values):
	return hpgl.load_ind_property(filename, undefined_value, indicator_values)

def load_gslib_cont_property(filename, undefined_value,i,j,k):
	return hpgl.load_gslib_cont_property(filename, undefined_value,i,j,k)

def load_gslib_ind_property(filename, undefined_value, indicator_values,i,j,k):
	return hpgl.load_gslib_ind_property(filename, undefined_value, indicator_values,i,j,k)
	

def ordinary_kriging(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, nugget = None, angles=None, use_new_cov = True):
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
	return hpgl.ordinary_kriging(prop, grid.grid, param, use_new_cov)

def simple_kriging(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None, mean=None):
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
	return hpgl.simple_kriging(prop, grid.grid, param) 

def lvm_kriging(prop, grid, mean_data, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None):
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
	return hpgl.lvm_kriging(prop, grid.grid, param, mean_data) 

def sgs_simulation(prop, grid, radiuses, max_neighbours, covariance_type, ranges, sill, seed, nugget=None, angles=None, kriging_type="sk", mean=None, use_harddata = True, mean_data = None, cdf_data = None, mask=None):
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
		#param.set_mean(hpgl.calc_mean(prop))
		param.set_mean_kind("stationary_auto")
		
	if mean_data is None:
		return hpgl.sgs_simulation(prop, grid.grid, param, use_harddata, cdf_data, mask)
	else:
		return hpgl.sgs_lvm_simulation(prop, grid.grid, param, mean_data, use_harddata, cdf_data, mask)
	
def indicator_kriging(prop, grid, data, use_vpc = False):
	return hpgl.indicator_kriging(prop, grid.grid, data, use_vpc)

def median_ik(prop, grid, marginal_probs, radiuses, max_neighbours, covariance_type, ranges, sill, nugget=None, angles=None, values=None):
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
	return hpgl.median_ik(prop, grid.grid, param)

def sis_simulation(prop, grid, data, seed, use_vpc = False, mean_data=None, use_corellogram=True, mask=None):
	if use_vpc and not mean_data is None:
		raise RuntimeError, "SIS simulation can't use both VPC and mean_data."
	if (mean_data == None):
		return hpgl.sis_simulation(prop, grid.grid, data, seed, use_vpc, use_corellogram, mask)
	else:
		return hpgl.sis_simulation_lvm(prop, grid.grid, data, seed, mean_data, use_corellogram, mask)

def simple_cokriging_markI(prop, grid, 
		secondary_data, primary_mean, secondary_mean, secondary_variance, correlation_coef,
		radiuses, max_neighbours, covariance_type, ranges, sill, nugget=0, angles=(0.0, 0.0, 0.0)):
	return hpgl.simple_cokriging_markI(prop, grid.grid, secondary_data, primary_mean, secondary_mean, secondary_variance, correlation_coef,
			radiuses, max_neighbours, covariance_type, ranges, sill, nugget, angles)

def simple_cokriging_markII(grid,
		primary_data,
		secondary_data,
		correlation_coef,
		radiuses,
		max_neighbours):
	if not primary_data.has_key("angles") or primary_data["angles"] == None:
		primary_data["angles"] = (0, 0, 0)
	if not secondary_data.has_key("angles") or secondary_data["angles"] == None:
		secondary_data["angles"] = (0, 0, 0)
	return hpgl.simple_cokriging_markII(grid.grid, primary_data, secondary_data, correlation_coef, radiuses, max_neighbours);

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
	
def calc_vpc(prop, grid, marginal_probs):
	return hpgl.calc_vpc(prop, grid.grid, marginal_probs)

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





