/* GsTL: the Geostatistics Template Library
 * 
 * Author: Nicolas Remy
 * Copyright (c) 2000 The Board of Trustees of the Leland Stanford Junior University
 * 
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modification, 
 * are permitted provided that the following conditions are met:
 * 
 *   1.Redistributions of source code must retain the above copyright notice, this 
 *     list of conditions and the following disclaimer. 
 *   2.Redistributions in binary form must reproduce the above copyright notice, this 
 *     list of conditions and the following disclaimer in the documentation and/or other
 *     materials provided with the distribution. 
 *   3.The name of the author may not be used to endorse or promote products derived 
 *     from this software without specific prior written permission. 
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED 
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR BE 
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

 

#ifndef __GTL_SEQUENTIAL_COSIMULATION__
#define __GTL_SEQUENTIAL_COSIMULATION__
#ifdef __GNUC__
#pragma interface
#endif


#include <GsTL/sampler/monte_carlo_sampler.h>


/** Performs sequential simulation on geovalues in range [begin, end).
 * @return the number of problem that occured during the simulation.
 * @param [first_neigh, last_neigh) is a range of Neighborhoods.
 */

template
<
  class GeovalueIterator,
  class ForwardIterator,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler
>
inline int
sequential_cosimulation(
			GeovalueIterator begin, GeovalueIterator end,
			ForwardIterator first_neigh, ForwardIterator last_neigh, 
			Cdf& ccdf,
			CdfEstimator& estim,
			const MarginalCdf& marginal,
			Sampler& samp
			) {

  int bad=0;

  for( ; begin != end; ++begin ) {
    
    // skip informed nodes
    if( begin->is_informed() ) continue;

    int nb_conditioning_data = 0;
    for(ForwardIterator it = first_neigh ; it != last_neigh; ++it ) {
      it->find_neighbors( *begin );
      nb_conditioning_data += it->size();
    }

    if( nb_conditioning_data == 0 )
      samp(*begin, marginal);
    else {
      int status = estim( *begin,
	                  		  first_neigh, last_neigh,
			                    ccdf );
      if(status == 0)
      	samp(*begin, ccdf);
      else {
      	// ccdf could not be estimated: draw from the marginal
	      bad++;
	      samp(*begin, marginal);
      }
    }
  }
  return bad;
}



template
<
  class GeovalueIterator,
  class ForwardIterator,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler,
  class Notifier
>
inline int
sequential_cosimulation(
                        GeovalueIterator begin, GeovalueIterator end,
                        ForwardIterator first_neigh, ForwardIterator last_neigh, 
                        Cdf& ccdf,
                        CdfEstimator& estim,
                        const MarginalCdf& marginal,
                        Sampler& samp,
                        Notifier* notifier
                        ) {
  int bad=0;
 
  for( ; begin != end; ++begin ) {
    // skip informed nodes^M
    if( begin->is_informed() ) continue;
 
    int nb_conditioning_data = 0;
    for(ForwardIterator it = first_neigh ; it != last_neigh; ++it ) {
      it->find_neighbors( *begin );
      nb_conditioning_data += it->size();
    }
 
    if( nb_conditioning_data == 0 )
      samp(*begin, marginal);
    else {
      int status = estim( *begin,
                          first_neigh, last_neigh,
                          ccdf );
      if(status == 0)
        samp(*begin, ccdf);
      else {
        // ccdf could not be estimated: draw from the marginal
        bad++;
        samp(*begin, marginal);
      }
    }
 
    if( !notifier->notify() ) return -1;
  }

  gstl_message( "ccdf could not be estimated " << bad << " times" ); 
  return bad;
}




template
<
  class GeovalueIterator,
  class ForwardIterator,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf
>
inline int
sequential_cosimulation(
			GeovalueIterator begin, GeovalueIterator end,
			ForwardIterator first_neigh, ForwardIterator last_neigh, 
			Cdf& ccdf,
			CdfEstimator& estim,
			const MarginalCdf& marginal
			) {
  
  return sequential_cosimulation(begin, end,
				 first_neigh, last_neigh,
				 ccdf,
				 estim, marginal,
				 Monte_carlo_sampler(
				    Rand48_generator(21111975)
				    )
				 );
}

#endif
