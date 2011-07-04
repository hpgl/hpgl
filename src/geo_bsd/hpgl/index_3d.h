#ifndef _INDEX_3D_H_SKLDJFLNMLKSMNLKEJFLKD7293847923479283HKANSKDNASKCBHANKSJn8237492374
#define _INDEX_3D_H_SKLDJFLNMLKSMNLKEJFLKD7293847923479283HKANSKDNASKCBHANKSJn8237492374

#include <math.h>

namespace hpgl
{
	struct index_3d_t
	{
		int m_coords[3];
		index_3d_t(){};
		index_3d_t(int x, int y, int z)
		{
			m_coords[0] = x;
			m_coords[1] = y;
			m_coords[2] = z;
		}
		inline int & operator[](int n)
		{
			return m_coords[n];
		}
		inline int operator[](int n)const
		{
			return m_coords[n];
		}
	};

	struct offset_3d_t
	{
		int m_vec[3];
		offset_3d_t(){}
		offset_3d_t(int x, int y, int z)
		{
			m_vec[0] = x;
			m_vec[1] = y;
			m_vec[2] = z;
		}
		inline int & operator[](int n)
		{
			return m_vec[n];
		}
		inline int operator[](int n)const
		{
			return m_vec[n];
		}
		inline float length()const
		{
			float result = 0;
			for (int i = 0; i < 3; ++i)
			{
				result += m_vec[i] * m_vec[i];
			}
			return sqrt(result);
		}
		inline bool operator<(const offset_3d_t & o)const
		{
			return this->length() < o.length();
		}
	};

	inline index_3d_t & operator += (index_3d_t & idx, const offset_3d_t & offset)
	{
		for (int i = 0; i < 3; ++i)
		{
			idx.m_coords[i] += offset.m_vec[i];
		}
		return idx;
	}

	inline index_3d_t operator+ (const index_3d_t & idx, const offset_3d_t & offset)
	{
		index_3d_t result = idx;
		result += offset;
		return result;
	}

	inline index_3d_t & operator -= (index_3d_t & idx, const offset_3d_t & offset)
	{
		for (int i = 0; i < 3; ++i)
		{
			idx.m_coords[i] -= offset.m_vec[i];
		}
		return idx;
	}

	inline index_3d_t operator-(const index_3d_t & idx, const offset_3d_t & offset)
	{
		index_3d_t result = idx;
		result -= offset;
		return result;
	}

	inline offset_3d_t operator-(const index_3d_t & idx1, const index_3d_t & idx2)
	{
		offset_3d_t result;
		for (int i = 0; i < 3; ++i)
		{
			result.m_vec[i] = idx1[i] - idx2[i];
		}
		return result;
	}
}

#endif /* _INDEX_3D_H_SKLDJFLNMLKSMNLKEJFLKD7293847923479283HKANSKDNASKCBHANKSJn8237492374 */
