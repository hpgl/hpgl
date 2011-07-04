import unittest
import variogram_routines
import numpy

class EllipsoidTestCase(unittest.TestCase):
    def test_creation(self):
        t = variogram_routines.TVEllipsoid(10, 20, 30, 40, 50, 60)
        self.assertAlmostEqual(t.R1, 10)
        self.assertAlmostEqual(t.R2, 20)
        self.assertAlmostEqual(t.R3, 30)
        
        d1 = [ 0.49240388,
               0.41317591,
               0.76604444 ]

        d2 = [ -0.82959837,
               -0.04341204,
                0.5566704 ]

        d3 = [ 0.26325835,
               -0.90961589,
               0.3213938 ]

        for i in range(3):
            self.assertAlmostEqual(t.Direction1[i, 0], d1[i])
        for i in range(3):
            self.assertAlmostEqual(t.Direction2[i, 0], d2[i])
        for i in range(3):
            self.assertAlmostEqual(t.Direction3[i, 0], d3[i])

class SearchTemplateTestCase(unittest.TestCase):
    def setUp(self):
        self.ell = variogram_routines.TVEllipsoid(10, 20, 30, 40, 50, 60)
        self.templ = variogram_routines.TVVariogramSearchTemplate(10, 10, 0.5, 5, self.ell)
    def test_1(self):
        
        iit = variogram_routines._IsInTunnel(
            self.templ, 
            numpy.array([
                    numpy.matrix(
                        [[ 10, 20, 30]])]))

        self.assert_(iit)

        iit = variogram_routines._IsInTunnel(
            self.templ, 
            numpy.array([
                    numpy.matrix(
                        [[ 100, 20, 30]])]))

        self.failIf(iit)
    def test_window(self):
        mini, minj, mink, maxi, maxj, maxk = variogram_routines._CalcSearchTemplateWindow(self.templ)
        self.assertEqual(mini, -24.489718110804734);
        self.assertEqual(minj, -28.156717480994359);
        self.assertEqual(mink, -20.775222129826481);
        self.assertEqual(maxi, 49.109911936109938);
        self.assertEqual(maxj, 48.815513039321097);
        self.assertEqual(maxk, 59.077444285775378);

    def test_distances(self):
        self.templ.FirstLagDistance = 1
        i,d,s,e = variogram_routines._CalcLagDistances(self.templ)
        self.assertEqual(len(i), 5)
        self.assertEqual(len(d), 5)
        self.assertEqual(len(s), 5)
        self.assertEqual(len(e), 5)

        self.assert_(numpy.all(
            numpy.array([ 0, 11, 22, 33, 44] == d)))
        self.assert_(numpy.all(
            numpy.array([ -5.,   6.,  17.,  28.,  39.] == s)))
        self.assert_(numpy.all(
            numpy.array([  5.,  16.,  27.,  38.,  49.] == e)))

    def test_lag_areas(self):
        I,J,K,Indices,Distances = variogram_routines._CalcLagsAreas(self.templ)
        self.assertEqual(len(I), 76905)
        self.assertEqual(len(J), 76905)
        self.assertEqual(len(K), 76905)
        self.assertEqual(len(Indices), 76905)
        self.assertEqual(len(Distances), 5)

if __name__ == '__main__':
    unittest.main()
