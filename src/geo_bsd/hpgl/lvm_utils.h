#ifndef __LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028
#define __LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028

namespace hpgl
{
	template<typename data_t, typename lvm_t>
	void subtract_means(data_t & prop, const lvm_t means)
	{
		for (typename data_t::size_type idx = 0, end_idx = prop.size(); idx < end_idx; ++idx)
		{
			if (prop.is_informed(idx))
				prop.set_at(idx, prop[idx] - means[idx]);
		}
	}

	template<typename data_t, typename lvm_t>
	void add_means(data_t & prop, const lvm_t means)
	{
		for (typename data_t::size_type idx = 0; idx < prop.size(); ++idx)
		{
			if (prop.is_informed(idx))
				prop.set_at(idx, prop[idx] + means[idx]);
		}
	}
}

#endif //__LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028