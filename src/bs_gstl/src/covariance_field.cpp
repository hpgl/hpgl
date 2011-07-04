/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include "covariance_field.h"
#include "var_radix_utils.h"



namespace
{
	class predicate
	{
		hpgl::covariance_field_t & m_field;
	public:
		predicate(hpgl::covariance_field_t & field)
			: m_field(field)
		{}

		bool operator()(
			const hpgl::sugarbox_vector_t & vec1,
			const hpgl::sugarbox_vector_t & vec2)
		{
			return m_field.value(vec1[0], vec1[1], vec1[2]) >
				m_field.value(vec2[0], vec2[1], vec2[2]);
		}
	};

	
}

namespace hpgl
{


	

covariance_field_t::covariance_field_t(
			int xradius, 
			int yradius, 
			int zradius, 
			const Covariance<sugarbox_location_t> & cov)
{
	init (xradius, yradius, zradius, cov);
}

covariance_field_t::covariance_field_t(
		const sugarbox_search_ellipsoid_t &ellipsoid,
		const Covariance<sugarbox_location_t> & cov)		
{
	init(ellipsoid[0], ellipsoid[1], ellipsoid[2], cov);
}



void covariance_field_t::init(	
		int xradius,
		int yradius,
		int zradius,
		const Covariance<sugarbox_location_t> & cov)	
{
	m_xradius = (xradius);
	m_yradius = (yradius);
	m_zradius = (zradius);
	m_xdiameter = (xradius * 2 + 1);
	m_ydiameter = (yradius * 2 + 1);
	m_zdiameter = (zradius * 2 + 1);

	double threshold = cov(
		sugarbox_location_t(0,0,0), 
		sugarbox_location_t(0,0,0)) / 100;
	for (int z = -zradius; z <= zradius; ++z)
	{
		for (int y = -yradius; y <= yradius; ++y)
		{
			for (int x = -xradius; x <= xradius; ++x)
			{
				double value = cov(
					sugarbox_location_t(0,0,0), 
					sugarbox_location_t(x,y,z));

				m_data.push_back(value);
				if (value > threshold)
				{
					m_vectors.push_back(
						sugarbox_location_t(0,0,0) - sugarbox_location_t(x,y,z));										
				}
			}
		}
	}

	std::sort(m_vectors.begin(), m_vectors.end(), predicate(*this));

	double treshold = value(0, 0, 0) / 100;

	for (int idx = 0, end_idx = m_vectors.size(); idx < end_idx; ++idx)
	{
		sugarbox_vector_t vec = m_vectors[idx];
		if (value(vec[0],vec[1],vec[2]) < treshold)
		{
			m_vectors.resize(idx);
			break;
		}		
	}
}



}
