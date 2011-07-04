/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __VARIABLE_H__08A0F060_D289_4920_8860_A545A21284BF
#define __VARIABLE_H__08A0F060_D289_4920_8860_A545A21284BF

namespace hpgl
{
	template<
		typename values_t,		
		typename nb_lookup_t,
		typename means_t,
		typename cov_model_t
		>
	struct variable_t
	{
		typedef values_t value_type;
		typedef nb_lookup_t neighbour_lookup_type;
		typedef means_t means_type;
		typedef cov_model_t cov_model_type;

		variable_t()
		{}

		variable_t(values_t * data, nb_lookup_t * nb_lookup, means_t * means, cov_model_t * cov_model)
			: m_data(data), m_nb_lookup(nb_lookup), m_means(means), m_cov_model(cov_model)
		{}

		values_t * m_data;
		inline values_t & values(){return *m_data;}
		
		nb_lookup_t * m_nb_lookup;
		inline nb_lookup_t & nb_lookup(){return *m_nb_lookup;}

		means_t * m_means;
		inline means_t & means(){return *m_means;}

		cov_model_t * m_cov_model;
		inline cov_model_t & cov_model(){return *m_cov_model;}
	};
}

#endif //__VARIABLE_H__08A0F060_D289_4920_8860_A545A21284BF