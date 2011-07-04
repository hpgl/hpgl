#!/bin/sh
python setup32.py clean
python setup32.py build
python setup32.py bdist_rpm
cp ../hpgl.pyd/geo.py ./geo/geo.py
cd dist
fakeroot alien HPGL_x32-0.9.4.noarch.rpm
rm HPGL_x32-0.9.4.tar.gz
rm HPGL_x32-0.9.4-1.src.rpm
