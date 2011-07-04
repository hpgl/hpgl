#include "stdafx.h"
#include "api.h"
#include "covariance_type.h"
#include "sugarbox_grid.h"
#include "sgs_params.h"
#include "ik_params.h"
#include "hpgl_core.h"

namespace hpgl
{

static std::string last_exception_message;

const std::string & 
get_last_exception_message()
{
	return last_exception_message;
}

void
set_last_exception_message(const char * message)
{
	last_exception_message = message;
}

int get_shape_volume(hpgl_shape_t * shape)
{
	int result = 1;
	for (int i = 0; i < 3; ++i)
	{
		result *= shape->m_data[i];
	}
	return result;
}


void init_grid(hpgl::sugarbox_grid_t & grid, hpgl_shape_t * shape)
{
	grid.init(shape->m_data[0], shape->m_data[1], shape->m_data[2]);
}

void init_sgs_params(hpgl_sgs_params_t * params, hpgl::sgs_params_t * params2)
{
	sgs_params_t & sgs_p = *params2;
	sgs_p.m_covariance_type = (covariance_type_t) params->m_covariance_type;
	sgs_p.set_ranges(
			params->m_ranges[0],
			params->m_ranges[1],
			params->m_ranges[2]);
	sgs_p.set_angles(
			params->m_angles[0],
			params->m_angles[1],
			params->m_angles[2]);
	sgs_p.m_sill = params->m_sill;
	sgs_p.m_nugget = params->m_nugget;
	
	sgs_p.set_radiuses(
			params->m_radiuses[0],
			params->m_radiuses[1],
			params->m_radiuses[2]);

	sgs_p.m_max_neighbours = params->m_max_neighbours;
	sgs_p.m_kriging_kind = (kriging_kind_t) params->m_kriging_kind;
	sgs_p.m_seed = params->m_seed;
	sgs_p.m_min_neighbours = params->m_min_neighbours;
}

void 
init_sis_params(
		hpgl_ik_params_t * params, 
		int indicator_count,
		hpgl::ik_params_t * ikp)
{
	ikp->m_category_count = indicator_count;
	for (int i = 0; i < indicator_count; ++i)
	{
		hpgl_ik_params_t * p = &(params[i]);
		ikp->m_covariances.push_back((covariance_type_t)p->m_covariance_type);
		std::vector<range_t> ranges;
		ranges.assign(p->m_ranges, p->m_ranges + 3);
		ikp->m_ranges.push_back(ranges);
		std::vector<angle_t> angles;
		angles.assign(p->m_angles, p->m_angles + 3);
		ikp->m_angles.push_back(angles);
		ikp->m_sills.push_back(p->m_sill);
		ikp->m_nuggets.push_back(p->m_nugget);
		ikp->m_radiuses.push_back(sugarbox_search_ellipsoid_t(
										 p->m_radiuses[0],
										 p->m_radiuses[1],
										 p->m_radiuses[2]));
		ikp->m_neighbour_limits.push_back(p->m_max_neighbours);
		ikp->m_marginal_probs.push_back(p->m_marginal_prob);
		covariance_param_t cp;
		cp.m_covariance_type = (covariance_type_t) p->m_covariance_type;
		for (int j = 0; j < 3; ++j)
		{
			cp.m_ranges[j] = p->m_ranges[j];
			cp.m_angles[j] = p->m_angles[j];
		}
		cp.m_sill = p->m_sill;
		cp.m_nugget = p->m_nugget;
		ikp->m_cov_params.push_back(cp);
		neighbourhood_param_t nbp;
		nbp.m_radiuses = sugarbox_search_ellipsoid_t(
				p->m_radiuses[0],
				p->m_radiuses[1],
				p->m_radiuses[2]);
		nbp.m_max_neighbours = p->m_max_neighbours;
		ikp->m_nb_params.push_back(nbp);
	}
}

void
init_sis_params_2(
		hpgl_ik_params_t * params,
		int indicator_count,
		std::vector<hpgl::indicator_params_t> & result)
{
	result.resize(indicator_count);

	for (int idx = 0; idx < indicator_count; ++idx)
	{
		indicator_params_t * p = &result[idx];
		hpgl_ik_params_t * o = &params[idx];
		p->cov_type = o->m_covariance_type;
		for (int i = 0; i < 3; ++i)
		{
			p->ranges[i] = o->m_ranges[i];
			p->angles[i] = o->m_angles[i];
			p->radiuses[i] = o->m_radiuses[i];
		}
		p->sill = o->m_sill;
		p->nugget = o->m_nugget;
		p->max_neighbours = o->m_max_neighbours;
	}
}

}
