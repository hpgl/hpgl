copy /Y ..\exe\release\hpgl.pyd geo\
copy /Y ..\exe\release\geo.py geo\
copy /Y ..\exe\release\boost_python-vc80-mt-1_35.dll geo\
copy /Y ..\exe\release\hpgl.pdb geo\
copy /Y __init__.py geo\
copy /Y ..\sample-scripts\*.py misc\sample-scripts\
python setup.py clean
python setup.py build
python setup.py bdist_wininst
python setup.py bdist_egg
