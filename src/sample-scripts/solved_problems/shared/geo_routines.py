#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from geo import *
from numpy import *
import numpy.ma as ma
from numpy import savetxt

def CalcMean(Cube, Mask):
    CubeMasked = ma.masked_array(Cube, Mask == False)
    return CubeMasked.mean()

def CalcMarginalProbsIndicator(Cube, Mask, Indicators):
    Result = zeros(len(Indicators))
    for i in xrange(len(Indicators)):
        Result.flat[i] = CalcMean(Cube == Indicators[i], Mask)
    return Result

def CalcVPC(Cube, Mask, MarginalMean):
    NZ = Cube.shape[2]
    MaskSum = Mask.sum(0).sum(0)
    CubeMasked = Cube
    CubeMasked[Mask == False] = 0
    
    CubeSum = Cube.sum(0).sum(0)
    Result = ones(NZ) * MarginalMean
    Filter = MaskSum > 0
    Result[Filter] = float32(CubeSum[Filter]) / float32(MaskSum[Filter])
    
    return Result
    
def CalcVPCsIndicator(Cube, Mask, Indicators, MarginalProbs):
    Result = []
    for i in xrange(len(Indicators)):
        VPC = CalcVPC(Cube == Indicators[i], Mask, MarginalProbs[i])
        Result.append(VPC)
    
    return array(Result)

def CubeFromVPC(VPC, NX, NY):
    NZ = len(VPC)
    VPC = reshape(VPC, (1, 1, NZ))
    Cube = repeat(repeat(VPC, NX, axis = 0), NY, axis = 1)
    return float32(Cube)
    
def CubesFromVPCs(VPCs, NX, NY):
    Cubes = []
    for i in xrange(len(VPCs)):
        Cube = CubeFromVPC(VPCs[i], NX, NY)
        Cubes.append(Cube)
    return Cubes

def Cubes2PointSet(CubesDictionary, Mask):
    NX, NY, NZ = CubesDictionary.values()[0].shape
    I, J = mgrid[0:NX, 0:NY]
    PointSet = {'X':zeros(0, dtype=int32), 'Y':zeros(0, dtype=int32), 'Z':zeros(0, dtype=int32)}
    for Key in CubesDictionary.keys():
        PointSet[Key] = zeros(0, dtype=int32)
    
    for k in xrange(NZ):
        Slice = Mask[:, :, k]
        PointSet['X'] = append(PointSet['X'], I[Slice])
        PointSet['Y'] = append(PointSet['Y'], J[Slice])
        PointSet['Z'] = append(PointSet['Z'], k * ones(Slice.sum(0).sum(0), dtype=int32))
        for Key in CubesDictionary.keys():
            DataSlice = CubesDictionary[Key][:, :, k]
            PointSet[Key] = append(PointSet[Key], DataSlice[Slice])
        
    return PointSet

def Cube2PointSet(Cube, Mask):
    NX, NY, NZ = Cube.shape
    I, J = mgrid[0:NX, 0:NY]
    X = zeros(0, dtype=int32)
    Y = zeros(0, dtype=int32)
    Z = zeros(0, dtype=int32)
    Property = zeros(0, dtype=int32)
    for k in xrange(NZ):
        Slice = Mask[:, :, k]
        X = append(X, I[Slice])
        Y = append(Y, J[Slice])
        Z = append(Z, k * ones(Slice.sum(0).sum(0), dtype=int32))
        DataSlice = Cube[:, :, k]
        Property = append(Property, DataSlice[Slice])
    return X, Y, Z, Property
   
def PointSet2Cube(X, Y, Z, Property, Cube):
    NX, NY, NZ = Cube.shape
    for Ind in xrange(len(X.flat)):
        if (0 <= X[Ind]) & (X[Ind] < NX) & (0 <= Y[Ind]) & (Y[Ind] < NY) & (0 <= Z[Ind]) & (Z[Ind] < NZ): 
            Cube[X[Ind], Y[Ind], Z[Ind]] = Property[Ind]
    return Cube

def SaveGSLIBPointSet(PointSet, FileName, Caption):
    f = open(FileName, "w")
    
    # 1. Caption
    f.write(Caption + '\n')
    
    # 2. Number of properties in file
    f.write(str(len(PointSet)) + '\n')
    
    # 3. Properties names
    lens = numpy.array([])
    
    for Key in PointSet.keys():
        f.write(Key + '\n')
        lens = numpy.append(lens, len(PointSet[Key].flat))
    
    # Check that all properties have the same length
    if(sum(lens - lens[0]) == 0):
        MegaPointSet = zeros((lens[0], 0))
        for Key in PointSet.keys():
            MegaPointSet = column_stack((MegaPointSet, PointSet[Key]))
        savetxt(f, MegaPointSet)
    else:
        print "ERROR! All properties in GSLIB dictionary must have equal size"
    
    f.close()

def SaveGSLIBCubes(CubesDictionary, FileName, Caption, Format = "%d"):
    f = open(FileName, "w")
    
    # 1. Caption
    f.write(Caption + '\n')
    
    # 2. Number of properties in file
    f.write(str(len(CubesDictionary)) + '\n')
    
    # 3. Properties names
    lens = numpy.array([])
    
    for Key in CubesDictionary.keys():
        f.write(Key + '\n')
        lens = numpy.append(lens, len(CubesDictionary[Key].flat))
    
    # Check that all properties have the same length
    if(sum(lens - lens[0]) == 0):
        MegaCube = zeros((lens[0], 0))
        for Key in CubesDictionary.keys():
            MegaCube = column_stack((MegaCube, CubesDictionary[Key].copy().swapaxes(0, 2).swapaxes(1, 2).flat))
        savetxt(f, MegaCube, Format)
    else:
        print "ERROR! All properties in GSLIB dictionary must have equal size"
    
    f.close()
