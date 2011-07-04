#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

f = open('RESULT_SIS.INC')
lines = f.readlines()
d = {}
for line in lines:
  if line in d:
    d[line] += 1
  else:
    d[line] = 1

print d
