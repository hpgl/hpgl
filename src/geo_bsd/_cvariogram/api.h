#ifndef _API_H_ASDJASLJDWQJLK1289379812hfdsaksdjKNKZBCXJHSADbhj7834kjsdkf78s234
#define _API_H_ASDJASLJDWQJLK1289379812hfdsaksdjKNKZBCXJHSADbhj7834kjsdkf78s234

#ifdef _MSC_VER
#ifdef CVAR_DLL
#ifdef CVAR_EXPORTS
#define CVAR_API1 __declspec(dllexport)
#else
#define CVAR_API1 __declspec(dllimport)
#endif /* HPGL_EXPORTS */
#else
#define CVAR_API1 
#endif /* HPGL_DLL */
#else
#define CVAR_API1
#endif

#ifdef __cplusplus
#define CVAR_API  extern "C" CVAR_API1
#else
#define CVAR_API CVAR_API1
#endif

struct vector_t
{
	double m_data[3];
};

struct ellipsoid_t
{
	vector_t m_direction1;
	vector_t m_direction2;
	vector_t m_direction3;
	double m_R1;
	double m_R2;
	double m_R3;
};

struct variogram_search_template_t
{
	double m_lag_width;
	double m_lag_separation;
	double m_tol_distance;
	int m_num_lags;
	double m_first_lag_distance;
	ellipsoid_t m_ellipsoid;
};

struct search_template_window_t
{
	double m_min_i;
	double m_min_j;
	double m_min_k;
	double m_max_i;
	double m_max_j;
	double m_max_k;
};

struct lag_point_t
{
    int m_coords[3];
    int m_lag_index;
};

struct cont_point_set_t
{
	float * xs;
	float * ys;
	float * zs;
	float * values;
	int size;
};

struct hard_data_t
{
	float * m_data;
	unsigned char * m_mask;
	int m_data_shape[3];
	int m_data_strides[3];
	int m_mask_shape[3];
	int m_mask_strides[3];
};

struct float_data_t
{
	float * m_data;
	int m_data_shape[3];
	int m_data_strides[3];
};


CVAR_API void 
fill_ellipsoid_directions(
    ellipsoid_t * ell, 
    double azimuth, 
    double dip, 
    double rotation);

bool 
is_in_tunnel(
    variogram_search_template_t * templ,
    vector_t * vec);

bool 
is_in_tunnel_v(
    variogram_search_template_t * templ,
    vector_t * vec, bool * results, int count);


void 
calc_search_template_window(
		variogram_search_template_t * templ,
		search_template_window_t * window);

lag_point_t * 
calc_lag_areas(variogram_search_template_t * templ, int * points_count);

CVAR_API void 
calc_variograms(
		variogram_search_template_t * templ,
		hard_data_t * data,
		float * result_covariations,
		int result_length,
		int percentToUse);

CVAR_API void 
calc_variograms_from_point_set(
		variogram_search_template_t * templ,
		cont_point_set_t * point_set,
		float * result_covariations,
		int result_length);

CVAR_API void
cvar_stack_layers(
		float_data_t * thick_layers,
		int * layer_markers,
		int layers_count,
		int nz,
		float scalez,
		int blank_value,
		float_data_t * result);


#endif /* _API_H_ASDJASLJDWQJLK1289379812hfdsaksdjKNKZBCXJHSADbhj7834kjsdkf78s234 */
