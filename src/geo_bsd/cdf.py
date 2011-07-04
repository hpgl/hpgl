import numpy

class CdfData:
    def __init__(self, values, probs):
        self.values = numpy.require(values, 'float32')
        self.probs = numpy.require(probs, 'float32')

def calc_cdf(prop):
    dx, dy, dz = prop.data.shape
    counts = {}
    full_count = 0
    for x in range(dx):
        for y in range(dy):
            for z in range(dz):
                if prop.mask[x,y,z] != 0:
                    value = prop.data[x,y,z]
                    full_count += 1
                    if counts.has_key(value):
                        counts[value] += 1
                    else:
                        counts[value] = 1
    full_count = float(full_count)
    values = numpy.sort(counts.keys())
    if values.size == 0:
        size = 0
    elif values.size == 1:
        size = 1
    else:
        size = values.size - 1
    values = numpy.resize(values, size)
    probs = numpy.zeros(values.shape)
    last_prob = 0.0    
    for i in range(size):
        probs[i] = last_prob + counts[values[i]] / full_count
        last_prob = probs[i]
    return CdfData(values = values, probs = probs)
