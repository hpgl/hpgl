#ifndef _API_H_ASDJLKJ123123L192301923LKAJSldsDFSDFsd123123
#define _API_H_ASDJLKJ123123L192301923LKAJSldsDFSDFsd123123

#ifdef _MSC_VER
#ifdef HPGL_DLL
#ifdef HPGL_EXPORTS
#define HPGL_API __declspec(dllexport)
#else
#define HPGL_API __declspec(dllimport)
#endif /* HPGL_EXPORTS */
#else
#define HPGL_API 
#endif /* HPGL_DLL */
#else
#define HPGL_API
#endif

struct hpgl_shape_t
{
	int m_data[3];
	int m_strides[3];
};

struct hpgl_cont_masked_array_t
{
	float * m_data;
	unsigned char * m_mask;
	hpgl_shape_t m_shape;
};

struct hpgl_ind_masked_array_t
{
	unsigned char * m_data;
	unsigned char * m_mask;
	hpgl_shape_t m_shape;
	int m_indicator_count;
};

struct hpgl_float_array_t
{
	float * m_data;
	hpgl_shape_t m_shape;
};

struct hpgl_ubyte_array_t
{
	unsigned char * m_data;
	hpgl_shape_t m_shape;
};

struct hpgl_ok_params_t
{
  int m_covariance_type;
  double m_ranges[3];
  double m_angles[3];
  double m_sill;
  double m_nugget;
  int m_radiuses[3];
  int m_max_neighbours;
};

struct hpgl_sk_params_t
{
  int m_covariance_type;
  double m_ranges[3];
  double m_angles[3];
  double m_sill;
  double m_nugget;
  int m_radiuses[3];
  int m_max_neighbours;
  unsigned char m_automatic_mean;
  double m_mean;
};

struct hpgl_cov_params_t
{
  int m_covariance_type;
  double m_ranges[3];
  double m_angles[3];
  double m_sill;
  double m_nugget;
};

struct hpgl_sgs_params_t
{
  int m_covariance_type;
  double m_ranges[3];
  double m_angles[3];
  double m_sill;
  double m_nugget;
  int m_radiuses[3];
  int m_max_neighbours;
  int m_kriging_kind;
  long int m_seed;
  int m_min_neighbours;
};

struct hpgl_median_ik_params_t
{
	int m_covariance_type;
	double m_ranges[3];
	double m_angles[3];
	double m_sill;
	double m_nugget;
	int m_radiuses[3];
	int m_max_neighbours;
	double m_marginal_probs[2];
};

struct hpgl_ik_params_t
{
	int m_covariance_type;
	double m_ranges[3];
	double m_angles[3];
	double m_sill;
	double m_nugget;
	int m_radiuses[3];
	int m_max_neighbours;
	double m_marginal_prob;
};

struct hpgl_cokriging_m1_params_t
{
	int m_covariance_type;
	double m_ranges[3];
	double m_angles[3];
	double m_sill;
	double m_nugget;
	int m_radiuses[3];
	int m_max_neighbours;	
	double m_primary_mean;
	double m_secondary_mean;
	double m_secondary_variance;
	double m_correlation_coef;
};

struct hpgl_cokriging_m2_params_t
{
	hpgl_cov_params_t m_primary_cov_params;
	hpgl_cov_params_t m_secondary_cov_params;
	int m_radiuses[3];
	int m_max_neighbours;
	double m_primary_mean;
	double m_secondary_mean;
	double m_correlation_coef;
};

struct hpgl_non_parametric_cdf_t
{
	float * m_values;
	float * m_probs;
	long long m_size;
};

#ifdef __cplusplus
extern "C" {
#endif

HPGL_API char * 
hpgl_get_last_exception_message();

HPGL_API void 
hpgl_set_output_handler(int (*handler)(char * str, void * param), void * param);

HPGL_API void 
hpgl_set_progress_handler(int (*handler)(char * stage, int percentage, void * param), void * param);

HPGL_API void hpgl_set_thread_num(int n_threads);
HPGL_API int hpgl_get_thread_num();

HPGL_API void hpgl_read_inc_file_float(
		char * filename,
		float undefined_value,
		int size,
		float * data,
		unsigned char * mask);

HPGL_API void hpgl_read_inc_file_byte(
		char * filename,
		int undefined_value,
		int size,
		unsigned char * data,
		unsigned char * mask,
		unsigned char * values,
		int values_count);

HPGL_API void hpgl_write_inc_file_float(
		char * filename,
		hpgl_cont_masked_array_t * arr,
		float undefined_value,
		char * name);

HPGL_API void hpgl_write_inc_file_byte(
		char * filename,
		hpgl_ind_masked_array_t * arr,
		int undefined_value,		
		char * name,
		unsigned char * values,
		int values_count);

HPGL_API void
hpgl_write_gslib_cont_property(
		hpgl_cont_masked_array_t * data,
		const char * filename,
		const char * name,
		double undefined_value);

HPGL_API void
hpgl_write_gslib_byte_property(
		hpgl_ind_masked_array_t * data,
		const char * filename,
		const char * name,
		double undefined_value,
		unsigned char * values,
		int values_count);


HPGL_API void hpgl_ordinary_kriging(
    hpgl_cont_masked_array_t * input_data,
    hpgl_ok_params_t * params,
    hpgl_cont_masked_array_t * output_data); 

HPGL_API void hpgl_simple_kriging(   
    float * input_data,
    unsigned char * input_mask,
    hpgl_shape_t * input_data_shape,
    hpgl_sk_params_t * params,
    float * output_data,
    unsigned char * output_mask,
    hpgl_shape_t * output_data_shape);

HPGL_API int
hpgl_simple_kriging_weights(
		float * center_coords,
		float * neighbours_x,
		float * neighbours_y,
		float * neighbours_z,
		int neighbours_count,
		hpgl_cov_params_t * params,
		float * weights);

HPGL_API void hpgl_lvm_kriging(
    float * input_data,
    unsigned char * input_mask,
    hpgl_shape_t * input_data_shape,
    float * mean_data,
    hpgl_shape_t * mean_data_shape,
    hpgl_ok_params_t * params,
    float * output_data,
    unsigned char * output_mask,
    hpgl_shape_t * output_data_shape);

HPGL_API void hpgl_sgs_simulation(
    hpgl_cont_masked_array_t * prop,
    hpgl_sgs_params_t * params,	
	hpgl_non_parametric_cdf_t * cdf,
	double * mean,
	hpgl_ubyte_array_t * simulation_mask);

HPGL_API void hpgl_sgs_lvm_simulation(
		hpgl_cont_masked_array_t * data,
		hpgl_sgs_params_t * params,
		hpgl_non_parametric_cdf_t * cdf,
		hpgl_float_array_t * means,
		hpgl_ubyte_array_t * simulation_mask);

HPGL_API void
hpgl_indicator_kriging(
		hpgl_ind_masked_array_t * in_data,
		hpgl_ind_masked_array_t * out_data,
		hpgl_ik_params_t * params,
		int indicator_count);

HPGL_API void hpgl_median_ik(
		hpgl_ind_masked_array_t * in_data,
		hpgl_median_ik_params_t * params,
		hpgl_ind_masked_array_t * out_data);

HPGL_API void
hpgl_sis_simulation(
		hpgl_ind_masked_array_t * data,
		hpgl_ik_params_t * params,
		int indicator_count,
		int seed,
		hpgl_ubyte_array_t * simulation_mask);

HPGL_API void
hpgl_sis_simulation_lvm(
		hpgl_ind_masked_array_t * data,
		hpgl_ik_params_t * params,
		hpgl_float_array_t * mean_data,
		int indicator_count,
		int seed,
                hpgl_ubyte_array_t * simulation_mask,
		int use_correlograms);

HPGL_API void
hpgl_simple_cokriging_mark1(
		hpgl_cont_masked_array_t * input_data,
		hpgl_cont_masked_array_t * secondary_data,
		hpgl_cokriging_m1_params_t * params,
		hpgl_cont_masked_array_t * output_data);

HPGL_API void
hpgl_simple_cokriging_mark2(
		hpgl_cont_masked_array_t * primary_data,
		hpgl_cont_masked_array_t * secondary_data,
		hpgl_cokriging_m2_params_t * params,		
		hpgl_cont_masked_array_t * output_data);

#ifdef __cplusplus
    }
#endif

#endif /* _API_H_ASDJLKJ123123L192301923LKAJSldsDFSDFsd123123 */
