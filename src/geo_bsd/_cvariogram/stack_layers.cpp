#include "api.h"

#include <math.h>
#include "stack_layers.h"


CVAR_API void
cvar_stack_layers(
		float_data_t * thick_layers,
		int * layer_markers,
		int layers_count,
		int nz,
		float scalez,
		int blank_value,
		float_data_t * result)
{
	std::vector<float_data_t> layers;
	layers.assign(thick_layers, thick_layers + layers_count);
	
	stack_layers(layers, layer_markers, nz, scalez, blank_value, *result);
}
