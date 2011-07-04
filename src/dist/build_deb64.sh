#!/bin/sh
cp ../exe/release/hpgl.so geo/
cp ../exe/release/geo.py geo/

python setup64.py clean
python setup64.py build
python setup64.py bdist_rpm
cd dist
fakeroot alien HPGL_x64-0.9.3-1.noarch.rpm
