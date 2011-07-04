#ifndef __GSTL_SAMPLER_UPDATER_SAMPLER_H__
#define __GSTL_SAMPLER_UPDATER_SAMPLER_H__


/** Updater_sampler is a model of concept Sampler.
* An Updater_sampler first updates the ccdf before sampling it.
* The update can be local, ie dependent on the geovalue where
* the sampling is performed. 
* \c local_cdf_updater is a model of BinaryFunction that has the
* following signature:
*   cdf_type binary_function( const geovalue&, const cdf_type& ).
* \c sampler is a model of concept Sampler.
*/
template <class BinaryFunction, class Sampler_>
class Updater_sampler {
public:
  Updater_sampler( BinaryFunction local_cdf_updater,
                   Sampler_& sampler )  
    : local_cdf_updater_( local_cdf_updater ),
      sampler_( sampler ) {
  }

  int operator() ( typename BinaryFunction::first_argument& g, 
                   const typename BinaryFunction::second_argument& cdf ) {
    return sampler_( g, local_cdf_updater_( g, cdf ) );
  }

private:
  BinaryFunction local_cdf_updater_;
  Sampler_ sampler_;
};

#endif

