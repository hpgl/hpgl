import numpy

import ctypes as C

from hpgl_wrap import _HPGL_SHAPE, _HPGL_CONT_MASKED_ARRAY, _HPGL_IND_MASKED_ARRAY, _HPGL_UBYTE_ARRAY, _HPGL_FLOAT_ARRAY, _HPGL_OK_PARAMS, _HPGL_SK_PARAMS, _HPGL_IK_PARAMS, _HPGL_MEDIAN_IK_PARAMS, __hpgl_cov_params_t, __hpgl_cockriging_m1_params_t, __hpgl_cockriging_m2_params_t, _hpgl_so
from hpgl_wrap import hpgl_output_handler, hpgl_progress_handler

def _c_array(ar_type, size, values):
	if len(values) != size:
		raise RuntimeError("%s values specified for array of %s elements" % (len(values), size))
	return (ar_type * size) (*values)

def _create_hpgl_shape(shape, strides=None):
	if strides is None:
		return _HPGL_SHAPE(data = _c_array(C.c_int, 3, shape),
				   strides = _c_array(C.c_int, 3, (1, shape[0], shape[0]*shape[1])))
	else:
		return _HPGL_SHAPE(data = _c_array(C.c_int, 3, shape),
				   strides = _c_array(C.c_int, 3, strides))

def __get_strides(prop):
	return (prop.strides[0] / prop.itemsize,
		prop.strides[1] / prop.itemsize,
		prop.strides[2] / prop.itemsize)


def __checked_create(T, **kargs):
    fields = []
    for f, _ in T._fields_:
        fields.append(f)
    for k in kargs.keys():
        if k in fields:
            fields.remove(k)
    assert len(fields) == 0, "No values for parameters: %s" % fields
    return T(**kargs)

def checkFWA(a):
	"""
	Checks for fortran-order, writable and aligned flags.
	"""
	assert (a.flags['F'] and a.flags['W'] and a.flags['A'])

def _create_hpgl_cont_masked_array(prop, grid):
	if (grid is None):
		sh = _create_hpgl_shape(prop.data.shape, __get_strides(prop.data))
	else:
		sh = _create_hpgl_shape((grid.x, grid.y, grid.z))
		if grid.x * grid.y * grid.z != prop.data.size:
			raise RuntimeError("Invalid data size. Size of data = %s. Size of grid = %s" % (prop.data.size, grid.x * grid.y * grid.z))

	return _HPGL_CONT_MASKED_ARRAY(
		data=prop.data.ctypes.data_as(C.POINTER(C.c_float)),
		mask=prop.mask.ctypes.data_as(C.POINTER(C.c_ubyte)),
		shape = sh)

def _create_hpgl_ind_masked_array(prop, grid):
	if (grid is None):
		sh = _create_hpgl_shape(prop.data.shape, __get_strides(prop.data))
		assert(prop.data.strides == prop.mask.strides)
	else:
		sh = _create_hpgl_shape((grid.x, grid.y, grid.z))

	return _HPGL_IND_MASKED_ARRAY(
		data=prop.data.ctypes.data_as(C.POINTER(C.c_ubyte)),
		mask=prop.mask.ctypes.data_as(C.POINTER(C.c_ubyte)),
		shape = sh,
		indicator_count = prop.indicator_count)

def _create_hpgl_ubyte_array(array, grid):
	checkFWA(array)
	if (grid is None):
		sh = _create_hpgl_shape(array.shape, strides=__get_strides(array))
	else:
		sh = _create_hpgl_shape((grid.x, grid.y, grid.z))
	return _HPGL_UBYTE_ARRAY(data = array.ctypes.data_as(C.POINTER(C.c_ubyte)), shape = sh)

def _create_hpgl_float_array(array, grid):
	checkFWA(array)
	if (grid is None):
		sh = _create_hpgl_shape(array.shape, strides=__get_strides(array))
	else:
		sh = _create_hpgl_shape((grid.x, grid.y, grid.z))
	return _HPGL_FLOAT_ARRAY(data = array.ctypes.data_as(C.POINTER(C.c_float)), shape = sh)

class ContProperty:
	def __init__(self, data, mask):
		self.data = numpy.require(data, 'float32', 'F')
		self.mask = numpy.require(mask, 'uint8', 'F')
	def validate(self):
		checkFWA(self.data)
		checkFWA(self.mask)
		assert(self.data.shape == self.mask.shape)
	def fix_shape(self, grid):
		if self.data.ndim != 3:
			if self.data.size == grid.x * grid.y * grid.z:
				self.data = self.data.reshape((grid.x, grid.y, grid.z), order='F')
		if self.mask.ndim != 3:
			if self.mask.size == grid.x * grid.y * grid.z:
				self.mask = self.mask.reshape((grid.x, grid.y, grid.z), order='F')
	def __getitem__(self, idx):
#		print "Warning. ContProperty.__getitem__ is deprecated."
		if idx == 0:
			return self.data
		elif idx == 1:
			return self.mask
		else:
			raise RuntimeError("Index out of range.")
	
class IndProperty:
	def __init__(self, data, mask, indicator_count):
		self.data = numpy.require(data, 'uint8', 'F')
		self.mask = numpy.require(mask, 'uint8', 'F')
		self.indicator_count = indicator_count
		if numpy.sum(numpy.bitwise_and((mask > 0), (data >= indicator_count))) > 0:
			raise RuntimeError("Property contains some indicators outside of [0..%s] range." % (indicator_count - 1))
		assert(data.shape == mask.shape)
	def validate(self):
		checkFWA(self.data)
		checkFWA(self.mask)
		assert(self.data.shape == self.mask.shape)
	def __getitem__(self, idx):
#		print "Warning. IndPoroperty.__getitem__ is deprecated."
		if idx == 0:
			return self.data
		elif idx == 1:
			return self.mask
		elif idx == 2:
			return self.indicator_count
		else:
			raise RuntimeError("Index out of range.")

def _prop_to_tuple_(prop):
	if isinstance(prop, ContProperty):
		return (prop.data, prop.mask)
	elif isinstance(prop, IndProperty):
		return (prop.data, prop.mask, prop.indicator_count)
	else:	
		assert False
		return prop

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
		self.x = x
		self.y = y
		self.z = z

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

	return ContProperty(numpy.array(values, dtype="float32"), numpy.array(mask, dtype="uint8"))

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

	return IndProperty(numpy.array(values, dtype="uint8", order='F'), numpy.array(mask, dtype="uint8", order='F'), len(ind_values))

def _create_cont_prop(size):
	return ContProperty(numpy.zeros(size, dtype="float32"), numpy.zeros(size, dtype='uint8'))

def _create_ind_prop(size, indicator_count):
	return IndProperty(numpy.zeros(size, dtype="uint8"), numpy.zeros(size, dtype='uint8'), indicator_count)

def _empty_clone(prop):	
	data2 = prop.data.copy('F')
	data2.fill(0)
	mask2 = prop.mask.copy('F')
	mask2.fill(0)
	if isinstance(prop, IndProperty):
		return IndProperty(data2, mask2, prop.indicator_count)
	elif isinstance(prop, ContProperty):
		return ContProperty(data2, mask2)
	else:
		assert False

def _clone_prop(prop):
	data2 = prop.data.copy('F')
	mask2 = prop.mask.copy('F')
	if isinstance(prop, IndProperty):
		return IndProperty(data2, mask2, prop.indicator_count)
	elif isinstance(prop, ContProperty):
		return ContProperty(data2, mask2)
	else:
		assert False, "Unknown prop type"

def _require_cont_data(data):
	if data is None:
		return None
	return numpy.require(data, dtype='float32', requirements='F')

def _requite_ind_data(data):
	if data is None:
		return None
	return numpy.require(data, dtype='uint8', requirements='F')

def accepts_tuple(arg_name, arg_pos):
	def tuple_to_prop(t):
		if isinstance(t, tuple):
			if len(t) == 3:
				return IndProperty(*t)
			elif len(t) == 2:
				return ContProperty(*t)
			else:
				assert False
		else:
			assert isinstance(t, ContProperty) or isinstance(t, IndProperty)
			return t
	def decorator(f):
		def new_f(*args, **kargs):
			if kargs.has_key(arg_name):
				kargs[arg_name] = tuple_to_prop(kargs[arg_name])
			elif len(args) > arg_pos:
				args = args[:arg_pos] + (tuple_to_prop(args[arg_pos]),) + args[arg_pos+1:]
			else:
				assert False
			return f(*args, **kargs)
		new_f.func_name = f.func_name
		return new_f
	return decorator
		
@accepts_tuple('prop', 0)
def write_property(prop, filename, prop_name, undefined_value, indicator_values=[]):
	if (prop.data.ndim == 3):
		sh = _create_hpgl_shape(prop.data.shape)
	else:
		sh = _create_hpgl_shape((prop.data.size, 1, 1))
	if isinstance(prop, ContProperty):
		marr = _HPGL_CONT_MASKED_ARRAY(
			data = prop.data.ctypes.data_as(C.POINTER(C.c_float)),
			mask = prop.mask.ctypes.data_as(C.POINTER(C.c_ubyte)),
			shape = sh)
		_hpgl_so.hpgl_write_inc_file_float(
			filename,
			C.byref(marr),
			undefined_value,
			prop_name)
	else:		
		marr = _HPGL_IND_MASKED_ARRAY(
			data = prop.data.ctypes.data_as(C.POINTER(C.c_ubyte)),
			mask = prop.mask.ctypes.data_as(C.POINTER(C.c_ubyte)),
			shape = sh,
			indicator_count = prop.indicator_count)
		_hpgl_so.hpgl_write_inc_file_byte(
			filename,
			C.byref(marr),
			undefined_value,
			prop_name,
			numpy.array(indicator_values).ctypes.data_as(C.POINTER(C.c_ubyte)),
			len(indicator_values))

@accepts_tuple('prop', 0)
def write_gslib_property(prop, filename, prop_name, undefined_value, indicator_values=[]):	
	if isinstance(prop, ContProperty):
		_hpgl_so.hpgl_write_gslib_cont_property(
			_create_hpgl_cont_masked_array(prop, None),
			filename,
			prop_name,
			undefined_value)
	else:
		_hpgl_so.hpgl_write_gslib_byte_property(
			_create_hpgl_ind_masked_array(prop, None),
			filename,
			prop_name,
			undefined_value,
			_c_array(C.c_ubyte, len(indicator_values), indicator_values),
			len(indicator_values))

def load_cont_property(filename, undefined_value, size=None):
	if size is None:			
		print "[WARNING]. load_cont_property: Size is not specified. Using slow and ineficcient method."
		return _load_prop_cont_slow(filename, undefined_value)
	else:
		return read_inc_file_float(filename, undefined_value, size)

def read_inc_file_float(filename, undefined_value, size):
	data = numpy.zeros((size), dtype='float32', order='F')
	mask = numpy.zeros((size), dtype='uint8', order='F')

	_hpgl_so.hpgl_read_inc_file_float(
		filename,
		undefined_value,
		size[0]*size[1]*size[2],
		data,
		mask)

	return ContProperty(data, mask)

def read_inc_file_byte(filename, undefined_value, size, indicator_values):
	data = numpy.zeros(size, dtype='uint8', order='F')
	mask = numpy.zeros(size, dtype='uint8', order='F')
	_hpgl_so.hpgl_read_inc_file_byte(
		filename,
		undefined_value,
		size[0] * size[1] * size[2],
		data,
		mask,
		numpy.array(indicator_values, dtype='uint8'),
		len(indicator_values))
	return IndProperty(data, mask, len(indicator_values))

def load_ind_property(filename, undefined_value, indicator_values, size=None):
	if (size is None):
		print "[WARNING]. load_ind_property: Size is not specified. Using slow and ineficcient method."
		return _load_prop_ind_slow(filename, undefined_value, indicator_values)
	else:
		return read_inc_file_byte(filename, undefined_value, size, indicator_values)

def set_thread_num(num):
	_hpgl_so.hpgl_set_thread_num(num)

def get_thread_num():
	return _hpgl_so.hpgl_get_thread_num()

@accepts_tuple('prop', 0)
def calc_mean(prop):
	l = len(prop.data.flat)
	sum = 0
	count = 0
	for i in xrange(l):
		if prop.mask.flat[i] == 1:
			sum += prop.data.flat[i]
			count += 1
	return sum/count

@accepts_tuple('prop', 0)
def ordinary_kriging(prop, grid, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)

	okp = _HPGL_OK_PARAMS(
		covariance_type = cov_model.type,
		ranges = cov_model.ranges,
		angles = cov_model.angles,
		sill = cov_model.sill,
		nugget = cov_model.nugget,
		radiuses = radiuses,
		max_neighbours = max_neighbours)

	_hpgl_so.hpgl_ordinary_kriging(
		_create_hpgl_cont_masked_array(prop, grid),
		C.byref(okp),
		_create_hpgl_cont_masked_array(out_prop, grid))

	return out_prop

@accepts_tuple('prop', 0)
def simple_kriging(prop, grid, radiuses, max_neighbours, cov_model, mean=None):
	out_prop = _clone_prop(prop)

	skp = _HPGL_SK_PARAMS(
		covariance_type = cov_model.type,
		ranges = cov_model.ranges,
		angles = cov_model.angles,
		sill = cov_model.sill,
		nugget = cov_model.nugget,
		radiuses = radiuses,
		max_neighbours = max_neighbours,
		automatic_mean = (mean is None),
		mean = (mean if mean != None else 0))

	sh_data = (C.c_int * 3)(grid.x, grid.y, grid.z)
	sh = _HPGL_SHAPE(data=sh_data)

	_hpgl_so.hpgl_simple_kriging(
		prop.data, prop.mask, 
		C.byref(sh), C.byref(skp),
		out_prop[0], out_prop[1],
		C.byref(sh))

	return out_prop

@accepts_tuple('prop', 0)
def lvm_kriging(prop, grid, mean_data, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)

	okp = _HPGL_OK_PARAMS(
		covariance_type = cov_model.type,
		ranges = cov_model.ranges,
		angles = cov_model.angles,
		sill = cov_model.sill,
		nugget = cov_model.nugget,
		radiuses = radiuses,
		max_neighbours = max_neighbours)

	sh_data = (C.c_int * 3)(grid.x, grid.y, grid.z)
	sh = _HPGL_SHAPE(data=sh_data)

	_hpgl_so.hpgl_lvm_kriging(
		prop.data, prop.mask, C.byref(sh), 
		mean_data, C.byref(sh),
		C.byref(okp),
		out_prop.data, out_prop.mask,
		C.byref(sh))

	return out_prop

@accepts_tuple('prop', 0)
def median_ik(prop, grid, marginal_probs, radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)

	miksp = _HPGL_MEDIAN_IK_PARAMS(
		covariance_type = cov_model.type,
		ranges = cov_model.ranges,
		angles = cov_model.angles,
		sill = cov_model.sill,
		nugget = cov_model.nugget,
		radiuses = radiuses,
		max_neighbours = max_neighbours,
		marginal_probs = marginal_probs)

	inp = _create_hpgl_ind_masked_array(prop, grid)
	outp = _create_hpgl_ind_masked_array(out_prop, grid)
	_hpgl_so.hpgl_median_ik(C.byref(inp), C.byref(miksp), C.byref(outp))
	return out_prop

def __create_hpgl_ik_params(data, indicator_count, is_lvm, marginal_probs):
	ikps = []
	assert len(data) == indicator_count
	for i in range(indicator_count):
		ikd = data[i]
		ikp = __checked_create(
			_HPGL_IK_PARAMS,
			covariance_type = ikd["cov_model"].type,
			ranges = _c_array(C.c_double, 3, ikd["cov_model"].ranges),
			angles = _c_array(C.c_double, 3, ikd["cov_model"].angles),
			sill = ikd["cov_model"].sill,
			nugget = ikd["cov_model"].nugget,
			radiuses = _c_array(C.c_int, 3, ikd["radiuses"]),
			max_neighbours = ikd["max_neighbours"],
			marginal_prob = 0 if is_lvm else marginal_probs[i])
		ikps.append(ikp)
	return _c_array(_HPGL_IK_PARAMS, indicator_count, ikps)

@accepts_tuple('prop', 0)
def indicator_kriging(prop, grid, data, marginal_probs):
	for i in xrange(len(data)):
		data[i]['marginal_prob'] = marginal_probs[i]
	if(len(data) == 2):
		return median_ik(
			prop, 
			grid, 
			(data[0]["marginal_prob"], 
			 1 - data[0]["marginal_prob"]), 
			data[0]["radiuses"], 
			data[0]["max_neighbours"],
			data[0]['cov_model'])	
	out_prop = _clone_prop(prop)
	_hpgl_so.hpgl_indicator_kriging(
		C.byref(_create_hpgl_ind_masked_array(prop, grid)),
		C.byref(_create_hpgl_ind_masked_array(out_prop, grid)),
		__create_hpgl_ik_params(data, len(data), False, marginal_probs),
		len(data))

	return out_prop

@accepts_tuple('prop', 0)
def simple_cokriging_markI(prop, grid, 
		secondary_data, primary_mean, secondary_mean, secondary_variance, correlation_coef,
		radiuses, max_neighbours, cov_model):
	out_prop = _clone_prop(prop)

	_hpgl_so.hpgl_simple_cokriging_mark1(
		C.byref(_create_hpgl_cont_masked_array(prop, grid)),
		C.byref(_create_hpgl_cont_masked_array(secondary_data, grid)),
		C.byref(__checked_create(
				__hpgl_cockriging_m1_params_t, 
				covariance_type = cov_model.type,
				ranges = _c_array(C.c_double, 3, cov_model.ranges),
				angles = _c_array(C.c_double, 3, cov_model.angles),
				sill = cov_model.sill,
				nugget = cov_model.nugget,
				radiuses = _c_array(C.c_int, 3, radiuses),
				max_neighbours = max_neighbours,
				primary_mean = primary_mean,
				secondary_mean = secondary_mean,
				secondary_variance = secondary_variance,
				correlation_coef = correlation_coef)),
		C.byref(_create_hpgl_cont_masked_array(out_prop, grid)))
	return out_prop

def simple_cokriging_markII(grid,
		primary_data,
		secondary_data,
		correlation_coef,
		radiuses,
		max_neighbours):
	out_prop = _clone_prop(primary_data["data"])

	pcp = primary_data["cov_model"]
	scp = secondary_data["cov_model"]

	_hpgl_so.hpgl_simple_cokriging_mark2(
		C.byref(_create_hpgl_cont_masked_array(primary_data["data"], grid)),
		C.byref(_create_hpgl_cont_masked_array(secondary_data["data"], grid)),
		C.byref(__checked_create(
				__hpgl_cockriging_m2_params_t,
				primary_cov_params = __checked_create(
					__hpgl_cov_params_t,
					covariance_type = pcp.type,
					ranges = _c_array(C.c_double, 3, pcp.ranges),
					angles = _c_array(C.c_double, 3, pcp.angles),
					sill = pcp.sill,
					nugget = pcp.nugget),
				secondary_cov_params = __checked_create(
					__hpgl_cov_params_t,
					covariance_type = scp.type,
					ranges = _c_array(C.c_double, 3, scp.ranges),
					angles = _c_array(C.c_double, 3, scp.angles),
					sill = scp.sill,
					nugget = scp.nugget),
				radiuses = _c_array(C.c_int, 3, radiuses),
				max_neighbours = max_neighbours,
				primary_mean = primary_data["mean"],
				secondary_mean = secondary_data["mean"],
				correlation_coef = correlation_coef)),
		C.byref(_create_hpgl_cont_masked_array(out_prop, grid)))
	return out_prop

def simple_kriging_weights(center_point, n_x, n_y, n_z, ranges = (100000,100000,100000), sill = 1, cov_type = covariance.exponential, nugget = None, angles = None):
	if angles is None:
		angles = (0,0,0)
	if nugget is None:
		nugget = 0
	covp = C.byref(__checked_create(
			__hpgl_cov_params_t, 
			covariance_type = cov_type,
			ranges = _c_array(C.c_double, 3, ranges),
			angles = _c_array(C.c_double, 3, angles),
			sill = sill,
			nugget = nugget))

	if len(n_x) != len(n_y) or len(n_x) != len(n_z):
		raise RuntimeError("Invalid pointset. %s,%s,%s." % (len(n_x), len(n_y), len(n_z)))
	assert(len(n_x) == len(n_y)) 
	assert(len(n_x) == len(n_z))

	if nugget > sill:
		raise RuntimeError("Nugget value must be less or equal to Sill value.")

	weights = numpy.array([0]*len(n_x), dtype='float32')

	_hpgl_so.hpgl_simple_kriging_weights(
		_c_array(C.c_float, 3, center_point),
		numpy.array(n_x, dtype='float32'),
		numpy.array(n_y, dtype='float32'),
		numpy.array(n_z, dtype='float32'),
		len(n_x),
		covp,
		weights)

	return weights

def get_gslib_property(prop_dict, prop_name, undefined_value):
	prop = prop_dict[prop_name]
	informed_array = numpy.zeros(prop.shape, dtype=numpy.uint8)
	for i in xrange(prop.size):
		if(prop[i] == undefined_value):
			informed_array[i] = 0
		else:
			informed_array[i] = 1
	return (prop_dict[prop_name], informed_array)

def set_output_handler(handler, param):
	global h
	if (handler is None):
		_hpgl_so.hpgl_set_output_handler(None, None)
		h = None
	else:
		h = hpgl_output_handler(handler)
		_hpgl_so.hpgl_set_output_handler(h, param)


def set_progress_handler(handler, param):
	global progress_handler
	if (handler is None):
		_hpgl_so.hpgl_set_output_handler(None, None)
		progress_handler = None
	else:
		progress_handler = hpgl_progress_handler(handler)
		_hpgl_so.hpgl_set_progress_handler(progress_handler, param)
