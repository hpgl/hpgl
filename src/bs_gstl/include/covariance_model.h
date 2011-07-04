/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7
#define __COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7

namespace hpgl
{
	class covariance_model_t
	{
	public:
		virtual covariance_t operator()(coord_t, coord_t)const = 0;
	};
}

#endif //__COVARIANCE_MODEL_H__2CCE338A_32EE_4CD0_AABC_C4BDA1C5A4F7