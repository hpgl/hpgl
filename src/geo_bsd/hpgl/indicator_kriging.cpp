#include "stdafx.h"
#include "typedefs.h"
#include "property_array.h"
#include "sugarbox_grid.h"
#include "ik_params.h"
#include "indicator_kriging.h"
#include "pretty_printer.h"
#include "mean_provider.h"
#include "sugarbox_indexed_neighbour_lookup.h"

namespace hpgl
{
void indicator_kriging(
	const indicator_property_array_t & input,
	const sugarbox_grid_t & grid,  //TODO: Сделать тип grid_size_t задающий размеры грида, и все функции перевода координат сделать свободными
	const ik_params_t & params,
	indicator_property_array_t & output)
{
	std::vector<single_mean_t> single_means;
	create_means(params.m_marginal_probs, single_means);
	do_indicator_kriging(input, grid, params, single_means, output);
}

}
