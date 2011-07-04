del /F /Q dist\*
del /F /Q geo\*

:: --- Python 2.5 ---

C:\Python25\python.exe setup.py clean
:: geo
copy /Y ..\exe\release\geo\hpgl.pyd geo\
copy /Y ..\geo\* geo\
copy /Y ..\exe\release\geo\hpgl.pdb geo\
:: submodules
copy /Y ..\exe\release\geo\variogram.py geo\
copy /Y ..\exe\release\geo\routines.py geo\
:: building
C:\Python25\python.exe setup.py build
C:\Python25\python.exe setup.py bdist_wininst
move /Y dist\HPGL-Commercial-0.9.6.win32.exe dist\HPGL-Commercial-0.9.6-py2.5.win32.exe 

:: --- Python 2.6 ---

C:\Python26\python.exe setup.py clean
:: geo
copy /Y ..\exe\release\geo\hpgl.pyd geo\
copy /Y ..\geo\* geo\
copy /Y ..\exe\release\geo\hpgl.pdb geo\
:: submodules
copy /Y ..\exe\release\geo\variogram.py geo\
copy /Y ..\exe\release\geo\routines.py geo\
:: building
C:\Python26\python.exe setup.py build
C:\Python26\python.exe setup.py bdist_wininst
move /Y dist\HPGL-Commercial-0.9.6.win32.exe dist\HPGL-Commercial-0.9.6-py2.6.win32.exe 
