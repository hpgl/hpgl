echo on
xcopy /F /E /I /Y /D test_data %1\test_data
xcopy /F /E /I /Y /D fixed %1\fixed
xcopy /F /E /I /Y /D support_scripts %1
xcopy /F /E /I /Y /D test_scripts %1
if not exist %1\results mkdir %1\results