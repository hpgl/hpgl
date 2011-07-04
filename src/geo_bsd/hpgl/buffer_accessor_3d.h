#ifndef _BUFFER_ACCESSOR_3D_H_SADFKSDJFLWKJLSKJLKJFHSKJDCVJKSLK23423LKSADJf234ASKLSd
#define _BUFFER_ACCESSOR_3D_H_SADFKSDJFLWKJLSKJLKJFHSKJDCVJKSLK23423LKSADJf234ASKLSd

#include <hpgl_core.h>
#include "hpgl_exception.h"
#include <index_3d.h>

namespace hpgl
{

	struct data_shape_t
	{
		data_shape_t(){};
		data_shape_t(int dx, int dy, int dz)
		{
			m_data[0] = dx;
			m_data[1] = dy;
			m_data[2] = dz;
		}
		data_shape_t(int * dv)
		{
			for (int i = 0; i < 3; ++i)
				m_data[i] = dv[i];
		}
		int m_data[3];
		inline int & operator[](int idx){return m_data[idx];}
		inline int operator[](int idx)const {return m_data[idx];}	
		inline bool contains(const index_3d_t & idx)const
		{
			bool result = true;
			for (int i = 0; i < 3; ++i)
			{
				result &= idx[i] >= 0;
				result &= idx[i] < m_data[i];
			}
			return result;
		}
		inline int flat_size()const
		{
			int result = 1;
			for (int i = 0; i < 3; ++i)
				result *= m_data[i];
			return result;
		}
		void intersect(const data_shape_t & shape, data_shape_t & result)const
		{
			for (int i = 0; i < 3; ++i)
			{
				result[i] = std::min(shape[i], m_data[i]);
			}
		}
	};

	inline std::ostream & operator<<(std::ostream& os, const data_shape_t & shape)
	{
		os << "(" << shape[0] << ", " << shape[1] << ", " << shape[2] << ")";
		return os;
	}

	struct data_strides_t
	{
		int m_data[3];
		data_shape_t m_shape;
		int m_size;

		inline int & operator[](int idx){return m_data[idx];}
		inline int operator[](int idx)const {return m_data[idx];}

		inline const data_shape_t & get_shape()const {return m_shape;}
		inline int size()const {return m_size;}
		
		inline void from_shape(const data_shape_t & shape, const int * axes_order = 0)
		{
			int x = 1;
			int def_ao[] = {0, 1, 2};
			const int * ao = axes_order != 0 ? axes_order : def_ao;
			for (int i = 0; i < 3; ++i)
			{
				m_data[ao[i]] = x;
				x *= shape[i];
				m_shape[i] = shape[i];
			}
			m_size = x;
		}

		inline int get_flat_idx(const index_3d_t & idx3d)const
		{
			int result = 0;
			for (int i = 0; i < 3; ++i)
			{
				if (idx3d[i] < 0 || idx3d[i] >= m_shape[i])
					return -1;
				result += m_data[i] * idx3d[i];
			}
			return result;
		}

		inline void get_idx_3d(int flat_idx, index_3d_t & idx3d)const
		{
			for (int i = 0; i < 3; ++i)
			{
				idx3d[i] = (flat_idx / m_data[i])  % m_shape[i];
			}
		}
	};

	template<typename T>
	class buffer_accessor_3d_t
	{
		T * m_buffer;
		data_strides_t m_strides;
		int get_flat_index(const index_3d_t & idx)const
		{
			using namespace boost;
			int node = m_strides.get_flat_idx(idx);
			if (node < 0)
				throw hpgl_exception(
						"buffer_accessor_3d_t::get_flat_index", 
						format("Index is out of range: (%s, %s, %s). Shape: (%s, %s, %s)")
						% idx[0] % idx[1] % idx[2]
						% m_strides.get_shape()[0]
						% m_strides.get_shape()[1]
						% m_strides.get_shape()[2]);

			return node;
		}
	public:
		typedef T value_type;
		buffer_accessor_3d_t()
			: m_buffer(0)
		{}
		
		buffer_accessor_3d_t(T * buffer, const data_shape_t & shape)
		{
			set_buffer(buffer);
			set_shape(shape);
		}

		inline void set_buffer(T * buffer)
		{
			m_buffer = buffer;
		}

		inline void set_shape(const data_shape_t & shape)
		{
			m_strides.from_shape(shape);
		}

		inline const data_shape_t & get_shape()const {return m_strides.get_shape();}

		inline T & operator[](const index_3d_t & idx)
		{
			return m_buffer[get_flat_index(idx)];
		}

		inline const T operator[](const index_3d_t & idx)const
		{
			return m_buffer[get_flat_index(idx)];
		}

		inline const T& get_at(const index_3d_t & idx)const
		{
		  return m_buffer[get_flat_index(idx)];
		}
	};

	template <typename T>
	class masked_buffer_3d_t
	{
		T * m_buffer;
		unsigned char * m_mask;
		data_strides_t m_strides;
		int get_flat_index(const index_3d_t & idx)const
		{
			using namespace boost;
			int node = m_strides.get_flat_idx(idx);
			if (node < 0)
				throw hpgl_exception(
						"buffer_accessor_3d_t::get_flat_index", 
						format("Index is out of range: (%s, %s, %s). Shape: %s")
						% idx[0] % idx[1] % idx[2]
						% m_strides.get_shape());

			return node;
		}
	public:
		typedef T value_type;
		masked_buffer_3d_t()
		{}
		masked_buffer_3d_t(T * buffer, unsigned char * mask_buffer, const data_shape_t & shape)
		{
			set_buffers(buffer, mask_buffer);
			set_shape(shape);
		}

		inline void set_buffers(T * buffer, unsigned char * mask_buffer)
		{
			m_buffer = buffer;
			m_mask = mask_buffer;
		}

		inline void set_shape(const data_shape_t & shape)
		{
			m_strides.from_shape(shape);
		}

		inline const data_shape_t & get_shape()const
		{
			return m_strides.get_shape();
		}

		inline const T operator[](const index_3d_t & idx)const
		{
			return m_buffer[get_flat_index(idx)];
		}

		inline const T& get_at(const index_3d_t & idx)const
		{
			return m_buffer[get_flat_index(idx)];
		}

		inline void set_at(const index_3d_t & idx, T value)const
		{
			m_buffer[get_flat_index(idx)] = value;
			m_mask[get_flat_index(idx)] = 1;
		}

		inline bool is_informed(const index_3d_t & idx)const
		{
			return m_mask[get_flat_index(idx)] != 0;
		}

		inline void delete_value_at(const index_3d_t & idx)
		{
			m_mask[get_flat_index(idx)] = 0;
		}
	};

	typedef masked_buffer_3d_t<indicator_value_t> indicator_masked_buffer_3d_t;
	typedef masked_buffer_3d_t<cont_value_t> cont_masked_buffer_3d_t;
}

#endif /* _BUFFER_ACCESSOR_3D_H_SADFKSDJFLWKJLSKJLKJFHSKJDCVJKSLK23423LKSADJf234ASKLSd */
