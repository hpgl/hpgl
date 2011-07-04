/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"
#include <hpgl_exception.h>

namespace hpgl
{
	hpgl_exception::hpgl_exception(
		const std::string & a_where, 
		const std::string & what)
			:m_where(a_where), m_what(what)
		{
			m_message = m_where + ": " + m_what;
			std::cerr << m_message << std::endl;
		}

	hpgl_exception::hpgl_exception(
		const std::string & a_where, 
		const boost::format & what)
			:m_where(a_where), m_what(what.str())
		{
			m_message = m_where + ": " + m_what;
			std::cerr << m_message << std::endl;
		}

}