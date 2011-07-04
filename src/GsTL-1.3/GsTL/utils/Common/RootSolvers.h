#ifndef __ROOTSOLVERS_H
#define __ROOTSOLVERS_H

#include "Common/CommonDefs.h"

namespace Common
{

int solve_quadric(double c[ 3 ], double s[ 2 ]);

#ifndef WIN32
int solve_cubic(double   c[ 4 ], double s[ 3 ]);
int solve_quartic(double c[ 5 ], double s[ 4 ]);
#endif

}

namespace CMN = Common;

#endif
