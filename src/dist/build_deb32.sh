#!/bin/sh

cp ../exe/release/hpgl.so geo/
cp ../exe/release/geo.py geo/

python setup32.py clean
python setup32.py build
python setup32.py bdist_rpm
cd dist
fakeroot alien --description="High Perfomance Gestatistics Library (HPGL)" HPGL_x32-0.9.3-1.noarch.rpm
