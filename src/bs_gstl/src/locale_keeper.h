/*

    Copyright 2009 HPGL Team

    This file is part of HPGL (High Perfomance Geostatistics Library).

    HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.

    HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.

*/


/**
 * \file locale_keeper.h
 * \brief Helper class for set locale and reset locale on dtor
 * \author Miryanov Sergey
 * \date 14.04.2008
 */

#ifndef BS_LOCALE_KEEPER_H_
#define BS_LOCALE_KEEPER_H_

#include <locale.h>

namespace blue_sky {

  struct locale_keeper
  {
	  std::string locale_;
    int category_;

    locale_keeper (const char *new_name, int category_=LC_ALL)
      : category_ (category_)
    {
      locale_ = setlocale (category_, 0);
      setlocale (category_, new_name);
    }

    ~locale_keeper()
    {
      setlocale (category_, locale_.c_str());
    }
  };

}

#endif // #ifndef BS_LOCALE_KEEPER_H_
