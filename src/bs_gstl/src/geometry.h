/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __GEOMETRY_H__ABCDF13E_0728_4451_B0B5_7FE7F5FEDFEA
#define __GEOMETRY_H__ABCDF13E_0728_4451_B0B5_7FE7F5FEDFEA

namespace hpgl
{
	template <typename T>
	struct line_section_t
	{
		typedef T coord_t;
		T c1; 
		T c2;
		
		line_section_t()
			: c1(0), c2(0)
		{}

		line_section_t(T c1, T c2)
			: c1(c1), c2(c2)
		{}

		inline bool has(T c)const
		{
			return (c1 <= c) && (c <= c2);
		}

		template<typename delta_t>
		inline line_section_t<T> offset(delta_t delta)const
		{
			return line_section_t<T>(c1 + delta, c2 + delta);
		}

		T length()const
		{
			return c2 - c1;
		}
		T length_inclusive()const
		{
			return c2 - c1 + 1;
		}
	};

	template<typename T>
	line_section_t<T> intersect(const line_section_t<T> & ls1, const line_section_t<T> & ls2)
	{
		line_section_t<T> result(std::max(ls1.c1, ls2.c1), std::min(ls1.c2, ls2.c2));
		if (result.c1 < result.c2)
			return result;
		else
			return line_section_t<T>(0, 0);
	}

	template<typename T>
	line_section_t<T> overlap(const line_section_t<T> & ls1, const line_section_t<T> & ls2)
	{
		return line_section_t<T>(std::min(ls1.c1, ls2.c1), std::max(ls1.c2, ls2.c2));
	}

	template <typename T>
	struct rect_3d_t
	{
		typedef line_section_t<T> section_t;
		line_section_t<T> x;
		line_section_t<T> y;
		line_section_t<T> z;		

		rect_3d_t()
		{}
		
		rect_3d_t(T x1, T y1, T z1, T x2, T y2, T z2)
			: x(x1, x2), y(y1,y2), z(z1,z2)
		{}

		template<typename point_t1, typename point_t2>
		rect_3d_t(const point_t1 & p1, const point_t2 & p2)
			: x(p1[0], p2[0]), y(p1[1], p2[1]), z(p1[2], p2[2])
		{}

		rect_3d_t(const line_section_t<T> & x, const line_section_t<T> & y, const  line_section_t<T> & z)
			: x(x), y(y), z(z)
		{}

		inline bool has(T cx, T cy, T cz)const
		{
			return x.has(cx) && y.has(cy) && z.has(cz);
		}		

		template <typename point_t>
		inline bool has(const point_t & point)const
		{
			return has(point[0], point[1], point[2]);
		}

		template<typename vector_t>
		inline rect_3d_t<T> offset(const vector_t & vec)const
		{
			return rect_3d_t<T>(x.offset(vec[0]), y.offset(vec[1]), z.offset(vec[2]));
		}

		inline T volume()const
		{
			return x.length() * y.length() * z.length();
		}

		inline T volume_inclusive()const
		{
			return x.length_inclusive() * y.length_inclusive() * z.length_inclusive();
		}
	};

	template<typename T>
	rect_3d_t<T> intersect(const rect_3d_t<T> & rect1, const rect_3d_t<T> & rect2)
	{
		return rect_3d_t<T>(intersect(rect1.x, rect2.x), intersect(rect1.y, rect2.y), intersect(rect1.z, rect2.z));
	}		

	template<typename T>
	rect_3d_t<T> bounding_box(const rect_3d_t<T> & rect1, const rect_3d_t<T> & rect2)
	{
		return rect_3d_t<T>(overlap(rect1.x, rect2.x), overlap(rect1.y, rect2.y), overlap(rect1.z, rect2.z));
	}	

	template<typename iterator_t, typename T>
	rect_3d_t<T> bounding_box_from_zerobased_rects(iterator_t it1, iterator_t it2)
	{
		iterator_t it = it1;
		int z[] = {0,0,0};
		rect_3d_t<T> result(z, *it);
		while (++it != it2)
		{
			result = bounding_box(result, rect_3d_t<T>(z, *it));
		}
		return result;
	}
	
}

#endif //__GEOMETRY_H__ABCDF13E_0728_4451_B0B5_7FE7F5FEDFEA
