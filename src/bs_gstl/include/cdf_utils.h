/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef CDF_UTILS_H_INCLUDED_ASDJASLD2134791287391KAJSHDKLASHD298237912837HIUREYR8732RH3R23
#define CDF_UTILS_H_INCLUDED_ASDJASLD2134791287391KAJSHDKLASHD298237912837HIUREYR8732RH3R23

#include <typedefs.h>
#include <property_array.h>

namespace hpgl
{
	indicator_index_t 
	most_probable_category(
		const std::vector<indicator_probability_t> & probs);

	template <typename prop_t, typename Cdf1, typename Cdf2>
	void transform_cdf_p(prop_t & property, Cdf1 from, Cdf2 to)
	{	
		for (int idx = 0, end_idx = property.size(); idx < end_idx; ++idx)
		{
			if (property.is_informed(idx))
			{
				double P = from.prob(property.get_at(idx));
				double value = to.inverse(P);
				property.set_at(idx, value);
			}
		}	
	}

	template <typename T, typename Cdf1, typename Cdf2>
	T transform_cdf_s(T value, Cdf1 from, Cdf2 to)
	{	
		return to.inverse(from.prob(value));			
	}

	template <typename T, typename Cdf1, typename Cdf2>
	void transform_cdf_v(const std::vector<T> & in, std::vector<T> & out, Cdf1 from, Cdf2 to)
	{
		out.resize(in.size());
		for (size_t idx = 0, end_idx = in.size(); idx < end_idx; ++idx)
		{
			out[idx] = to.inverse(from.prob(in[idx]));
		}
	}	

	template <typename T, typename Cdf1, typename Cdf2>
	void transform_cdf_ptr(const T * in, std::vector<T> & out, Cdf1 from, Cdf2 to)
	{
		for (size_t idx = 0, end_idx = out.size(); idx < end_idx; ++idx)
		{
			out[idx] = to.inverse(from.prob(in[idx]));
		}
	}	


} //namespace hpgl

#endif // CDF_UTILS_H_INCLUDED_ASDJASLD2134791287391KAJSHDKLASHD298237912837HIUREYR8732RH3R23
