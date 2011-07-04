/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef COVARIANCE_FIELD_H_INCLUDED_IN_BLUE_SKY_GSTL_AND_SOME_RANDOM_SYMBOLS_AFKJSDFKSJFKSDLKASDHKLWIUEWRWIWEIURH
#define COVARIANCE_FIELD_H_INCLUDED_IN_BLUE_SKY_GSTL_AND_SOME_RANDOM_SYMBOLS_AFKJSDFKSJFKSDLKASDHKLWIUEWRWIWEIURH

#include <typedefs.h>
#include "var_radix_utils.h"
#include "cov_model.h"
#include "sugarbox_grid.h"

namespace hpgl
{
	namespace detail
	{
		class predicate_2
		{
			const std::vector<hpgl::covariance_t> & m_data;
			int m_xradius;
			int m_yradius;
			int m_zradius;
			int m_xd;
			int m_yd;
		public:
			predicate_2(const std::vector<hpgl::covariance_t> & data, int xradius, int yradius, int zradius)
				: m_data(data), m_xradius(xradius), m_yradius(yradius), m_zradius(zradius), m_xd(xradius * 2 + 1), m_yd(yradius * 2 + 1)
			{}

			bool operator()(
				const hpgl::sugarbox_vector_t & vec1,
				const hpgl::sugarbox_vector_t & vec2)
			{
				hpgl::covariance_t value1 = m_data[hpgl::vr_to_dec(m_yd, m_xd, vec1[2] + m_zradius, vec1[1] + m_yradius, vec1[0] + m_xradius)];
				hpgl::covariance_t value2 = m_data[hpgl::vr_to_dec(m_yd, m_xd, vec2[2] + m_zradius, vec2[1] + m_yradius, vec2[0] + m_xradius)];
				return value1 > value2;			
			}
		};
	}

// calc_distance vector case
	template <typename coord_t>
	void calc_dist_field(const search_area_t & area, std::vector<sugarbox_vector_t> & vectors)
	{
		int m_xradius = area[0];
		int m_yradius = area[1];
		int m_zradius = area[2];
		int m_xdiameter = (m_xradius * 2 + 1);
		int m_ydiameter = (m_yradius * 2 + 1);
		int m_zdiameter = (m_zradius * 2 + 1);

		for (int z = -m_zradius; z <= m_zradius; ++z)
		{
			for (int y = -m_yradius; y <= m_yradius; ++y)
			{
				for (int x = -m_xradius; x <= m_xradius; ++x)
				{
					vectors.push_back(sugarbox_location_t(0,0,0) - sugarbox_location_t(x,y,z));										
				}
			}
		}
	}

	// type covariance_model_t:
	//     covariance_t operator(coord_t, coord_t)
	template <typename covariance_model_t, typename coord_t>
	void calc_cov_field(const search_area_t & area, const covariance_model_t & cov, std::vector<sugarbox_vector_t> & vectors)
	{
		std::vector<covariance_t> data;	
		int m_xradius = area[0];
		int m_yradius = area[1];
		int m_zradius = area[2];
		int m_xdiameter = (m_xradius * 2 + 1);
		int m_ydiameter = (m_yradius * 2 + 1);
		int m_zdiameter = (m_zradius * 2 + 1);

		double threshold = cov(coord_t(0, 0, 0), coord_t(0,0,0)) / 100;
		for (int z = -m_zradius; z <= m_zradius; ++z)
		{
			for (int y = -m_yradius; y <= m_yradius; ++y)
			{
				for (int x = -m_xradius; x <= m_xradius; ++x)
				{
					double value = cov(coord_t(0,0,0), coord_t(x,y,z));

					data.push_back(value);
					if (value > threshold)
					{
						vectors.push_back(sugarbox_location_t(0,0,0) - sugarbox_location_t(x,y,z));										
					}
				}
			}
		}

		std::sort(vectors.begin(), vectors.end(), detail::predicate_2(data, m_xradius, m_yradius, m_zradius));

		double treshold = cov(coord_t(0, 0, 0), coord_t(0,0,0)) / 100;

		for (int idx = 0, end_idx = (int) vectors.size(); idx < end_idx; ++idx)
		{
			sugarbox_vector_t vec = vectors[idx];
			if (cov(coord_t(0,0,0), coord_t(vec[0], vec[1], vec[2])) < treshold)
			{
				vectors.resize(idx);
				break;
			}		
		}

		//It seems there is duplicate treshold cutt-off....
	};

class covariance_field_t
{
	std::vector<covariance_t> m_data;	
	int m_xradius;
	int m_yradius;
	int m_zradius;
	int m_xdiameter;
	int m_ydiameter;
	int m_zdiameter;
	std::vector<sugarbox_vector_t> m_vectors;
	void init(int xradius, 
			int yradius, 
			int zradius, 
			const cov_model_t &);
public:
	covariance_field_t(
			int xradius, 
			int yradius, 
			int zradius, 
			const cov_model_t &);

	covariance_field_t(
			const sugarbox_search_ellipsoid_t & ellipsoid,
			const cov_model_t &);
		
	inline double value(int x, int y, int z)const;
	const std::vector<sugarbox_vector_t> & vectors()const
	{
		return m_vectors;
	};
	
	size_t size()
	{
		return m_data.size();
	}

	inline double operator()(const sugarbox_location_t  & loc1 , const sugarbox_location_t & loc2)const
	{
		sugarbox_vector_t vec = loc1 - loc2;
		if (vec[0] < - m_xradius || vec[0] > m_xradius 
			|| vec[1] < - m_yradius || vec[1] > m_yradius 
			|| vec[2] < - m_zradius || vec[2] > m_zradius )
			return 0;
		else
			return value(vec[0], vec[1], vec[2]);
	}
};

double
covariance_field_t::value(
	int x,
	int y,
	int z)const
{
	return m_data[vr_to_dec(m_ydiameter, m_xdiameter, z + m_zradius, y + m_yradius, x + m_xradius)];
	/*int xx = x + m_xradius; 
	int yy = y + m_yradius;
	int zz = z + m_zradius;

	int idx = zz * m_xdiameter * m_ydiameter + yy * m_xdiameter + xx;
	return m_data[idx];*/
}

} //namespace hpgl;

#endif //COVARIANCE_FIELD_H_INCLUDED_IN_BLUE_SKY_GSTL_AND_SOME_RANDOM_SYMBOLS_AFKJSDFKSJFKSDLKASDHKLWIUEWRWIWEIURH
