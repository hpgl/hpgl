/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef PROGRESS_REPORTER_H_INCLUDED_ALKSJSHDKASHDJKASBNJDFHNGKHDFKLSHFJKSDGJHGVDGLJKHVJGKKGJ
#define PROGRESS_REPORTER_H_INCLUDED_ALKSJSHDKASHDJKASBNJDFHNGKHDFKLSHFJKSDGJHGVDGLJKHVJGKKGJ

namespace hpgl
{
	//TODOTODO: find better name for class
	class progress_reporter_t
	{
		long m_counter;
		long m_delta;
		long m_iterations;
		time_t m_start;
		time_t m_end;
		boost::mutex m_mutex;
		void set_iteration_count(long iteration_count);
	public:
		progress_reporter_t(long n_iterations);
		void start();
		void start(long n_iterations);
		void next_lap();
		void stop();
		double iterations_per_second();
		double duration();
	};
}

#endif //PROGRESS_REPORTER_H_INCLUDED_ALKSJSHDKASHDJKASBNJDFHNGKHDFKLSHFJKSDGJHGVDGLJKHVJGKKGJ
