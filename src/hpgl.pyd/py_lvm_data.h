/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB
#define __PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB

#include "lvm_data.h"

namespace hpgl
{
	class py_indicator_lvm_data_t
	{
	public:
		boost::shared_ptr<indicator_lvm_data_t> m_lvm_data;
	};

	py_indicator_lvm_data_t py_load_indicator_mean_data(PyObject * filenames);
}

#endif //__PY_LVM_DATA_H__9F38BC53_A2BF_4442_BC6A_27ED517DA7DB