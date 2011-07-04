#ifndef __SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498
#define __SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498

namespace hpgl
{
	template<typename source_t, typename indices_t, typename dest_t>
	inline void select(const source_t & src, const indices_t & indices, dest_t & dest)
	{
		dest.resize(indices.size());
		for (size_t i = 0, end_i = indices.size(); i < end_i; ++i)
		{
			dest[i] = src[indices[i]];
		}
	}
}

#endif //__SELECT_H__D393AF18_8E49_4668_8C3F_DCE536469498