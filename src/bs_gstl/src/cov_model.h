/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef COV_MODEL_H_INCLUDED_IN_HPGL_SDFKSJHKJ234SDF234JSDFIW29834KJNFHDAJK234NK32JNFDSAF234
#define COV_MODEL_H_INCLUDED_IN_HPGL_SDFKSJHKJ234SDF234JSDFIW29834KJNFHDAJK234NK32JNFDSAF234

#include "covariance_param.h"

namespace hpgl
{
	const double pi = 3.14159265358979323846264338327950288419;
	
	inline double grad_to_rad(double grad)
	{
		return ( grad*pi ) / 180;
	}

	template<typename ranges_t, typename angles_t>
	void create_transform(const ranges_t & ranges, const angles_t & angles, std::vector<double> & result)
	{
		using namespace TNT;
		result.resize(9);

		for (int i = 0; i < 3; ++i)
		{
			if (ranges[i] == 0)
				throw hpgl_exception("create_transform", "All covariance ranges should be non-zero");
		}

		Matrix<double> scale(3,3,0.0);
		Matrix<double> rotate_z(3,3,0.0);
		Matrix<double> rotate_y(3,3,0.0);
		Matrix<double> rotate_x(3,3,0.0);

		scale(1, 1) = 1.0;
		scale(2, 2) = ranges[0]/ranges[1];
		scale(3, 3) = ranges[0]/ranges[2];

		rotate_z(1, 1) = cos(grad_to_rad(angles[0]));
		rotate_z(2, 2) = cos(grad_to_rad(angles[0]));
		rotate_z(1, 2) = sin(grad_to_rad(angles[0]));
		rotate_z(2, 1) = -sin(grad_to_rad(angles[0]));
		rotate_z(3, 3) = 1;

		rotate_y(1, 1) =  cos(grad_to_rad(angles[1]));
		rotate_y(3, 3) =  cos(grad_to_rad(angles[1]));
		rotate_y(1, 3) = -sin(grad_to_rad(angles[1]));
		rotate_y(3, 1) =  sin(grad_to_rad(angles[1]));
		rotate_y(2, 2) =  1;  
  
		rotate_x(2, 2) =  cos(grad_to_rad(angles[2]));
		rotate_x(3, 3) =  cos(grad_to_rad(angles[2]));
		rotate_x(2, 3) =  sin(grad_to_rad(angles[2]));
		rotate_x(3, 2) = -sin(grad_to_rad(angles[2]));
		rotate_x(1, 1) = 1;

		Matrix<double> t = scale*((rotate_x * rotate_y) * rotate_z);
		for (int i = 0; i < 3; ++i)
			for (int j = 0; j < 3; ++j)
			{
				result[3*i+j] = t(i+1, j+1);
			}
	}

	template<typename vec_t>
	double transfrom_and_norm(const vec_t & vec, const std::vector<double> & transform)
	{
		double result = 0.0;
		for (int i = 0; i < 3; ++i)
		{
			double d = 0.0;
			for (int j = 0; j < 3; ++j)
			{
				d += vec[j] * transform[3*i + j];
			}
			result += d*d;
		}
		return sqrt(result);
	}

	class cov_model_t
	{
		covariance_param_t m_params;

		double (cov_model_t::*fun)(double)const;

		std::vector<double> m_transform;

		void init_fun()
		{
			switch (m_params.m_covariance_type)
			{
			case COV_SPHERICAL:
				fun = &cov_model_t::spherical;
				break;
			case COV_EXPONENTIAL:
				fun = &cov_model_t::exponential;
				break;
			case COV_GAUSSIAN:
				fun = &cov_model_t::gaussian;
				break;
			}
			
		}
	public:

		cov_model_t(const covariance_param_t & params)
		{
			m_params = params;
			init_fun();
			create_transform(params.m_ranges, params.m_angles, m_transform);
		}

		template<typename ranges_t, typename angles_t>
		cov_model_t(covariance_type_t cov_type, const ranges_t & ranges,
			const angles_t & angles, double sill, double nugget)
		{
			m_params.m_covariance_type = cov_type;
			m_params.set_ranges(ranges[0], ranges[1], ranges[2]);
			m_params.set_angles(angles[1], angles[2], angles[3]);
			m_params.m_sill = sill;
			m_params.m_nugget = nugget;
			init_fun();
			create_transform(m_params.m_ranges, m_params.m_angles, m_transform);
		}
		

		double operator()(double h)const
		{
			return ((*this).*fun)(h);
		}

		template<typename coord_t>
		double operator()(const coord_t & c1, const coord_t & c2)const
		{
			double v[3];
			v[0] = c1[0] - c2[0];
			v[1] = c1[1] - c2[1];
			v[2] = c1[2] - c2[2];
			
			//double h = sqrt(v0 * v0 + v1 * v1 + v2 * v2);
			double h = transfrom_and_norm(v, m_transform);
			return this->operator()(h);
		}
		
		double gaussian(double h)const
		{
			if(h < 0.0001)
			{
				return m_params.m_sill;
			}
			else
			{
				return (m_params.m_sill - m_params.m_nugget) * exp(-3 * pow(h / m_params.m_ranges[0], 2));
			}
		}

		double exponential(double h)const
		{
			if(h < 0.0001)
			{
				return m_params.m_sill;
			}
			else
			{
				return (m_params.m_sill - m_params.m_nugget) * exp(-3 * h / m_params.m_ranges[0]);			
			}
		}

		double spherical(double h)const
		{
			if(h < 0.0001)
			{
				return m_params.m_sill;
			}
			else
			{
				if (h > m_params.m_ranges[0])
					return 0;
				return (m_params.m_sill - m_params.m_nugget) * (1 - 1.5 * h / m_params.m_ranges[0] + 0.5 * pow(h / m_params.m_ranges[0], 3));
			}
		}
	};
}

#endif
