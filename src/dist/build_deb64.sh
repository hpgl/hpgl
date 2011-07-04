#!/bin/sh
python setup64.py clean
python setup64.py build
python setup64.py bdist_rpm
cp ../bs_gstl/geo.py ./geo/geo.py
cd dist
fakeroot alien HPGL_x64-0.9.2-1.noarch.rpm
rm HPGL_x64-0.9.2.tar.gz
rm HPGL_x64-0.9.2-1.src.rpm
