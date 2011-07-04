#include "stdafx.h"
#include "hpgl_exception.h"

namespace hpgl
{
	hpgl_exception::hpgl_exception(
		const std::string & a_where, 
		const std::string & what)
			:m_where(a_where), m_what(what)
		{
			m_message = m_where + ": " + m_what;
			//std::cerr << m_message << std::endl;
		}

	hpgl_exception::hpgl_exception(
		const std::string & a_where, 
		const boost::format & what)
			:m_where(a_where), m_what(what.str())
		{
			m_message = m_where + ": " + m_what;
			//std::cerr << m_message << std::endl;
		}

}
