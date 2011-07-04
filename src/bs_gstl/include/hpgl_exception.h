/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef HPGL_EXCEPTION_H_INCLUDED_3904FHNEWC94323VM0349MNP0ENAPW934RNVPSEVRVQW5434CV54VG34
#define HPGL_EXCEPTION_H_INCLUDED_3904FHNEWC94323VM0349MNP0ENAPW934RNVPSEVRVQW5434CV54VG34

#include <stdexcept>
#include <boost/format.hpp>

namespace hpgl
{
	class hpgl_exception : public std::exception
	{
		std::string m_where;
		std::string m_what;
		std::string m_message;
	public:
		hpgl_exception(const std::string & a_where, const std::string & what);
		hpgl_exception(const std::string & a_where, const boost::format & what);

		virtual const char* what()const throw()
		{
			return m_message.c_str();
		}
		
		~hpgl_exception() throw() {}
	};
}

#endif //HPGL_EXCEPTION_H_INCLUDED_3904FHNEWC94323VM0349MNP0ENAPW934RNVPSEVRVQW5434CV54VG34
