import os
import ctypes as C
import numpy.ctypeslib as NC
import numpy

hpgl_output_handler = C.CFUNCTYPE(C.c_int, C.c_char_p, C.py_object)
hpgl_progress_handler = C.CFUNCTYPE(C.c_int, C.c_char_p, C.c_int, C.py_object)

class _HPGL_MEAN_KIND:
	stationary_auto = 0
	stationary = 1
	varying = 2

class _HPGL_KRIGING_KIND:
	ordinary = 0
	simple = 1

class _HPGL_SHAPE(C.Structure):
	_fields_ = [("data", C.c_int * 3 ),
		    ("strides", C.c_int * 3)]

class _HPGL_CONT_MASKED_ARRAY(C.Structure):
	_fields_ = [
		("data", C.POINTER(C.c_float)),
		("mask", C.POINTER(C.c_ubyte)),
		("shape", _HPGL_SHAPE)]

class _HPGL_IND_MASKED_ARRAY(C.Structure):
	_fields_ = [
		("data", C.POINTER(C.c_ubyte)),
		("mask", C.POINTER(C.c_ubyte)),
		("shape", _HPGL_SHAPE),
		("indicator_count", C.c_int)]

class _HPGL_UBYTE_ARRAY(C.Structure):
	_fields_ = [
		("data", C.POINTER(C.c_ubyte)),
		("shape", _HPGL_SHAPE)]

class _HPGL_FLOAT_ARRAY(C.Structure):
	_fields_ = [
		("data", C.POINTER(C.c_float)),
		("shape", _HPGL_SHAPE)]

class _HPGL_OK_PARAMS(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int)]

class _HPGL_SK_PARAMS(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int),
		("automatic_mean", C.c_ubyte),
		("mean", C.c_double)]

class _HPGL_SGS_PARAMS(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int),
		("kriging_kind", C.c_int),
		("seed", C.c_long),
		("min_neighbours", C.c_int),
		]

class _HPGL_MEDIAN_IK_PARAMS(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int),
		("marginal_probs", C.c_double * 2)]

class _HPGL_IK_PARAMS(C.Structure):
    _fields_ = [
        ("covariance_type", C.c_int),
        ("ranges", C.c_double * 3),
        ("angles", C.c_double * 3),
        ("sill", C.c_double),
        ("nugget", C.c_double),
        ("radiuses", C.c_int * 3),
        ("max_neighbours", C.c_int),
        ("marginal_prob", C.c_double)]

class __hpgl_cov_params_t(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double)]

class __hpgl_cockriging_m1_params_t(C.Structure):
	_fields_ = [
		("covariance_type", C.c_int),
		("ranges", C.c_double * 3),
		("angles", C.c_double * 3),
		("sill", C.c_double),
		("nugget", C.c_double),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int),
		("primary_mean", C.c_double),
		("secondary_mean", C.c_double),
		("secondary_variance", C.c_double),
		("correlation_coef", C.c_double)]

class __hpgl_cockriging_m2_params_t(C.Structure):
	_fields_ = [
		("primary_cov_params", globals()["__hpgl_cov_params_t"]),
		("secondary_cov_params", globals()["__hpgl_cov_params_t"]),
		("radiuses", C.c_int * 3),
		("max_neighbours", C.c_int),
		("primary_mean", C.c_double),
		("secondary_mean", C.c_double),
		("correlation_coef", C.c_double)]

class hpgl_non_parametric_cdf_t(C.Structure):
	_fields_ = [
		("values", C.POINTER(C.c_float)),
		("probs", C.POINTER(C.c_float)),
		("size", C.c_longlong)]

_hpgl_so = None

if os.environ.has_key('HPGL_DEBUG'):
	_hpgl_so = NC.load_library('hpgl_d', __file__)
else:
	_hpgl_so = NC.load_library('hpgl', __file__)

_hpgl_so.hpgl_set_output_handler.restype = None
_hpgl_so.hpgl_set_output_handler.argtypes = [hpgl_output_handler, C.py_object]

_hpgl_so.hpgl_set_progress_handler.restype = None
_hpgl_so.hpgl_set_progress_handler.argtypes = [hpgl_progress_handler, C.py_object]

_hpgl_so.hpgl_ordinary_kriging.restype = None
_hpgl_so.hpgl_ordinary_kriging.argtypes = [
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(_HPGL_OK_PARAMS),
	C.POINTER(_HPGL_CONT_MASKED_ARRAY)]

_hpgl_so.hpgl_simple_kriging.restype = None
_hpgl_so.hpgl_simple_kriging.argtypes =  [
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	C.POINTER(_HPGL_SHAPE),
	C.POINTER(_HPGL_SK_PARAMS),
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	C.POINTER(_HPGL_SHAPE)]

_hpgl_so.hpgl_lvm_kriging.restype = None
_hpgl_so.hpgl_lvm_kriging.argtypes = [
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	C.POINTER(_HPGL_SHAPE),
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	C.POINTER(_HPGL_SHAPE),
	C.POINTER(_HPGL_OK_PARAMS),
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	C.POINTER(_HPGL_SHAPE)]

_hpgl_so.hpgl_simple_kriging_weights.restype = C.c_int
_hpgl_so.hpgl_simple_kriging_weights.argtypes = [
	(C.c_float * 3),
	NC.ndpointer(dtype = numpy.float32),
	NC.ndpointer(dtype = numpy.float32),
	NC.ndpointer(dtype = numpy.float32),
	C.c_int,
	C.POINTER(__hpgl_cov_params_t),
	NC.ndpointer(dtype = numpy.float32)]

_hpgl_so.hpgl_sgs_simulation.restype = None
_hpgl_so.hpgl_sgs_simulation.argtypes = [
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(_HPGL_SGS_PARAMS),
	C.POINTER(hpgl_non_parametric_cdf_t),
	C.POINTER(C.c_double),
	C.POINTER(_HPGL_UBYTE_ARRAY)]

_hpgl_so.hpgl_sgs_lvm_simulation.restype = None
_hpgl_so.hpgl_sgs_lvm_simulation.argtypes = [
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(_HPGL_SGS_PARAMS),
	C.POINTER(hpgl_non_parametric_cdf_t),
	C.POINTER(_HPGL_FLOAT_ARRAY),
	C.POINTER(_HPGL_UBYTE_ARRAY)]

_hpgl_so.hpgl_median_ik.restype = None
_hpgl_so.hpgl_median_ik.argtypes = [
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.POINTER(_HPGL_MEDIAN_IK_PARAMS),
	C.POINTER(_HPGL_IND_MASKED_ARRAY)
	]

_hpgl_so.hpgl_indicator_kriging.restype = None
_hpgl_so.hpgl_indicator_kriging.argtypes = [
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.POINTER(_HPGL_IK_PARAMS),
	C.c_int]

_hpgl_so.hpgl_set_thread_num.restype = None
_hpgl_so.hpgl_set_thread_num.argtypes = [C.c_int]

_hpgl_so.hpgl_get_thread_num.restype = C.c_int
_hpgl_so.hpgl_get_thread_num.argtypes = []

_hpgl_so.hpgl_read_inc_file_float.restype = None
_hpgl_so.hpgl_read_inc_file_float.argtypes = [
	C.c_char_p,
	C.c_float,
	C.c_int,
	NC.ndpointer(dtype = numpy.float32, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A'])]

_hpgl_so.hpgl_read_inc_file_byte.restype = None
_hpgl_so.hpgl_read_inc_file_byte.argtypes = [
	C.c_char_p,
	C.c_int,
	C.c_int,
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	NC.ndpointer(dtype = numpy.ubyte, flags=['F', 'W', 'A']),
	C.c_int]

_hpgl_so.hpgl_write_inc_file_float.restype = None
_hpgl_so.hpgl_write_inc_file_float.argtypes = [
	C.c_char_p,
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.c_float,
	C.c_char_p
]

_hpgl_so.hpgl_write_inc_file_byte.restype = None
_hpgl_so.hpgl_write_inc_file_byte.argtypes = [
	C.c_char_p,
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.c_int,
	C.c_char_p,
	C.POINTER(C.c_ubyte),
	C.c_int]

_hpgl_so.hpgl_write_gslib_cont_property.restype = None
_hpgl_so.hpgl_write_gslib_cont_property.argtypes = [
		C.POINTER(_HPGL_CONT_MASKED_ARRAY),
		C.c_char_p,
		C.c_char_p,
		C.c_double]

_hpgl_so.hpgl_write_gslib_byte_property.restype = None
_hpgl_so.hpgl_write_gslib_byte_property.argtypes = [
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.c_char_p,
	C.c_char_p,
	C.c_double,
	C.POINTER(C.c_ubyte),
	C.c_int]


_hpgl_so.hpgl_sis_simulation.restype = None
_hpgl_so.hpgl_sis_simulation.argtypes = [
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.POINTER(_HPGL_IK_PARAMS),
	C.c_int,
	C.c_int,
	C.POINTER(_HPGL_UBYTE_ARRAY)]

_hpgl_so.hpgl_sis_simulation_lvm.restype = None
_hpgl_so.hpgl_sis_simulation_lvm.argtypes = [
	C.POINTER(_HPGL_IND_MASKED_ARRAY),
	C.POINTER(_HPGL_IK_PARAMS),
	C.POINTER(_HPGL_FLOAT_ARRAY),
	C.c_int,
	C.c_int,
	C.POINTER(_HPGL_UBYTE_ARRAY),
	C.c_int
	]

_hpgl_so.hpgl_simple_cokriging_mark1.restype = None
_hpgl_so.hpgl_simple_cokriging_mark1.argtypes = [
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(__hpgl_cockriging_m1_params_t),
	C.POINTER(_HPGL_CONT_MASKED_ARRAY)]

_hpgl_so.hpgl_simple_cokriging_mark2.restype = None
_hpgl_so.hpgl_simple_cokriging_mark2.argtypes = [
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(_HPGL_CONT_MASKED_ARRAY),
	C.POINTER(__hpgl_cockriging_m2_params_t),
	C.POINTER(_HPGL_CONT_MASKED_ARRAY)]
