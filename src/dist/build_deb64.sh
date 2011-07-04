#!/bin/sh
python setup64.py clean
python setup64.py build
python setup64.py bdist_rpm
cp ../hpgl.pyd/geo.py ./geo/geo.py
cp ../exe/release/hpgl.so ./geo/hpgl.so
cd dist
fakeroot alien HPGL_x64-0.9.5-1.noarch.rpm
rm HPGL_x64-0.9.5-1.src.rpm
