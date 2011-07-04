import ctypes as C
import numpy.ctypeslib as NC
import numpy

#from _cvariogram import CStackLayers

cvar = NC.load_library('_cvariogram', __file__)

class vector_t(C.Structure):
    _fields_ = [("data", C.c_double * 3)]

class ellipsoid_t(C.Structure):
    _fields_ = [
        ("direction1", vector_t),
        ("direction2", vector_t),
        ("direction3", vector_t),
        ("R1", C.c_double),
        ("R2", C.c_double),
        ("R3", C.c_double)]

class variogram_search_template_t (C.Structure):
    _fields_ = [
        ("lag_width", C.c_double),
        ("lag_separation", C.c_double),
        ("tol_distance", C.c_double),
        ("num_lags", C.c_int),
        ("first_lag_distance", C.c_double),
        ("ellipsoid", ellipsoid_t)]


class hard_data_t (C.Structure):
    _fields_ = [
        ("data", C.POINTER(C.c_float)),
        ("mask", C.POINTER(C.c_ubyte)),
        ("data_shape", (C.c_int * 3)),
        ("data_strides", (C.c_int * 3)),
        ("mask_shape", (C.c_int * 3)),
        ("mask_strides", (C.c_int * 3))]

class cont_point_set_t (C.Structure):
    _fields_ = [
        ("xs", C.POINTER(C.c_float)),
        ("ys", C.POINTER(C.c_float)),
        ("zs", C.POINTER(C.c_float)),
        ("values", C.POINTER(C.c_float)),
        ("values_count", C.c_int)]

class float_data_t (C.Structure):
    _fields_ = [
        ("data", C.POINTER(C.c_float)),
        ("data_shape", (C.c_int * 3)),
        ("data_strides", (C.c_int * 3))]


cvar.fill_ellipsoid_directions.restype = None
cvar.fill_ellipsoid_directions.argtypes = [
    C.POINTER(ellipsoid_t), C.c_double, C.c_double, C.c_double]

cvar.calc_variograms.restype = None
cvar.calc_variograms.argtypes = [
            C.POINTER(variogram_search_template_t), 
            C.POINTER(hard_data_t),
            NC.ndpointer(dtype = numpy.float32),
            C.c_int,
            C.c_int]

cvar.calc_variograms_from_point_set.restype = None
cvar.calc_variograms_from_point_set.argtypes = [
            C.POINTER(variogram_search_template_t),
            C.POINTER(cont_point_set_t),
            NC.ndpointer(dtype = numpy.float32),
            C.c_int]

cvar.cvar_stack_layers.restype = None
cvar.cvar_stack_layers.argtypes = [
    C.POINTER(float_data_t),
    C.POINTER(C.c_int),
    C.c_int,
    C.c_int,
    C.c_float,
    C.c_int,
    C.POINTER(float_data_t)]

def checked_create(T, **kargs):
    fields = []
    for f, _ in T._fields_:
        fields.append(f)
    for k in kargs.keys():
        if k in fields:
            fields.remove(k)
    assert len(fields) == 0, "No values for parameters: %s" % fields
    return T(**kargs)

def __strides(array):
    return (array.strides[0] / array.itemsize, array.strides[1] / array.itemsize, array.strides[2] / array.itemsize)

def _c_array(t, size, values):
    return (t * size)(*values)

class Ellipsoid:
    def __init__(self, R1, R2, R3, azimuth, dip, rotation):
        vec = checked_create(vector_t, data = _c_array(C.c_double, 3, (0,0,0)))
        self.ell = checked_create(
            ellipsoid_t,
            direction1 = vec,
            direction2 = vec,
            direction3 = vec,
            R1 = R1,
            R2 = R2,
            R3 = R3)
        cvar.fill_ellipsoid_directions(C.byref(self.ell), azimuth, dip, rotation)


class VariogramSearchTemplate:
    def __init__(self, lag_width, lag_separation, tol_distance, num_lags, first_lag_distance, ellipsoid):
        self.templ = checked_create(
            variogram_search_template_t,
            lag_width = lag_width,
            lag_separation = lag_separation,
            tol_distance = tol_distance,
            num_lags = num_lags,
            first_lag_distance = first_lag_distance,
            ellipsoid = ellipsoid.ell)

        self.num_lags = num_lags
	self.lag_separation = lag_separation
        
       
def CalcVariograms(templ, hard_data, percent=100):
    variogram = numpy.array([0] * templ.num_lags, dtype='float32')
    
    hd = checked_create(
        hard_data_t,
        data = hard_data[0].ctypes.data_as(C.POINTER(C.c_float)),
        mask = hard_data[1].ctypes.data_as(C.POINTER(C.c_ubyte)),
        data_shape = _c_array(C.c_int, 3, hard_data[0].shape),
        mask_shape = _c_array(C.c_int, 3, hard_data[1].shape),
        data_strides = _c_array(C.c_int, 3, __strides(hard_data[0])),
        mask_strides = _c_array(C.c_int, 3, __strides(hard_data[1])))
    
    cvar.calc_variograms(
        C.byref(templ.templ),
        C.byref(hd),
        variogram,
        variogram.size,
        percent)

    lags_borders = numpy.zeros(templ.num_lags)

    for k in xrange(templ.num_lags):
        lags_borders[k] = k * templ.lag_separation

    return (lags_borders, variogram)

def CalcVariogramsFromPointSet(templ, point_set, variogram):
    variogram = numpy.array([0] * templ.num_lags, dtype='float32')

    ps = checked_create(
        cont_point_set_t,
        xs = point_set["X"].ctypes.data_as(C.POINTER(C.c_float)),
        ys = point_set["Y"].ctypes.data_as(C.POINTER(C.c_float)),
        zs = point_set["Z"].ctypes.data_as(C.POINTER(C.c_float)),
        values = point_set("Property").ctypes.data_as(C.POINTER(C.c_float)),
        values_count = values.size)

    cvar.calc_variograms_from_point_set(
        C.byref(templ.templ),
        C.byref(ps),
        variogram,
        variogram.size)

    lags_borders = numpy.zeros(templ.num_lags)

    for k in xrange(templ.num_lags):
        lags_borders[k] = k * templ.lag_separation

    return (lags_borders, variogram)

def _create_float_data(array):
    return __checked_create(
        float_data_t,
        data = array.ctypes.data_as(C.POINTER(C.c_float)),
        data_shape = _c_array(C.c_int, 3, array.shape),
        data_strides = _c_array(C.c_int, 3, __strides(array)))

def CStackLayers(layers, markers, nz, scalez, blank_value, result):
    layers2 = []
    for layer in layers:
        layers2.append(_create_float_data(layer))

    result2 = _create_float_data(result)
    cvar.cvar_stack_layers(
        _c_array(float_data_t, len(layers2), layers2),
        _c_array(C.c_int, len(markers), markers),
        len(layers2),
        nz,
        scalez,
        blank_value,
        C.byref(result2))

