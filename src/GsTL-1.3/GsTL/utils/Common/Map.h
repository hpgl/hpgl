#ifndef __MAP_H
#define __MAP_H

#include <sstream>
#include <typeinfo>
#include "Common/ExceptionStandard.h"

namespace Common
{

DERIVEEXCEPTION(MapException, CMN::MotherException);

/** A Map is an abstract class for mapping from one domain to another.

    Basically, Map is a template which takes two args. The first is the
    image T and the second is the domain V, so Map: V->T

    Usually we map from a vector to a scalar - In volume graphics we usually
    map from a Vec3i to a float or something like that. */

template<class V, class T> class Map
{

protected:

	/** The map function must be overridden. This function is assumed to perform
			the mapping in an unsafe way and, hence, it is protected. Users should
			use the public [] operator. Derived classes can use map if they know 
			the input is in domain. 	*/
	virtual T map(const V&) const = 0;

public:
  
  /// Check if a point is in the domain.
  virtual bool in_domain(const V&) const = 0;
  
  /** Safe, const access to the map. THis function simply calls the pure 
			virtual map function, if the argument to the [] operator is in the
			domain. Otherwise it terminates the program. */
  T operator[](const V& v) const
	{
#ifndef NDEBUG
		if(!in_domain(v))
			{
				std::ostringstream str;
				str << "Map of type " << typeid(*this).name() << "\n"
						<< "applied to out-of-domain input: " << v << std::endl;
				throw MapException(str.str());
			}
#endif
		return map(v);
	}
};

}
namespace CMN = Common;

#endif
