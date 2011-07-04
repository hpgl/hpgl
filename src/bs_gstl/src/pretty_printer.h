/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


#ifndef __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4
#define __PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4



namespace hpgl
{
	class ok_params_t;
	class sk_params_t;
	class ik_params_t;
	class sgs_params_t;
	class neighbourhood_param_t;
	class covariance_param_t;

	template<typename T>
	void print_param(const std::string & param, const T & value)
	{
		std::cout << param << ": " << value << "\n";
	}

	void print_algo_name(const std::string & name);

	void print_params(const neighbourhood_param_t & p);

	void print_params(const covariance_param_t & p);
	
	void print_params(const ok_params_t & p);

	void print_params(const sk_params_t & p);

	void print_params(const sgs_params_t & p);

	void print_params(const ik_params_t & p);
}

#endif //__PRETTY_PRINTER_H__10B315C1_BC96_475C_B9E0_8FAE76849DE4