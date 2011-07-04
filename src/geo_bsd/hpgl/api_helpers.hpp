#ifndef _API_HELPERS_H_KKJJ123NASDLKAS9234ASDnJKNKJNAJKSD
#define _API_HELPERS_H_KKJJ123NASDLKAS9234ASDnJKNKJNAJKSD

struct hpgl_shape_t;
struct hpgl_sgs_params_t;

namespace hpgl
{
	class sugarbox_grid_t;
	struct sgs_params_t;
	class ik_params_t;
	class indicator_params_t;

	int get_shape_volume(hpgl_shape_t * shape);
	void init_grid(hpgl::sugarbox_grid_t & grid, hpgl_shape_t * shape);
	void init_sgs_params(hpgl_sgs_params_t * params, hpgl::sgs_params_t * sgs_p);

	void 
	init_sis_params(
			hpgl_ik_params_t * params, 
			int indicator_count,
			ik_params_t * ikp);

	void
	init_sis_params_2(
		hpgl_ik_params_t * params,
		int indicator_count,
		std::vector<hpgl::indicator_params_t> & result);

	const std::string & 
	get_last_exception_message();

	void
	set_last_exception_message(const char * message);
}


#endif /* _API_HELPERS_H_KKJJ123NASDLKAS9234ASDnJKNKJNAJKSD */
