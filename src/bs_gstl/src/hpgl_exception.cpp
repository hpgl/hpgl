/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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