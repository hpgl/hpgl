#
#   Copyright 2009 HPGL Team
#   This file is part of HPGL (High Perfomance Geostatistics Library).
#   HPGL is free software: you can redistribute it and/or modify it under the terms of the BSD License.
#   You should have received a copy of the BSD License along with HPGL.
#

from numpy import *

class TVEllipsoid:
    Direction1 = [1, 0, 0]
    Direction2 = [0, 1, 0]
    Direction3 = [0, 0, 1]
    R1 = 1
    R2 = 1
    R3 = 1
    def __init__(self, R1, R2, R3, Azimut = 0, Dip = 0, Rotation = 0):
        Azimut = deg2rad(Azimut)
        Dip = deg2rad(Dip)
        Rotation = deg2rad(Rotation);
        
        A = matrix([
             [cos(Azimut), -sin(Azimut), 0],
             [sin(Azimut),  cos(Azimut), 0], 
             [0, 0, 1]
             ])
        
        B = matrix([
             [cos(Dip), 0, -sin(Dip)], 
             [0, 1, 0], 
             [sin(Dip), 0,  cos(Dip)]
             ])
        
        C = matrix([
             [1, 0, 0], 
             [0, cos(Rotation), -sin(Rotation)], 
             [0, sin(Rotation),  cos(Rotation)]
             ])
        
        ABC = A * B * C
        
        self.Direction1 = ABC[:, 0]
        self.Direction2 = ABC[:, 1]
        self.Direction3 = ABC[:, 2]
        
        self.R1 = R1
        self.R2 = R2
        self.R3 = R3

class TVVariogramSearchTemplate:
    LagWith = 0.5
    LagSeparation = 1
    TolDistance = 1
    NumLags = 10
    FirstLag = 0
    Ellipsoid = TVEllipsoid(1,  1,  1)
    def __init__(self, LagWidth, LagSeparation, TolDistance, NumLags, Ellipsoid,  FirstLagDistance = 0):
        self.LagWidth  = LagWidth
        self.LagSeparation = LagSeparation
        self.TolDistance = TolDistance
        self.NumLags = NumLags
        self.Ellipsoid = Ellipsoid
        self.FirstLagDistance = FirstLagDistance

def _IsInTunnel(VariogramSearchTemplate, V):
    SS1 = V * VariogramSearchTemplate.Ellipsoid.Direction1
    SS2 = V * VariogramSearchTemplate.Ellipsoid.Direction2
    SS3 = V * VariogramSearchTemplate.Ellipsoid.Direction3
    
    S2 = SS2 / VariogramSearchTemplate.Ellipsoid.R2
    S3 = SS3 / VariogramSearchTemplate.Ellipsoid.R3
    
    Dist = power(power(S2, 2) + power(S3, 2), 0.5)
    Result = array(bitwise_and(Dist <= 1, VariogramSearchTemplate.TolDistance * Dist <= SS1))
        
    return reshape(Result, len(Result))
    
def _CalcSearchTemplateWindow(VariogramSearchTemplate):
    Max = 1E10
    MinI = Max
    MaxI = -Max
    MinJ = Max
    MaxJ = -Max
    MinK = Max
    MaxK = -Max
    for i in range(0, 2):
        for j in range(-1, 2, 2):
            for k in range(-1, 2, 2):
                DI = VariogramSearchTemplate.Ellipsoid.Direction1 * VariogramSearchTemplate.LagSeparation * VariogramSearchTemplate.NumLags * i
                DJ = VariogramSearchTemplate.Ellipsoid.Direction2 * VariogramSearchTemplate.Ellipsoid.R2 * j
                DK = VariogramSearchTemplate.Ellipsoid.Direction3 * VariogramSearchTemplate.Ellipsoid.R3 * k
                V = DI + DJ + DK
                
                MinI = float(min(MinI, V[0]))
                MaxI = float(max(MaxI, V[0]))
                MinJ = float(min(MinJ, V[1]))
                MaxJ = float(max(MaxJ, V[1]))
                MinK = float(min(MinK, V[2]))
                MaxK = float(max(MaxK, V[2]))
    return MinI, MinJ, MinK, MaxI, MaxJ, MaxK

def _CalcLagDistances(VariogramSearchTemplate):
    LagIndexes = range(0, VariogramSearchTemplate.NumLags)
    LagDistance = dot(LagIndexes, VariogramSearchTemplate.LagSeparation + VariogramSearchTemplate.FirstLagDistance)
    LagWidth = VariogramSearchTemplate.LagWidth
    LagStart = LagDistance - LagWidth / 2
    LagEnd = LagDistance + LagWidth / 2
    
    return LagIndexes, LagDistance, LagStart, LagEnd

def _CalcLagsAreas(VariogramSearchTemplate):
    (MinI, MinJ, MinK, MaxI, MaxJ, MaxK) = _CalcSearchTemplateWindow(VariogramSearchTemplate)
    MinI = int(floor(MinI))
    MinJ = int(floor(MinJ))
    MinK = int(floor(MinK))
    MaxI = int(ceil(MaxI))
    MaxJ = int(ceil(MaxJ))
    MaxK = int(ceil(MaxK))
    
    I = zeros([])
    J = zeros([])
    K = zeros([])
    LagIndexes = zeros([])
    
    (Index, LagDistance, LagStart, LagEnd) = _CalcLagDistances(VariogramSearchTemplate)
    
    GI, GJ, GK = mgrid[MinI:MaxI+1, MinJ:MaxJ+1, MinK:MaxK+1]
    
    GI = GI.reshape(prod(GI.shape), 1)
    GJ = GJ.reshape(prod(GJ.shape), 1)
    GK = GK.reshape(prod(GK.shape), 1)
    
    ActivePoints = _IsInTunnel(VariogramSearchTemplate, column_stack((GI, GJ, GK)))
    
    GI = GI[ActivePoints]
    GJ = GJ[ActivePoints]
    GK = GK[ActivePoints]
    
    Dist = power(power(GI, 2) + power(GJ, 2) + power(GK, 2), 0.5)
    
    for i in Index:
        Filter = bitwise_and(LagStart[i] <= Dist, Dist < LagEnd[i])
        NumPoints = sum(Filter)
        I = row_stack((I, GI[Filter].reshape(NumPoints, 1)))
        J = row_stack((J, GJ[Filter].reshape(NumPoints, 1)))
        K = row_stack((K, GK[Filter].reshape(NumPoints, 1)))
        LagIndexes = row_stack((LagIndexes, ones((NumPoints, 1)) * i))
    
    return I[1:], J[1:], K[1:], LagIndexes[1:],  LagDistance

def PointSetScanContStyle(VariogramSearchTemplate, PointSet, Function, Params):
    PX = PointSet['X']
    PY = PointSet['Y']
    PZ = PointSet['Z']
    
    MinX, MinY, MinZ, MaxX, MaxY, MaxZ = _CalcSearchTemplateWindow(VariogramSearchTemplate)
    
    LagIndex, LagDistance, LagStart, LagEnd = _CalcLagDistances(VariogramSearchTemplate)
    MinDistance2 = max(0, min(LagStart)) ** 2
    MaxDistance2 = max(LagEnd) ** 2
    
    if Function != None:
        Result = Function(0, 0, None, Params)  
        Result = reshape(Result, (1, len(Result)))
        Result = repeat(Result, VariogramSearchTemplate.NumLags, 0)
    
    Index = array(range(0, len(PX)))
    for i in xrange(len(PX)):
        X1, Y1, Z1 = PX[i], PY[i], PZ[i]
        DX, DY, DZ = PX - X1, PY - Y1, PZ - Z1
        
        Filter = MinX <= DX
        Filter = bitwise_and(Filter, DX <= MaxX)
        Filter = bitwise_and(Filter, MinY <= DY)
        Filter = bitwise_and(Filter, DY <= MaxY)
        Filter = bitwise_and(Filter, MinY <= DY)
        Filter = bitwise_and(Filter, DY <= MaxY)
        
        FDX, FDY, FDZ = DX[Filter], DY[Filter], DZ[Filter]
        FIndex = Index[Filter]
        
        FDistance2 = FDX ** 2 + FDY ** 2 + FDZ ** 2
        Filter = MinDistance2 <= FDistance2
        Filter = bitwise_and(Filter, FDistance2 <= MaxDistance2)
        
        FDX, FDY, FDZ = FDX[Filter], FDY[Filter], FDZ[Filter]
        FIndex = FIndex[Filter]
        FDistance2 = FDistance2[Filter]
        
        Filter = _IsInTunnel(VariogramSearchTemplate, column_stack((FDX, FDY, FDZ)))
        
        FDX, FDY, FDZ = FDX[Filter], FDY[Filter], FDZ[Filter]
        FIndex = FIndex[Filter]
        FDistance2 = FDistance2[Filter]
        
        FDistance = FDistance2 ** 0.5
        
        for Lag in LagIndex:
            Filter = bitwise_and(LagStart[Lag] <= FDistance, FDistance < LagEnd[Lag])
            for j in FIndex[Filter]:
                Result[Lag, :] = Function(i, j, Result[Lag, :], Params)
    
    return Result, LagDistance

def PointSetScanGridStyle(VariogramSearchTemplate, PointSetXYZ, Function, Params):
    LI,  LJ,  LK,  LagIndexes, LagDistance = _CalcLagsAreas(VariogramSearchTemplate)
    IMin, IMax = min(LI), max(LI)
    JMin, JMax = min(LJ), max(LJ)
    KMin, KMax = min(LK), max(LK)
    
    PI = PointSetXYZ[0]
    PJ = PointSetXYZ[1]
    PK = PointSetXYZ[2]
        
    if Function != None:
        Result = Function(0, 0, None, Params)  
        Result = reshape(Result, (1, len(Result)))
        Result = repeat(Result, VariogramSearchTemplate.NumLags, 0)
    
    Index = array(range(0, len(PI)))
    for i in xrange(len(PI)):
        I1, J1, K1 = PI[i], PJ[i], PK[i]
        DI, DJ, DK = PI - I1, PJ - J1, PK - K1
        
        Filter = IMin <= DI
        Filter = bitwise_and(Filter, DI <= IMax)
        Filter = bitwise_and(Filter, JMin <= DJ)
        Filter = bitwise_and(Filter, DJ <= JMax)
        Filter = bitwise_and(Filter, KMin <= DK)
        Filter = bitwise_and(Filter, DK <= KMax)
        
        FPI, FPJ, FPK = PI[Filter], PJ[Filter], PK[Filter]
        FDI, FDJ, FDK = FPI - I1, FPJ - J1, FPK - K1
        FIndex = Index[Filter]
        
        for j in xrange(len(FDI)):
            LFilter = FDI[j] == LI
            LFilter = bitwise_and(LFilter, FDJ[j] == LJ)
            LFilter = bitwise_and(LFilter, FDK[j] == LK)
            
            ActiveLags = LagIndexes[LFilter]
            
            if Function != None:
                I2, J2, K2 = FPI[j], FPJ[j], FPK[j]
                for Lag in ActiveLags:
                    Result[Lag, :] = Function(i, FIndex[j], Result[Lag, :], Params)
    
    return Result, LagDistance

def CalcVariogramFunction(Point1, Point2, Result, Params):
    Values = Params['HardData']
    NumValues = len(Values)
    if Result == None:
        Result = zeros(NumValues + NumValues + 1, dtype=float32)
    else:
        Values1 = zeros(NumValues)
        Values2 = zeros(NumValues)
        for i in xrange(NumValues):
            Values1[i] = Values[i][Point1]
            Values2[i] = Values[i][Point2]
        Variances = float32(Values1 - Values2)**2
        Result[NumValues + 0:NumValues + NumValues] = Result[NumValues + 0:NumValues + NumValues] + Variances
        Result[NumValues + NumValues] += 1
        Result[0:NumValues] = Result[NumValues + 0:NumValues + NumValues] / Result[NumValues + NumValues] / 2
    return Result

def CalcCovarianceFunction(Point1, Point2, Result, Params):
    Values = Params['HardData']
    SoftData = Params['SoftData']
    NumValues = len(Values)
    if Result == None:
        Result = zeros(NumValues + NumValues + 1, dtype=float32)
    else:
        Values1 = zeros(NumValues)
        Values2 = zeros(NumValues)
        SoftValues1 = zeros(NumValues)
        SoftValues2 = zeros(NumValues)
        for i in xrange(NumValues):
            Values1[i] = Values[i][Point1]
            Values2[i] = Values[i][Point2]
            SoftValues1[i] = SoftData[i][Point1]
            SoftValues2[i] = SoftData[i][Point2]
        Covariances = float32((Values1 - SoftValues1)*(Values2 - SoftValues2))
        Result[NumValues + 0:NumValues + NumValues] = Result[NumValues + 0:NumValues + NumValues] + Covariances
        Result[NumValues + NumValues] += 1
        Result[0:NumValues] = Result[NumValues + 0:NumValues + NumValues] / Result[NumValues + NumValues] / 2
    return Result

def CalcIndCorrelationFunction(Point1, Point2, Result, Params):
    Values = Params['HardData']
    SoftData = Params['SoftData']
    NumValues = len(Values)
    if Result == None:
        Result = zeros(NumValues + NumValues + 1, dtype=float32)
    else:
        Values1 = zeros(NumValues)
        Values2 = zeros(NumValues)
        SoftValues1 = zeros(NumValues)
        SoftValues2 = zeros(NumValues)
        for i in xrange(NumValues):
            Values1[i] = Values[i][Point1]
            Values2[i] = Values[i][Point2]
            SoftValues1[i] = SoftData[i][Point1]
            SoftValues2[i] = SoftData[i][Point2]
        Covariances = float32((Values1 - SoftValues1)*(Values2 - SoftValues2) / (SoftValues1 * (1 - SoftValues1) * SoftValues2 * (1-SoftValues2)) ** 0.5)
        Result[NumValues + 0:NumValues + NumValues] = Result[NumValues + 0:NumValues + NumValues] + Covariances
        Result[NumValues + NumValues] += 1
        Result[0:NumValues] = Result[NumValues + 0:NumValues + NumValues] / Result[NumValues + NumValues] / 2
    return Result
