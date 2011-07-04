/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/



#include "stdafx.h"

#include "py_sgs_params.h"

namespace hpgl
{
	void py_sgs_params_t::set_covariance_type(int type)
	{
		m_sgs_params.m_covariance_type = covariance_type_t(type);
	}

	void py_sgs_params_t::set_ranges(double r1, double r2, double r3)
	{
		m_sgs_params.set_ranges(r1, r2, r3);
	}

	void py_sgs_params_t::set_angles(double a1, double a2, double a3)
	{
		m_sgs_params.set_angles(a1, a2, a3);
	}

	void py_sgs_params_t::set_sill(double sill)
	{
		m_sgs_params.m_sill = sill;
	}

	void py_sgs_params_t::set_nugget(double nugget)
	{
		m_sgs_params.m_nugget = nugget;
	}

	void py_sgs_params_t::set_radiuses(size_t r1, size_t r2, size_t r3)
	{
		m_sgs_params.set_radiuses(r1, r2, r3);
	}

	void py_sgs_params_t::set_max_neighbours(size_t mn)
	{
		m_sgs_params.m_max_neighbours = mn;
	}

	void py_sgs_params_t::set_mean(double mean)
	{
		m_sgs_params.set_mean(mean);
	}

	void py_sgs_params_t::set_kriging_kind(int kind)
	{
		m_sgs_params.m_kriging_kind = kriging_kind_t(kind);
	}

	void py_sgs_params_t::set_seed(long int seed)
	{
		m_sgs_params.m_seed = seed;
	}

	void py_sgs_params_t::set_mean_kind(const std::string & mean_kind)
	{
		if (mean_kind == "stationary_auto")
			m_sgs_params.m_mean_kind = e_mean_stationary_auto;
		else if (mean_kind == "stationary")
			m_sgs_params.m_mean_kind = e_mean_stationary;
		else if (mean_kind == "varying")
			m_sgs_params.m_mean_kind = e_mean_varying;
		else
			throw hpgl_exception("py_sgs_params", boost::format("Unknown mean kind: %s.") % mean_kind);
	}

	void py_sgs_params_t::set_auto_reg(bool auto_reg)
	{
		m_sgs_params.auto_region_size = auto_reg;
	}

	void py_sgs_params_t::set_region_size(size_t r1, size_t r2, size_t r3)
	{
		m_sgs_params.reg_size.d[0] = r1;
		m_sgs_params.reg_size.d[1] = r2;
		m_sgs_params.reg_size.d[2] = r3;
		
		m_sgs_params.reg_size.volume = r1 * r2 * r3;
	}
}
