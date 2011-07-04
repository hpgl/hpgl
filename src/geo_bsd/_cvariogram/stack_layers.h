#include <vector>
#include <iostream>

void stack_layers(
	std::vector<float_data_t> & thick_layers, 
	int * layers_markers,
	int nz,
	float scalez,
	int blank_value,
	float_data_t & result)
{
	int nx = thick_layers[0].m_data_shape[0];
	int ny = thick_layers[0].m_data_shape[1];

	std::vector<double> cumulative_k(nx*ny, 0.0);

	for(int layer = 0; layer < thick_layers.size(); layer++)
	{
		for(int i = 0; i < nx; i++)
		{
			for(int j = 0; j < ny; j++)
			{
				int map_index = thick_layers[layer].m_data_strides[0] * i + thick_layers[layer].m_data_strides[1] * j;

				// positive layer
				if(thick_layers[layer].m_data[map_index] > 0)
				{
					float old_k = cumulative_k[map_index];
					cumulative_k[map_index] += thick_layers[layer].m_data[map_index] / scalez;
					float new_k = cumulative_k[map_index];

					//int k_next = floor((thick_layers[layer].m_data[map_index] / scalez) + cumulative_k[map_index]);
					int k_next = floor(new_k) + 1;
					if(k_next > nz)
					{
						k_next = nz;
					}

					for(int k = ceil(old_k); k < k_next; k++)
					{
						int cube_index = result.m_data_strides[0]*i + result.m_data_strides[1]*j + result.m_data_strides[2]*k;
						result.m_data[cube_index] = layers_markers[layer];
						
					}
					//cumulative_k[map_index] += (thick_layers[layer].m_data[map_index] / scalez);
				}
				// negative layer
				else
				{
					int k_start = ceil(cumulative_k[map_index] + (thick_layers[layer].m_data[map_index] / scalez));
					for(int k = k_start; k < nz; k++)
					{
						int cube_index = result.m_data_strides[0]*i + result.m_data_strides[1]*j + result.m_data_strides[2]*k;
						result.m_data[cube_index] = blank_value;

					}
					cumulative_k[map_index] +=(thick_layers[layer].m_data[map_index] / scalez);

				}
			}
		}
	}
	
}