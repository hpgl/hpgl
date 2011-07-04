#!/bin/sh
python setup32.py clean
python setup32.py build
python setup32.py bdist_rpm
cp ../hpgl.pyd/geo.py ./geo/geo.py
cp ../exe/release/hpgl.so ./geo/hpgl.so
cp ../exe/release/geo.pyc ./geo/geo.pyc
cd dist
fakeroot alien HPGL_x32-0.9.5-1.noarch.rpm
rm HPGL_x32-0.9.5-1.src.rpm
