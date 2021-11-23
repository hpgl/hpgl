#ifndef __HPGL_CORE_H__A666CEEC_CEB4_49C4_9B92_992B1A9EA24B
#define __HPGL_CORE_H__A666CEEC_CEB4_49C4_9B92_992B1A9EA24B

#include "typedefs.h"
#include "api.h"
#include "property_array.h"

namespace hpgl
{
	class ok_params_t;
	class sk_params_t;
	class ik_params_t;
	class sgs_params_t;
	class neighbourhood_param_t;
	class progress_reporter_t;
	class sugarbox_grid_t;
	class indicator_lvm_data_t;
	class covariance_param_t;
	struct indicator_params_t
	{
		int cov_type;
		double ranges[3];
		double angles[3];
		double sill;
		double nugget;
		double radiuses[3];
		int max_neighbours;
	};

	void set_thread_num(int n_threads);
	int get_thread_num();

	void read_inc_file_float(
		const char* file_name,
		float undefined_value,
		int size,
		float* data_buffer,
		unsigned char* mask_buffer);

	void read_inc_file_byte(
		const char* file_name,
		int undefined_value,
		int size,
		unsigned char* data_buffer,
		unsigned char* mask_buffer);


	//TODO: maybe where should be pointers instead of references?

	void ordinary_kriging(
		const cont_property_array_t& input,
		const sugarbox_grid_t& grid,
		const ok_params_t& params,
		cont_property_array_t& output,
		bool use_new_cov = true);

	void simple_kriging(
		const cont_property_array_t& input,
		const sugarbox_grid_t& grid,
		const sk_params_t& params, //TODO: �������� �� ok_params � mean_t // �����?!
		cont_property_array_t& output);

	void simple_kriging_weights(
		const covariance_param_t* cov_params,
		real_location_t center_point,
		std::vector<real_location_t> neighbourhoods_coords,
		std::vector<kriging_weight_t>& weights,
		double& variance);

	void lvm_kriging(
		const cont_property_array_t& input,
		const mean_t* mean_data,
		const sugarbox_grid_t& grid,
		const ok_params_t& params,
		cont_property_array_t& output);

	void indicator_kriging(
		const indicator_property_array_t& input,
		const sugarbox_grid_t& grid,
		const ik_params_t& params,
		indicator_property_array_t& output);

	// median_ik

	void sequential_gaussian_simulation(
		const sugarbox_grid_t& grid,
		const sgs_params_t& params,
		cont_property_array_t& output,
		const hpgl_non_parametric_cdf_t* cdf,
		const unsigned char* mask = NULL);

	void sequential_gaussian_simulation_lvm(
		const sugarbox_grid_t& grid,
		const sgs_params_t& params,
		const mean_t* mean_data,
		cont_property_array_t& output,
		const hpgl_non_parametric_cdf_t* cdf,
		const unsigned char* mask = NULL);

	void sequential_indicator_simulation(
		indicator_property_array_t& property,
		const sugarbox_grid_t& grid,
		const ik_params_t& params,
		int seed,
		progress_reporter_t& report,
		bool use_corellogram,
		const unsigned char* mask = NULL);

	void sequential_indicator_simulation_lvm(
		indicator_property_array_t& property,
		const sugarbox_grid_t& grid,
		const ik_params_t& params,
		int seed,
		const mean_t** mean_data,
		progress_reporter_t& report,
		bool use_corellogram,
		const unsigned char* mask = NULL);


	struct sis_mean_params_t
	{
		mean_kind_t m_mean_kind;
		mean_t* m_single_means;
		mean_t** m_lvm_means;
		bool m_use_corellograms;
	};

	void simple_cokriging_markI(
		const sugarbox_grid_t& grid,
		const cont_property_array_t& input_prop,
		const cont_property_array_t& secondary_data,
		mean_t primary_mean,
		mean_t secondary_mean,
		double secondary_variance,
		double correlation_coef,
		const neighbourhood_param_t& neighbourhood_params,
		const covariance_param_t& primary_cov_params,
		cont_property_array_t& output_prop);

	void simple_cokriging_markII(
		const sugarbox_grid_t& grid,
		const cont_property_array_t& input_prop,
		const cont_property_array_t& secondary_data,
		mean_t primary_mean,
		mean_t secondary_mean,
		double correlation_coef,
		const neighbourhood_param_t& neighbourhood_params,
		const covariance_param_t& primary_cov_params,
		const covariance_param_t& secondary_cov_params,
		cont_property_array_t& output_prop);

	//dual_kriging



	bool calc_mean(const cont_property_array_t* prop, double* mean);



}



#endif //__HPGL_CORE_H__A666CEEC_CEB4_49C4_9B92_992B1A9EA24B
