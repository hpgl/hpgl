/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/



#ifdef HPGL_LOG_ON
#define HPGL_LOG_SYSTEM(A, b, size) log_system(A, b, size)
#define HPGL_LOG_SYSTEM_SOLUTION(res, x, size) log_system_solution(res, x, size)
#define HPGL_LOG_NEIGHBOURS(center, neib) log_neighbours(center, neib)
#define HPGL_LOG_STRING(msg) log_string(msg)
#else
#define HPGL_LOG_SYSTEM(A, b, size) {}
#define HPGL_LOG_SYSTEM_SOLUTION(res, x, size) {}
#define HPGL_LOG_NEIGHBOURS(center, neib) {}
#define HPGL_LOG_STRING(msg) {}
#endif

namespace hpgl
{
	template<typename coord_t>
    void log_neighbours(const coord_t & center, const std::vector<coord_t> & neighbours)
	{
		std::cout << "LOG: (";
		for (int i = 0; i < 2; ++i)
		{
			std::cout << center[i] << ", ";
		}
		std::cout << center[2] << ")\n";
		for (int i = 0; i < neighbours.size() ; ++i)
		{
			std::cout << "LOG: \t(";
			for (int j = 0; j < 2; ++j)
				std::cout << neighbours[i][j] << ", ";
			std::cout << neighbours[i][2] << "]\n";
		}
	}

	void log_system(double * A, double *b, int size);
	void log_system_solution(bool system_solved, double * x, int size);
	void log_string(const char * message);

}
