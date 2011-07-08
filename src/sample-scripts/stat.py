f = open('RESULT_SIS.INC')
lines = f.readlines()
d = {}
for line in lines:
  if line in d:
    d[line] += 1
  else:
    d[line] = 1

print d
