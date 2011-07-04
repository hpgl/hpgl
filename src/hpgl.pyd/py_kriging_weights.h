/*
   Copyright 2009 HPGL Team
   This file is part of HPGL (High Perfomance Geostatistics Library).
   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
   You should have received a copy of the BSD License along with HPGL.

*/


/* 
 * File:   py_kriging_weights.h
 * Author: nobu
 *
 * Created on 05 March 2009, 11:24
 */

#ifndef _PY_KRIGING_WEIGHTS_H
#define	_PY_KRIGING_WEIGHTS_H

#include "typedefs.h"

namespace hpgl
{
    boost::python::list py_calculate_kriging_weight(
        const boost::python::list & center_coords,
        const boost::python::list & neighbourhoods_coords_x,
        const boost::python::list & neighbourhoods_coords_y,
        const boost::python::list & neighbourhoods_coords_z,
            const py_sk_params_t & param)

    {
        assert(boost::python::len(center_coords) == 3);

        assert(boost::python::len(neighbourhoods_coords_x) ==
                boost::python::len(neighbourhoods_coords_y) ==
                boost::python::len(neighbourhoods_coords_z) );

        // center point
        real_location_t center((double)boost::python::extract<double>(center_coords[0]),
                (double)boost::python::extract<double>(center_coords[1]),
                (double)boost::python::extract<double>(center_coords[2]));

        std::vector<real_location_t> neighbourhoods_coords;

        // neighbourhoods
        for (int i = 0; i < boost::python::len(neighbourhoods_coords_x); i++)
        {
            neighbourhoods_coords.push_back(
                    real_location_t(
                        (double)boost::python::extract<double>(neighbourhoods_coords_x[i]),
                        (double)boost::python::extract<double>(neighbourhoods_coords_y[i]),
                        (double)boost::python::extract<double>(neighbourhoods_coords_z[i])
                        )
                    );
        }     

        // covariance
        
        std::vector<kriging_weight_t> weights;
        double variance;
        
        simple_kriging_weights(
			&param.m_sk_params,
                center,
                neighbourhoods_coords,
                weights,
                variance);


        boost::python::list result_weights;
        
        for (int i=0; i< weights.size(); i++)
        {
            result_weights.append(double(weights[i]));
        }

        return result_weights;

    }
}




#endif	/* _PY_KRIGING_WEIGHTS_H */

