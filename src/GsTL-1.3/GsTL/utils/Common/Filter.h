#ifndef __VOXELGRIDFILTER_H
#define __VOXELGRIDFILTER_H

#include "Map.h"

namespace Common
{

/** A filter is an abstract template derived from a Map.
    
    Hence, a Filter is a Map, but it also takes its input from
    a Map which is passed to its constructor. An instantiatable 
    class derived from this one must do the following when the
    [] operator is invoked on a variable X of type MapV. 
    Transform X to X' in InputV (the domain of the input map).
    Take the value V = (*in_map)[X'] of X' and then map V to V' 
    in MapT and return V'.

    Concisely: MapV -> InputMapV -> InputT  -> MapT. */

template<class V,class IMV, class IMT,class T>
class Filter: public Map<V,T>
{
	typedef Map<IMV, IMT> InputMap;

protected:
	
  const InputMap* const in_map;
  
public:

  Filter(const InputMap* const _in_map): in_map(_in_map) {}

};

}

namespace CMN = Common;

#endif
