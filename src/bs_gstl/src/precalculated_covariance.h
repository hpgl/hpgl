/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __PRECALCULATED_COVARIANCE_H__8E9E179D_F626_4EEF_8096_9F075A40B740
#define __PRECALCULATED_COVARIANCE_H__8E9E179D_F626_4EEF_8096_9F075A40B740

#include "typedefs.h"
#include "geometry.h"
#include "var_radix_utils.h"

namespace hpgl
{
	/*
	Implements CovarianceModel concept.
	*/	
	class precalculated_covariances_t
	{
		int dx;
		int dy;
		int dz;
		int sx;
		int sy;
		int sz;
		std::vector<covariance_t> m_covariances;
		rect_3d_t<int> m_box;
	public:		
		precalculated_covariances_t()
		{}

		template<typename covariances_t, typename radiuses_t>
		precalculated_covariances_t(const covariances_t & cov, const radiuses_t & rs)
		{
			init(cov, rs);			
		}

		template<typename covariances_t, typename radiuses_t>
		void init(const covariances_t & cov, const radiuses_t & rs)
		{			
			int rx = rs[0];
			int ry = rs[1];
			int rz = rs[2];
			dx = - rx;
			dy = - ry;
			dz = - rz;
			sx = rx * 2 + 1;
			sy = ry * 2 + 1;
			sz = rz * 2 + 1;
			size_t size = sx * sy * sz;	
			m_covariances.resize(size);
			for (int z = 0; z < sz; ++z)
				for (int y = 0; y < sy; ++y)
					for (int x = 0; x < sx; ++x)
					{
						int index = vr_to_dec(sy, sx, z, y, x);
						double c1[] = {0, 0, 0};
						double c2[] = {x + dx, y + dy, z + dz};
						m_covariances[index] = cov(c1, c2);
					}
			m_box = rect_3d_t<int>(-rx, -ry, -rz, rx, ry, rz);
			int vol = m_box.volume_inclusive();
			assert(vol == size);
		}

		template<typename coord_t>
		covariance_t operator()(const coord_t & c1, const coord_t & c2)const
		{
			double vec[] = {c2[0] - c1[0], c2[1] - c1[1], c2[2] - c1[2]}; //c2 - c1;

			if (m_box.has(vec))
			{
				int index = vr_to_dec(sy, sx, vec[2] - dz, vec[1] - dy, vec[0] - dx);
				return m_covariances[index];
			}
			else
			{
				return 0;
			}

		}
	};
}

#endif //__PRECALCULATED_COVARIANCE_H__8E9E179D_F626_4EEF_8096_9F075A40B740
