/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A
#define __EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A

namespace hpgl
{
	boost::shared_ptr<indicator_lvm_data_t> extract_lvm_data(boost::python::object & mean_data, int indicator_count);
}

#endif //__EXTRACT_LVM_DATA_H__9ABD102B_8963_4102_B40B_7BD38DCA6D2A