/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

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
