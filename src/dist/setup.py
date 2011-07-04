#
#
#   Copyright 2009 HPGL Team
#
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2 of the License.
#
#   HPGL is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along with HPGL. If not, see http://www.gnu.org/licenses/.
#
from setuptools import setup, find_packages
setup(
    name = "HPGL",
    version = "0.9.2",
    #scripts = ['geo.py'],
	include_package_data = True,
	#package_dir = {'': 'package'},
	packages = ['geo'],
	package_data = {'geo': ['*.*'], 'geo':['*.so']},

    # metadata for upload to PyPI
    author = "Artur Muharlyamov, Andrew Bezrukov, Konstantin Barsky",
    author_email = "muharlyamovar@ufanipi.ru, barskykd@ufanipi.ru, bezrukovav@ufanipi.ru",
    description = "HPGL: High Perfomance Geostatistical Library",
    license = "---",
    keywords = ["geostatistics"]
    #url = "---",   # project home page, if any

    # could also include long_description, download_url, classifiers, etc.
)
