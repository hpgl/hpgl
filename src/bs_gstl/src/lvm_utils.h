/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028
#define __LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028

namespace hpgl
{
	template<typename data_t, typename lvm_t>
	void subtract_means(data_t & prop, const lvm_t & means)
	{
		if (means.size() != prop.size())
				throw hpgl_exception("subtract_means", boost::format("Data size: %s. Mean data size: %s. Must be equal.") % prop.size() % means.size());
		for (size_t idx = 0, end_idx = prop.size(); idx < end_idx; ++idx)
		{
			if (prop.is_informed(idx))
				prop.set_at(idx, prop[idx] - means[idx]);
		}
	}

	template<typename data_t, typename lvm_t>
	void add_means(data_t & prop, const lvm_t & means)
	{
		if (means.size() != prop.size())
				throw hpgl_exception("add_means", boost::format("Data size: %s. Mean data size: %s. Must be equal.") % prop.size() % means.size());
		for (size_t idx = 0; idx < prop.size(); ++idx)
		{
			if (prop.is_informed(idx))
				prop.set_at(idx, prop[idx] + means[idx]);
		}
	}
}

#endif //__LVM_UTILS_H__C3B5899D_F1AA_4F6A_AC6F_0D0568359028