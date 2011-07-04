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

 * modified by Jianbing Wu, Feb. 2006 @ Stanford University
 */

 

#ifndef __GTL_SEQUENTIAL_SIMULATION__
#define __GTL_SEQUENTIAL_SIMULATION__
#ifdef __GNUC__
#pragma interface
#endif

#include <GsTL/sampler/monte_carlo_sampler.h>
#include <GsTL/utils/debug_tools.h>


/** This function implements the sequential simulation algorithm
 * @return 0 if no problem were encountered during the simulation,
 * -1 if the execution was aborted, n (n>0) if the ccdf could not
 * be estimated n times.
 *
 * @param [begin,end) is the range of geovalues to be simulated
 * @param neighbors is the neighborhood that will be used to retrieve the 
 * conditioning data at each node to be simulated
 * @param ccdf is the conditional cdf that will be estimated at each 
 * node. <pre>ccdf</pre> is changed at each node.
 * @param estim is a functor the estimates the ccdf.
 * @param marginal is the marginal cdf.
 * @param samp is the functor that draws a realization from the ccdf.
 */
template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler
>
inline int 
sequential_simulation(
		      GeovalueIterator begin, GeovalueIterator end,
		      Neighborhood& neighbors,
		      Cdf& ccdf,
		      CdfEstimator& estim,
		      const MarginalCdf& marginal,
		      Sampler& samp
		      ) {
  int ok = 0;

  for(; begin != end; begin++) {
    if( begin->is_informed() ) continue;
    
    neighbors.find_neighbors( *begin );
    
    DEBUG_PRINT_LOCATION( "center", begin->location() );

    if( neighbors.is_empty() ){
      //if we don't have any conditioning data, we simply draw from the
      // marginal
      WRITE_TO_DEBUG_STREAM( "drawing from marginal" << std::endl );
      samp(*begin, marginal);
    }
    else {
      DEBUG_PRINT_NEIGHBORHOOD( "neighbors", &neighbors );

      int status = estim( *begin, neighbors, ccdf);

      if(status == 0) {
      	samp(*begin, ccdf);
      }
      else {
      	// the ccdf could not be estimated. Draw from the marginal
	      samp(*begin, marginal);
	      ok++;
      }
    }
  }

  return ok;
}





/** This function implements the sequential simulation algorithm.
 * @return 0 if no problem were encountered during the simulation,
 * -1 if the execution was aborted, n (n>0) if the ccdf could not
 * be estimated n times.
 *
 * @param [begin,end) is the range of geovalues to be simulated
 * @param neighbors is the neighborhood that will be used to retrieve the 
 * conditioning data at each node to be simulated
 * @param ccdf is the conditional cdf that will be estimated at each 
 * node. <pre>ccdf</pre> is changed at each node.
 * @param estim is a functor the estimates the ccdf.
 * @param marginal is the marginal cdf.
 * @param samp is the functor that draws a realization from the ccdf.
 * @param notifier is an object that has a <pre>notify()</pre> function.
 * That function is executed after a node has been simulated. It can be 
 * used to give information on the progress of the simulation.
 */
template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler,
  class Notifier
>
inline int 
sequential_simulation(
		      GeovalueIterator begin, GeovalueIterator end,
		      Neighborhood& neighbors,
		      Cdf& ccdf,
		      CdfEstimator& estim,
		      const MarginalCdf& marginal,
		      Sampler& samp,
		      Notifier* notifier
		      ) {

  int bad = 0;

  for(; begin != end; begin++) {
    if( begin->is_informed() ) continue;
      
    neighbors.find_neighbors( *begin );

    DEBUG_PRINT_LOCATION( "center", begin->location() );

    if( neighbors.is_empty() ){
      //if we don't have any conditioning data, we simply draw from the
      // marginal
      WRITE_TO_DEBUG_STREAM( "drawing from marginal" << std::endl );
      samp(*begin, marginal);
    }
    else {
      DEBUG_PRINT_NEIGHBORHOOD( "neighbors", &neighbors );

      int status = estim( *begin, neighbors, ccdf);
      
      if(status == 0) {
	      samp(*begin, ccdf);
      }
      else {
      	// the ccdf could not be estimated. Draw from the marginal
        WRITE_TO_DEBUG_STREAM( "Can't estimate ccdf. drawing from marginal\n" );
      	samp(*begin, marginal);
      	bad++;
      }
    }

    if( !notifier->notify() ) return -1;
  }

  return bad;
}



/** This overloaded version of sequential_simulation uses a Monte Carlo sampling
 * scheme by default. The random number generator is initialized with a default
 * seed value.
 */
template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf
>
inline int
sequential_simulation(
                      GeovalueIterator begin, GeovalueIterator end,
                      Neighborhood& neighbors, Cdf& ccdf,
                      CdfEstimator& estim, const MarginalCdf& marginal
                      ) {

  // use a random number generator with a default seed.
  Monte_carlo_sampler_t<> sampler;
  return sequential_simulation(
                               begin, end,
                               neighbors, ccdf,
                               estim, marginal,
                               sampler
                               );
}




/** This overloaded version of sequential_simulation does not require a
 * marginal cdf. If a node can not be simulated, e.g. because no conditioning
 * data could be found or because the ccdf could not be estimated, the
 * node is left unestimated...
 */
template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class Sampler
>
inline int
sequential_simulation_2(
                        GeovalueIterator begin, GeovalueIterator end,
                        Neighborhood& neighbors, Cdf& ccdf,
                        CdfEstimator& estim,
                        Sampler& samp
                        ) {

  int ok=0;

  for( ; begin != end; ++begin ) {

    if( begin->is_informed() ) continue;

    neighbors.find_neighbors( *begin );

    int status = estim( *begin, *neighbors, ccdf);

    if(status == 0)
      samp(*begin, ccdf);
    else {
      // The ccdf could not be estimated. Since we don't have a marginal
      // distribution, the node is left unestimated...
      ok++;
    }
  }

  return ok;
}


/*
 * record the number of nodes dropped during simulation 
 */
template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler,
  class DropNodes
>
inline int 
sequential_simulation(
		      GeovalueIterator begin, GeovalueIterator end,
		      Neighborhood& neighbors,
		      Cdf& ccdf,
		      CdfEstimator& estim,
		      const MarginalCdf& marginal,
		      Sampler& samp,
              DropNodes& drop
		      ) {
  int ok = 0;
  int loc = 0;
  int nodes_dropped;

  for(; begin != end; begin++, loc++) {
    if( begin->is_informed() ) continue;
    
    neighbors.find_neighbors( *begin );
    
    DEBUG_PRINT_LOCATION( "center", begin->location() );

    if( neighbors.is_empty() ){
      //if we don't have any conditioning data, we simply draw from the
      // marginal
      WRITE_TO_DEBUG_STREAM( "drawing from marginal" << std::endl );
      samp(*begin, marginal);  
      //drop( loc, neighbors.max_size() );
    }
    else {
      DEBUG_PRINT_NEIGHBORHOOD( "neighbors", &neighbors );

      int status = estim( *begin, neighbors, ccdf, nodes_dropped);	//get ccdf from the search trees
      drop( loc, nodes_dropped );

      if(status == 0) {
      	samp(*begin, ccdf);
      }
      else {
      	// the ccdf could not be estimated. Draw from the marginal
	      samp(*begin, marginal);
	      ok++;
      }
    }
  }

  return ok;
}


template
<
  class GeovalueIterator,
  class Neighborhood,
  class Cdf,
  class CdfEstimator,
  class MarginalCdf,
  class Sampler,
  class Notifier,
  class DropNodes
>
inline int 
sequential_simulation(
		      GeovalueIterator begin, GeovalueIterator end,
		      Neighborhood& neighbors,
		      Cdf& ccdf,
		      CdfEstimator& estim,
		      const MarginalCdf& marginal,
		      Sampler& samp,
		      Notifier* notifier,
              DropNodes& drop
		      ) {

  int bad = 0;
  int loc = 0;
  int nodes_dropped;

  for(; begin != end; begin++, loc++) {
    if( begin->is_informed() ) continue;
      
    neighbors.find_neighbors( *begin );

    DEBUG_PRINT_LOCATION( "center", begin->location() );

    if( neighbors.is_empty() ){
      //if we don't have any conditioning data, we simply draw from the
      // marginal
      WRITE_TO_DEBUG_STREAM( "drawing from marginal" << std::endl );
      samp(*begin, marginal);
      //drop( loc, neighbors.max_size() );
    }
    else {
      DEBUG_PRINT_NEIGHBORHOOD( "neighbors", &neighbors );

      int status = estim( *begin, neighbors, ccdf, nodes_dropped);	//get ccdf from the search trees
      drop( loc, nodes_dropped );

      if(status == 0) {
	      samp(*begin, ccdf);
      }
      else {
      	// the ccdf could not be estimated. Draw from the marginal
        WRITE_TO_DEBUG_STREAM( "Can't estimate ccdf. drawing from marginal\n" );
      	samp(*begin, marginal);
      	bad++;
      }
    }

    if( !notifier->notify() ) return -1;
  }

  return bad;
}



#endif
