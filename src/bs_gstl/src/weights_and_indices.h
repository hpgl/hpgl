/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __WEIGHTS_AND_INDICES_H__3862498C_8F5B_49F9_A5B3_A94F1366D581
#define __WEIGHTS_AND_INDICES_H__3862498C_8F5B_49F9_A5B3_A94F1366D581

namespace hpgl
{
	struct weights_and_indices_t
	{	
		typedef std::vector<node_index_t> index_vector_t;
		typedef std::vector<kriging_weight_t> weights_vector_t;
		index_vector_t m_indices;
		weights_vector_t m_weights;	
	};
}

#endif //__WEIGHTS_AND_INDICES_H__3862498C_8F5B_49F9_A5B3_A94F1366D581