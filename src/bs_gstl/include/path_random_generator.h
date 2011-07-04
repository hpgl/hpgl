/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PATH_RANDOM_GENERATOR_H__907328AB_FE6E_437B_9725_65FD1119284A__
#define __PATH_RANDOM_GENERATOR_H__907328AB_FE6E_437B_9725_65FD1119284A__

namespace hpgl
{
	class path_random_generator_t
	{
		class Impl;
		boost::shared_ptr<Impl> m_impl;
	public:
		path_random_generator_t(int size, int seed);
		~path_random_generator_t();

		int next();		
	};
}

#endif //__PATH_RANDOM_GENERATOR_H__907328AB_FE6E_437B_9725_65FD1119284A__