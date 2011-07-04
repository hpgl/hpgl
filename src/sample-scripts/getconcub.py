from numpy import *
from scipy import *
from pylab import *
from python_property import *
from sys import *

def getconcube(cube):
    #cube = load_property_python(x,y,z,filename,intype)
    [x,y,z]=cube.shape
    conncube = zeros( (x,y,z), dtype=int16 )
    qI = zeros( (x*y*z), dtype=int16 )
    qJ = zeros( (x*y*z), dtype=int16 )
    qK = zeros( (x*y*z), dtype=int16 )
    compnum = 0
    for i in range(x):
        for j in range(y):
            for k in range(z):
                if (cube[i,j,k] == 1) and (conncube[i,j,k] == 0):
                    compnum = compnum + 1
                    conncube[i,j,k] = compnum
                    iqb = -1
                    iqe = 0
                    qI[iqe] = i
                    qJ[iqe] = j
                    qK[iqe] = k
                    while (iqb<iqe):
                        iqb = iqb + 1
                        ni = qI[iqb]
                        nj = qJ[iqb]
                        nk = qK[iqb]
                        if ni>0:
                            if (cube[ni-1,nj,nk] == 1) and (conncube[ni-1,nj,nk] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni-1
                                qJ[iqe] = nj
                                qK[iqe] = nk
                                conncube[ni-1,nj,nk] = compnum
                        if nj>0:
                            if (cube[ni,nj-1,nk] == 1) and (conncube[ni,nj-1,nk] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni
                                qJ[iqe] = nj-1
                                qK[iqe] = nk
                                conncube[ni,nj-1,nk] = compnum
                        if nk>0:
                            if (cube[ni,nj,nk-1] == 1) and (conncube[ni,nj,nk-1] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni
                                qJ[iqe] = nj
                                qK[iqe] = nk-1
                                conncube[ni,nj,nk-1] = compnum
                        if ni<(x-1):
                            if (cube[ni+1,nj,nk] == 1) and (conncube[ni+1,nj,nk] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni+1
                                qJ[iqe] = nj
                                qK[iqe] = nk
                                conncube[ni+1,nj,nk] = compnum
                        if nj<(y-1):
                            if (cube[ni,nj+1,nk] == 1) and (conncube[ni,nj+1,nk] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni
                                qJ[iqe] = nj+1
                                qK[iqe] = nk
                                conncube[ni,nj+1,nk] = compnum
                        if nk<(z-1):
                            if (cube[ni,nj,nk+1] == 1) and (conncube[ni,nj,nk+1] == 0):
                                iqe = iqe + 1
                                qI[iqe] = ni
                                qJ[iqe] = nj
                                qK[iqe] = nk+1
                                conncube[ni,nj,nk+1] = compnum
    save_property_python(conncube,x,y,z,"SAVE_CUB.INC")
