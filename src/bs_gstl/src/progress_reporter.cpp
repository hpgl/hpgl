/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#include "stdafx.h"

#include <progress_reporter.h>

int progress_bar_length = 20;

void print_progressbar(int percent)
{
	std::cout << "\r[";
	for (int i = 0; i < 20; ++i)
	{
		if (percent / 100.0 > i / 20.0)
			std::cout << "*";
		else
			std::cout << " ";
	}
	std::cout << "] " << percent << "%                ";
	std::cout.flush();
}


namespace hpgl
{
	void progress_reporter_t::set_iteration_count(long iteration_count)
	{
		m_iterations = iteration_count;
		m_counter = 0;
		m_delta = m_iterations / 10;
	}

	progress_reporter_t::progress_reporter_t(long n_iterations)
	{
		set_iteration_count(n_iterations);		
	}

	void progress_reporter_t::start()
	{
		std::cout << "\n";
		//std::cout << |--------------------|
		//print_progressbar(0);
		time(&m_start);
	}

	void progress_reporter_t::start(long n_iterations)
	{
		set_iteration_count(n_iterations);		
		start();
	}

	void progress_reporter_t::next_lap()
	{
		//BOOST_INTERLOCKED_INCREMENT(&m_counter);
		m_counter++;
		if (m_counter % m_delta == 0)
		{
		//	boost::mutex::scoped_lock lock(m_mutex);
			std::cout << (int) 100.0 * m_counter / m_iterations << "%... ";
			std::cout.flush();
			//print_progressbar(m_counter * 100 / m_iterations);
		}		
	}

	void progress_reporter_t::stop()
	{
		time(&m_end);
		std::cout << std::endl;
		//print_progressbar(100);
	}

	double progress_reporter_t::iterations_per_second()
	{
		return m_iterations / difftime(m_end, m_start);
	}

	double progress_reporter_t::duration()
	{
		return difftime(m_end, m_start);
	}
}
