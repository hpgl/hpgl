#include "stdafx.h"

#include "pretty_printer.h"
#include "ok_params.h"
#include "sk_params.h"
#include "ik_params.h"
#include "sgs_params.h"
#include "hpgl_core.h"

#include "output.h"

namespace hpgl
{
	void print_algo_name(const std::string & name)
	{
		write(boost::format("---------- Starting %1% ----------\n") % name);
		//		std::cout << "---------- Starting " << name << " ----------\n";
	}

	
	
	void print_param (const std::string & param, const std::vector<double> & value)
	{
		write(boost::format("%1%: [%2%, %3%, %4%]\n") % param % value[0] % value[1] % value[2]);
	}

	void print_param (const std::string & param, const double * value)
	{
		write(boost::format("%1%: [%2%, %3%, %4%]\n") % param % value[0] % value[1] % value[2]);
	}

	void print_params(const neighbourhood_param_t & p)
	{
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
	}

	void print_params(const covariance_param_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Nugget", p.m_nugget);
		print_param("Ranges", p.m_ranges);
		print_param("Angles", p.m_angles);		
	}

	void print_params(const ok_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Nugget", p.m_nugget);
		print_param("Ranges", p.m_ranges);
		print_param("Angles", p.m_angles);	
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
	}

	void print_params(const sk_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Nugget", p.m_nugget);
		print_param("Ranges", p.m_ranges);
		print_param("Angles", p.m_angles);	
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
		if (p.m_calculate_mean)
			print_param("Mean", "automatic");
		else
			print_param("Mean", p.mean());
	}
	void print_params(const sgs_params_t & p)
	{
		print_param("Covariance type", p.m_covariance_type);
		print_param("Sill", p.m_sill);
		print_param("Nugget", p.m_nugget);
		print_param("Ranges", p.m_ranges);
		print_param("Angles", p.m_angles);	
		print_param("Search radiuses", p.m_radiuses);
		print_param("Max number of neighbours", p.m_max_neighbours);
		if (p.m_calculate_mean)
			print_param("Mean", "automatic");
		else
			print_param("Mean", p.mean());
		print_param("Seed", p.m_seed);
	}

	void add_ws(int & counter, int pos)
	{
		printf(" ");
		++counter;
		while (counter < pos)
		{
			printf(" ");
			++counter;
		}
	}

	void print_params(const ik_params_t & p)
	{
		using namespace boost;
		std::string values         ("Indicators:\t");
		std::string cov_type       ("Covariance:\t");
		std::string ranges         ("Ranges    :\t");
		std::string angles         ("Angles    :\t");
		std::string sills          ("Sills     :\t");
		std::string nuggets        ("Nuggets   :\t");
		std::string radiuses       ("Radiuses  :\t");
		std::string maxneighbours  ("MaxNeighb :\t");
		std::string probs          ("MargProb  :\t");

		for (indicator_index_t idx = 0; idx < p.m_category_count; ++idx)
		{
			values += str(format("%1%\t") % idx);
			cov_type += str(format("%1%\t") % p.m_covariances[idx]);
			ranges += str(format("%1%:%2%:%3%\t") % p.m_ranges[idx][0] % p.m_ranges[idx][1] % p.m_ranges[idx][2]);
			angles += str(format("%1%:%2%:%3%\t") % p.m_angles[idx][0] % p.m_angles[idx][1] % p.m_angles[idx][2]);
			sills += str(format("%1%\t") % p.m_sills[idx]);
			nuggets += str(format("%1%\t") % p.m_nuggets[idx]);
			radiuses += str(format("%1%:%2%:%3%\t") % p.m_radiuses[idx][0] % p.m_radiuses[idx][1] % p.m_radiuses[idx][2]);
			maxneighbours += str(format("%1%\t") % p.m_neighbour_limits[idx]);
			probs += str(format("%1%\t") % p.m_marginal_probs[idx]);
		}

		write(values);
		write(cov_type);
		write(ranges);
		write(angles);
		write(sills);
		write(nuggets);
		write(radiuses);
		write(maxneighbours);
		write(probs);
		
		/*int pos[] = {0, 2, 6, 10, 22, 34, 41, 48, 63, 71}	;
		printf("#     Cov Ranges      Angles      Sill   Nugget SearchRadiuses MaxNeib MargProb\n");
		for (indicator_index_t idx = 0; idx < p.m_category_count; ++idx)
		{
			int counter = 0;			
			counter += printf("%d", idx);
			add_ws(counter, pos[1]);
			//counter += printf("%d", p.m_values[idx]);
			add_ws(counter, pos[2]);
			counter += printf("%d", p.m_covariances[idx]);
			add_ws(counter, pos[3]);
			counter += printf("%g:%g:%g", p.m_ranges[idx][0], p.m_ranges[idx][1], p.m_ranges[idx][2]);
			add_ws(counter, pos[4]);
			counter += printf("%g:%g:%g", p.m_angles[idx][0], p.m_angles[idx][1], p.m_angles[idx][2]);
			add_ws(counter, pos[5]);
			counter += printf("%g", p.m_sills[idx]);
			add_ws(counter, pos[6]);
			counter += printf("%g", p.m_nuggets[idx]);
			add_ws(counter, pos[7]);
			counter += printf("%g:%g:%g", p.m_radiuses[idx][0], p.m_radiuses[idx][1], p.m_radiuses[idx][2]);
			add_ws(counter, pos[8]); 
			counter += printf("%d", (int)p.m_neighbour_limits[idx]);
			add_ws(counter, pos[9]); 
			printf("%g\n", p.m_marginal_probs[idx]);				
		}				
		fflush(stdout);*/
	}

	void print_params(const indicator_params_t * p, int param_count, const mean_t * marginal_probs)
	{
	using namespace boost;
		std::string values         ("Indicators:\t");
		std::string cov_type       ("Covariance:\t");
		std::string ranges         ("Ranges    :\t");
		std::string angles         ("Angles    :\t");
		std::string sills          ("Sills     :\t");
		std::string nuggets        ("Nuggets   :\t");
		std::string radiuses       ("Radiuses  :\t");
		std::string maxneighbours  ("MaxNeighb :\t");
		std::string probs          ("MargProb  :\t");

		for (indicator_index_t idx = 0; idx < param_count; ++idx)
		{
			values += str(format("%1%\t") % idx);
			cov_type += str(format("%1%\t") % p[idx].cov_type);
			ranges += str(format("%1%:%2%:%3%\t") % p[idx].ranges[0] % p[idx].ranges[1] % p[idx].ranges[2]);
			angles += str(format("%1%:%2%:%3%\t") % p[idx].angles[0] % p[idx].angles[1] % p[idx].angles[2]);
			sills += str(format("%1%\t") % p[idx].sill);
			nuggets += str(format("%1%\t") % p[idx].nugget);
			radiuses += str(format("%1%:%2%:%3%\t") % p[idx].radiuses[0] % p[idx].radiuses[1] % p[idx].radiuses[2]);
			maxneighbours += str(format("%1%\t") % p[idx].max_neighbours);
			if (marginal_probs != 0)
				probs += str(format("%1%\t") % marginal_probs[idx]);
			else
				probs += "N/A\t";
		}

		write(values);
		write(cov_type);
		write(ranges);
		write(angles);
		write(sills);
		write(nuggets);
		write(radiuses);
		write(maxneighbours);
		write(probs);

		/*int pos[] = {0, 2, 6, 10, 22, 34, 41, 48, 63, 71}	;
		printf("#     Cov Ranges      Angles      Sill   Nugget SearchRadiuses MaxNeib MargProb\n");
		for (indicator_index_t idx = 0; idx < param_count; ++idx)
		{
			int counter = 0;			
			counter += printf("%d", idx);
			add_ws(counter, pos[1]);
			//counter += printf("%d", p.m_values[idx]);
			add_ws(counter, pos[2]);
			counter += printf("%d", p[idx].cov_type);
			add_ws(counter, pos[3]);
			counter += printf("%g:%g:%g", p[idx].ranges[0], p[idx].ranges[1], p[idx].ranges[2]);
			add_ws(counter, pos[4]);
			counter += printf("%g:%g:%g", p[idx].angles[0], p[idx].angles[1], p[idx].angles[2]);
			add_ws(counter, pos[5]);
			counter += printf("%g", p[idx].sill);
			add_ws(counter, pos[6]);
			counter += printf("%g", p[idx].nugget);
			add_ws(counter, pos[7]);
			counter += printf("%g:%g:%g", p[idx].radiuses[0], p[idx].radiuses[1], p[idx].radiuses[2]);
			add_ws(counter, pos[8]); 
			counter += printf("%d", p[idx].max_neighbours);
			add_ws(counter, pos[9]); 
			if (marginal_probs != 0)
				printf("%g\n", marginal_probs[idx]);				
			else
				printf("N/A\n");
		}				
		fflush(stdout);*/
	}
}
