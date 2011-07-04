/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


#ifndef __NON_PARAMETRIC_CDF_H__6BBB60EA_BA86_4AE3_98FF_3B287C1D1522____
#define __NON_PARAMETRIC_CDF_H__6BBB60EA_BA86_4AE3_98FF_3B287C1D1522____

namespace hpgl
{
	template<typename value_t, typename prob_t>
	class non_parametric_cdf_t
	{		
		typedef std::vector<std::pair<value_t, prob_t> > cdf_t;
		cdf_t m_cdf;

		struct inverse_less
		{
			template<typename pair1, typename pair2>
			bool operator()(const pair1 & p1, const pair2 & p2)
			{
				return p1.second < p2.second;
			}
		};
		
	public:
		
		template <typename T>
		non_parametric_cdf_t(const T & data)
		{
			using namespace std;
			typedef map<value_t, int> occ_map_t;
			typedef typename occ_map_t::iterator occ_iterator_t;

			occ_map_t occ;
			int size = data.size();
			int count = 0;
			for (int i = 0; i < size; ++i)
			{
				if (data.is_informed(i))
				{
					pair<occ_iterator_t, bool> result = occ.insert(make_pair(data[i], 1));
					if (!result.second)
					{
						result.first->second += 1;
					}
					++count;
				}
			}		

			if (count <= 0)
				return;

			assert(occ.size() > 0);

			vector<pair<value_t, prob_t> > pdf;
			pdf.reserve(occ.size());

			for (occ_iterator_t it = occ.begin(), it_end = occ.end(); it != it_end; ++it)
			{
				pdf.push_back(make_pair(it->first, double(it->second) / double(count)));
			}
			std::sort(pdf.begin(), pdf.end());

			m_cdf.reserve(pdf.size());
			m_cdf.push_back(pdf[0]);
			for (int i = 1; i < pdf.size()-1; ++i)
			{				
				m_cdf.push_back(make_pair(pdf[i].first, pdf[i].second + m_cdf[i-1].second));
			}			
		}

		void dump()
		{
			std::cout << "Non-parametric CDF. Size: " << m_cdf.size() << "\n";
			for (int i = 0; i < m_cdf.size(); ++i)
			{
				std::cout << "(" << m_cdf[i].first << "," << m_cdf[i].second << ");";
			}
			std::cout << std::endl;
		}

		prob_t prob(value_t value)
		{
			return (*this)(value);
		}
		
		prob_t operator()(value_t value)
		{
			using namespace std;
			prob_t result;
			typename cdf_t::iterator it = lower_bound(m_cdf.begin(), m_cdf.end(), make_pair(value, 1.0));
			if (it == m_cdf.end())
			{
				result = m_cdf.back().second;
			}
			else if (it == m_cdf.begin())
			{
				result = m_cdf.front().second;
			}
			else
			{
				typename cdf_t::iterator it2 = it - 1;
				value_t x1 = it2->first;
				value_t x2 = it->first;
				prob_t y1 = it2->second;
				prob_t y2 = it->second;
				result = y1 + (double)(y2 - y1) / (double)(x2 - x1) * (value - x1);
			}			
			return result;
		}

		value_t inverse(prob_t prob)
		{
			using namespace std;

			if (m_cdf.size() <= 0)
				return 0;

			value_t result;

			typename cdf_t::iterator it = lower_bound(m_cdf.begin(), m_cdf.end(), make_pair(m_cdf.back().first, prob), inverse_less());
			if (it == m_cdf.end())
				result = m_cdf.back().first;
			else if (it == m_cdf.begin())
				result = m_cdf.front().first;
			else
			{
				typename cdf_t::iterator it2 = it - 1;
				value_t x1 = it2->first;
				value_t x2 = it->first;
				prob_t y1 = it2->second;
				prob_t y2 = it->second;
				result = x1 + (double)(x2 - x1) / (double)(y2 - y1) * (prob - y1);				
			}			
			return result;
		}
	};

	
}

#endif //__NON_PARAMETRIC_CDF_H__6BBB60EA_BA86_4AE3_98FF_3B287C1D1522____
