#include "stdafx.h"
#include "sgs_params.h"

namespace hpgl
{
	sgs_params_t::sgs_params_t()
	{
		m_kriging_kind = kriging_kind_t::KRIG_ORDINARY;
	}
}