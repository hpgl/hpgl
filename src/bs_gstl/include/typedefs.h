/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __TYPEDEFS_H__A2E48559_1FA0_4196_BC88_F018A3AE1162__
#define __TYPEDEFS_H__A2E48559_1FA0_4196_BC88_F018A3AE1162__

#include <boost/smart_ptr.hpp>

namespace hpgl
{
	template<typename T>
	struct vec_t
	{
		T c[3];

		vec_t()
		{
			c[0] = 0;
			c[1] = 0;
			c[2] = 0;
		}

		vec_t(T x, T y, T z)
		{
			c[0] = x;
			c[1] = y;
			c[2] = z;
		}

		T & operator[](int idx)
		{
			return c[idx];
		}

		T operator[](int idx)const
		{
			return c[idx];
		}

		template<typename T2>
		bool operator==(const vec_t<T2> & t)
		{
			return c[0] == t[0] && c[1] == t[1] && c[2] == t[2];
		}
	};

	template<typename T>
	struct loc_t
	{
		T c[3];
		typedef vec_t<T> difference_type;

		loc_t()
		{
			c[0] = 0;
			c[1] = 0;
			c[2] = 0;
		}

		loc_t(T x, T y, T z)
		{
			c[0] = x;
			c[1] = y;
			c[2] = z;
		}

		T & operator[](int idx)
		{
			return c[idx];
		}

		T operator[](int idx)const
		{
			return c[idx];
		}

		template<typename T2>
		bool operator==(const loc_t<T2> & t)
		{
			return c[0] == t[0] && c[1] == t[1] && c[2] == t[2];
		}
	};

	template<typename T>
	inline vec_t<T> operator-(const loc_t<T> & loc1, const loc_t<T> & loc2)
	{
		vec_t<T> result;
		for (int i = 0; i < 3; ++i)
		{
			result.c[i] = loc1.c[i] - loc2.c[i];
		}
		return result;
	}

	template<typename T>
	inline loc_t<T> operator+(const loc_t<T> & loc1, const vec_t<T> & vec2)
	{
		loc_t<T> result;
		for (int i = 0; i < 3; ++i)
		{
			result.c[i] = loc1.c[i] + vec2.c[i];
		}
		return result;
	}

	typedef loc_t<double> real_location_t; 

	typedef loc_t<int> sugarbox_location_t; 
	
	typedef vec_t<int> sugarbox_vector_t; 


	typedef int bs_node_index_t;
	typedef bs_node_index_t node_index_t;
	typedef int sugarbox_grid_size_t;

	typedef unsigned char indicator_value_t;
	typedef unsigned char indicator_index_t;
	typedef float indicator_probability_t;
	typedef float cont_value_t;
	typedef double kriging_weight_t;
	typedef double kriging_variance_t;
	typedef float mean_t;
	typedef double covariance_t;
	typedef double ellipse_size_t;
	typedef double angle_t;
	typedef double range_t;

	template<typename T>
	struct xyz_tuple_t
	{
		enum {size = 3};
	private:
		T m_data[size];
	public:		
		xyz_tuple_t(){}		

		xyz_tuple_t(T t1, T t2, T t3)
		{
			m_data[0] = t1;
			m_data[1] = t2;
			m_data[2] = t3;
		}

		inline T & x() {return m_data[0];}
		inline T & y() {return m_data[1];}
		inline T & z() {return m_data[2];}
		inline T & operator[](size_t idx)
		{
			assert(idx < 3);
			return m_data[idx];
		}

		inline const T & x() const {return m_data[0];} 
		inline const T & y() const {return m_data[1];}
		inline const T & z() const {return m_data[2];}
		inline const T & operator[](size_t idx)const
		{
			assert(idx < 3);
			return m_data[idx];
		}
	};

	template<typename T>
	T length(const xyz_tuple_t<T> & t)
	{
		T sum = 0;
		for (size_t i = 0; i < xyz_tuple_t<T>::size; ++i)
			sum +=  t[i] * t[i];
		return sqrt(sum);
	}

	template<typename T>
	xyz_tuple_t<T> operator-(const xyz_tuple_t<T> &t1, const xyz_tuple_t<T> & t2)
	{
		return xyz_tuple_t<T>(t1.x() - t2.x(), t1.y() - t2.y(), t1.z() - t2.z());
	}

	template<typename T>	
	std::ostream & 
	operator<<(std::ostream & stream, const xyz_tuple_t<T> & tuple)
	{
		return (stream << "(" << tuple.x() << ", " << tuple.y() << ", "  << tuple.z() << ")");
	}

	typedef xyz_tuple_t<double> coord_t;
	typedef xyz_tuple_t<double> search_area_t;	

	typedef xyz_tuple_t<angle_t> angles_t;	
	typedef xyz_tuple_t<range_t> ranges_t;
	
	typedef std::vector<node_index_t> enum_path_t;
	typedef std::vector<node_index_t> reverse_path_t;
	typedef size_t path_index_t;	

	enum kriging_constraint_kind_t
	{
		OK_CONSTRAINTS,
		SK_CONSTRAINTS
	};

	enum mean_kind_t
	{
		e_mean_stationary_auto,
		e_mean_stationary,
		e_mean_varying
	};
}


#endif //__TYPEDEFS_H__A2E48559_1FA0_4196_BC88_F018A3AE1162__
