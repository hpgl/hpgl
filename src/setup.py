from setuptools import *
setup(
    name = "HPGL-BSD",
    version = "0.9.7 Xmass Edition",
    #scripts = ['geo.py'],
	include_package_data = True,
	#package_dir = {'': 'package'},
	packages = ['geo_bsd'],
	package_data = {'geo_bsd': ['*.py'], 'geo_bsd':['*.dll'],'geo_bsd':['*.pyd'],'geo_bsd':['*.pdb'], 'geo_bsd':['*.so']},

    # metadata for upload to PyPI
    author = "Artur Muharlyamov, Andrew Bezrukov, Konstantin Barsky",
    author_email = "muharlyamovar@ufanipi.ru, barskykd@ufanipi.ru, bezrukovav@ufanipi.ru",
    description = "HPGL: High Perfomance Geostatistical Library (BSD Release)",
    license = "BSD",
    keywords = ["geostatistics"]
    #url = "---",   # project home page, if any

    # could also include long_description, download_url, classifiers, etc.
)
