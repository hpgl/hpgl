from hpgl_wrap import _HPGL_KRIGING_KIND, _HPGL_SGS_PARAMS, hpgl_non_parametric_cdf_t,  _hpgl_so

from geo import _empty_clone, _clone_prop, _create_hpgl_cont_masked_array, _create_hpgl_float_array, _create_hpgl_ubyte_array, _require_cont_data, _requite_ind_data, __checked_create

from geo import CovarianceModel
from geo import accepts_tuple

from cdf import CdfData

import ctypes as C

import numpy

def __prepare_sgs(prop, mean=None, use_harddata=True, mask=None):
	if use_harddata:
		out_prop = _clone_prop(prop)
	else:
		out_prop = _empty_clone(prop)
	if not mean is None and not numpy.isscalar(mean):
		mean = _require_cont_data(mean)
	if not mask is None:
		mask = _requite_ind_data(mask)
	return out_prop, mean, mask

def _create_hpgl_nonparam_cdf(cdf_data):
	cd2 = cdf_data
	assert isinstance(cdf_data, CdfData)
	return __checked_create(
		hpgl_non_parametric_cdf_t,
		values = cd2.values.ctypes.data_as(C.POINTER(C.c_float)),
		probs = cd2.probs.ctypes.data_as(C.POINTER(C.c_float)),
		size = cd2.values.size)

def normed_cov_model(cov_model):
	coef = cov_model.sill - cov_model.nugget
	return CovarianceModel(
		cov_model.type, 
		cov_model.ranges, 
		cov_model.angles, 
		cov_model.sill / coef,
		cov_model.nugget / coef)

@accepts_tuple('prop', 0)
def sgs_simulation(prop, grid, cdf_data, radiuses, max_neighbours, cov_model, seed, kriging_type="sk", mean=None, use_harddata = True, use_regions=False, region_size = None, mask=None, force_single_thread=False, force_parallel=False, min_neighbours = 0, **params):
	"""Performs Sequential Gaussian Simulation

Parameters:
-----------
cdf_data: None or array_like or CdfData:
    Cumulative distribution data or data to use for calulating it. 
    If None - no cdf transformation is performed.
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
mask: None or array_like, optional:
    Array containing 1 in cell that need to be simulated and 0 in cell that aren't. 
    If None simulate all cells. Defualt: None.  
"""
	prop.fix_shape(grid)
	cov_model = normed_cov_model(cov_model)
	
	out_prop, mean, mask = __prepare_sgs(
		prop=prop, 
		mean=mean, 
		use_harddata=use_harddata, 
		mask=mask)	

	sgsp = _HPGL_SGS_PARAMS(
		covariance_type = cov_model.type,
		ranges = cov_model.ranges,
		angles = cov_model.angles,
		sill = cov_model.sill,
		nugget = cov_model.nugget,
		radiuses = radiuses,
		max_neighbours = max_neighbours,
		kriging_kind = {"sk" : _HPGL_KRIGING_KIND.simple, "ok" : _HPGL_KRIGING_KIND.ordinary}[kriging_type],
		seed = seed,
		min_neighbours = min_neighbours)

	if (cdf_data is None):
		hpgl_cdf = None
	else:
		C.byref(_create_hpgl_nonparam_cdf(cdf_data))

	hpgl_mask = C.byref(_create_hpgl_ubyte_array(mask, grid)) if mask != None else None

	hpgl_cdf = _create_hpgl_nonparam_cdf(cdf_data)
	
	if mean is None or numpy.isscalar(mean):
		_hpgl_so.hpgl_sgs_simulation(
			C.byref(_create_hpgl_cont_masked_array(out_prop, grid)),
			C.byref(sgsp),
			hpgl_cdf,
			C.byref(C.c_double(mean)) if mean != None else None,
			hpgl_mask
			)
		

	else:
		_hpgl_so.hpgl_sgs_lvm_simulation(
			C.byref(_create_hpgl_cont_masked_array(out_prop, grid)),
			C.byref(sgsp),
			hpgl_cdf,
			C.byref(_create_hpgl_float_array(mean, grid)),
			hpgl_mask)

	return out_prop
