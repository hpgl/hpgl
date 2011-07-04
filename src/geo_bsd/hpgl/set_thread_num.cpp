#include "stdafx.h"
#include <omp.h>

namespace hpgl
{
	void set_thread_num(int n_threads)
	{
		omp_set_num_threads(n_threads);
	}

	int get_thread_num()
	{
		return omp_get_max_threads();
	}
}
