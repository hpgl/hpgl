#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from setuptools import setup, find_packages
setup(
    name = "HPGL_Commercial",
    version = "0.9.6-py2.6",
    #scripts = ['geo.py'],
	include_package_data = True,
	#package_dir = {'': 'package'},
	packages = ['geo'],
	package_data = {'geo': ['geo'], 'geo':['misc']},

    # metadata for upload to PyPI
    author = "Artur Muharlyamov, Andrew Bezrukov, Konstantin Barsky, Nasibullina Dina",
    author_email = "muharlyamovar@ufanipi.ru, barskykd@ufanipi.ru, bezrukovav@ufanipi.ru",
    description = "HPGL: High Perfomance Geostatistical Library",
    license = "Commercial",
    keywords = ["geostatistics"]
    #url = "---",   # project home page, if any

    # could also include long_description, download_url, classifiers, etc.
)
