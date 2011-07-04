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
		void set_iteration_count(long iteration_count);
	public:
		progress_reporter_t(long n_iterations);
		void start();
		void start(long n_iterations);
		void next_lap();
		void next_lap(int lap_count)
		{
			for (int i = 0; i < lap_count; ++i)
				next_lap();
		}
		void stop();
		double iterations_per_second();
		double duration();
	};
}

#endif //PROGRESS_REPORTER_H_INCLUDED_ALKSJSHDKASHDJKASBNJDFHNGKHDFKLSHFJKSDGJHGVDGLJKHVJGKKGJ
