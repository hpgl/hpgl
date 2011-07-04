#include "geovalue.h"
#include "cartesian_grid.h"


geovalue::property_type& geovalue::property_value() {
  assert(is_informed());
  return grid_->prop_value_at(index_);
}

const geovalue::property_type& geovalue::property_value() const {
  assert(is_informed());
  return grid_->prop_value_at(index_);
}

geovalue::location_type geovalue::location() {
  assert(is_informed());
  return grid_->location_of(index_);
}

const geovalue::location_type geovalue::location() const {
  assert(is_informed());
  return grid_->location_of(index_);
}

bool geovalue::is_informed() const{
  return index_ < grid_->size();
}
