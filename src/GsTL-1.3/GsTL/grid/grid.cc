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

#include <GsTL/math/random_number_generators.h>


template<class PropertyServer, class GeometryServer>
grid<PropertyServer,GeometryServer>::grid() {
  geom_server_ = 0;
  prop_server_ = 0;
  is_informed_ = 0;
  regions_ = 0;
  random_path_ = 0;
}

template<class PropertyServer, class GeometryServer>
grid<PropertyServer,GeometryServer>::grid(GeometryServer* gserver) 
  : geom_server_(gserver) {
  
  prop_server_ = new PropertyServer(geom_server_->size(), -1);
  is_informed_ = new flag_server(geom_server_->size(), 0);

  regions_ = new flag_server(geom_server_->size(), 0);

  random_path_ = 0;
}



template<class PropertyServer, class GeometryServer>
inline void grid<PropertyServer,GeometryServer>::set_geometry(GeometryServer* gserver) {
  geom_server_ = gserver;

  prop_server_ = new PropertyServer(geom_server_->size(), -1);
  is_informed_ = new flag_server(geom_server_->size(), 0);

  regions_ = new flag_server(geom_server_->size(), 0);

}


template<class PropertyServer, class GeometryServer>
grid<PropertyServer,GeometryServer>::~grid() {
  delete prop_server_;
  delete is_informed_;
  delete regions_;

  if(random_path_ != 0)
    delete [] random_path_;
}




template<class PropertyServer, class GeometryServer>
template<class RandomNumberGenerator>
void
grid<PropertyServer,GeometryServer>::initialize_random_path(RandomNumberGenerator& generator) {
  
  if(random_path_ == 0) {
    random_path_ = new int[size()];
    for(int i=0; i<size(); i++)
      random_path_[i] = i;
  }
  STL_generator_adaptor_t<RandomNumberGenerator> stl_gen(generator);
  std::random_shuffle(random_path_, random_path_+size(),
                      stl_gen );


}






#ifdef __GNUC__
#pragma implementation
#endif
#include <GsTL/grid/grid.h>



