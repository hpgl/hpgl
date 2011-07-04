#include <stdlib.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "api.h"

class variogram_test : public CppUnit::TestFixture
{
public:
	void setUp(){}
	void tearDown(){}
	void test1()
	{
		ellipsoid_t ell = {0};
		ell.m_R1 = 10;
		ell.m_R2 = 20;
		ell.m_R3 = 30;

		fill_ellipsoid_directions(&ell, 40, 50, 60);

		double d1[] = {0.49240388,
					   0.41317591,
					   0.76604444};
		double d2[] = { -0.82959837,
						-0.04341204,
						0.5566704};
		double d3[] = {0.26325835,
					   -0.90961589,
					   0.3213938};
		for (int i = 0; i < 3; ++i)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(d1[i], ell.m_direction1.m_data[i], 0.0001);
		}
		for (int i = 0; i < 3; ++i)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(d2[i], ell.m_direction2.m_data[i], 0.0001);
		}
		for (int i = 0; i < 3; ++i)
		{
			CPPUNIT_ASSERT_DOUBLES_EQUAL(d3[i], ell.m_direction3.m_data[i], 0.0001);
		}
	}

	void test2()
	{
		variogram_search_template_t templ = {0};
		templ.m_ellipsoid.m_R1 = 10;
		templ.m_ellipsoid.m_R2 = 20;
		templ.m_ellipsoid.m_R3 = 30;

		fill_ellipsoid_directions(&templ.m_ellipsoid, 40, 50, 60);

		templ.m_lag_width = 10;
		templ.m_lag_separation = 10;
		templ.m_tol_distance = 0.5;
		templ.m_num_lags = 5;
	
		vector_t v1 = {0};
		vector_t v2 = {0};

		v1.m_data[0] = 10;
		v1.m_data[1] = 20;
		v1.m_data[2] = 30;

		v2.m_data[0] = 100;
		v2.m_data[1] = 20;
		v2.m_data[2] = 30;

		CPPUNIT_ASSERT_EQUAL(false, is_in_tunnel(&templ, &v2));
	}

	void test_window()
	{
		variogram_search_template_t templ = {0};
		templ.m_ellipsoid.m_R1 = 10;
		templ.m_ellipsoid.m_R2 = 20;
		templ.m_ellipsoid.m_R3 = 30;

		fill_ellipsoid_directions(&templ.m_ellipsoid, 40, 50, 60);

		templ.m_lag_width = 10;
		templ.m_lag_separation = 10;
		templ.m_tol_distance = 0.5;
		templ.m_num_lags = 5;

		search_template_window_t window = {0};

		calc_search_template_window(&templ, &window);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_min_i, -24.489718110804734, 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_min_j, -28.156717480994359, 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_min_k, -20.775222129826481, 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_max_i, 49.109911936109938, 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_max_j, 48.815513039321097, 0.0001);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(window.m_max_k, 59.077444285775378, 0.0001);
	}

	void test_lag_areas()
	{
		variogram_search_template_t templ = {0};
		templ.m_ellipsoid.m_R1 = 10;
		templ.m_ellipsoid.m_R2 = 20;
		templ.m_ellipsoid.m_R3 = 30;
		
		fill_ellipsoid_directions(&templ.m_ellipsoid, 40, 50, 60);

		templ.m_lag_width = 10;
		templ.m_lag_separation = 10;
		templ.m_tol_distance = 0.5;
		templ.m_num_lags = 5;

        int points_count;

        free(calc_lag_areas(&templ, &points_count));
        CPPUNIT_ASSERT_EQUAL(points_count, 76905);
	}

	CPPUNIT_TEST_SUITE(variogram_test);
	CPPUNIT_TEST(test1);
	CPPUNIT_TEST(test2);
	CPPUNIT_TEST(test_window);
	CPPUNIT_TEST(test_lag_areas);
	CPPUNIT_TEST_SUITE_END();
};

CPPUNIT_TEST_SUITE_REGISTRATION(variogram_test);

int main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSuccessful = runner.run( "", false );
  return wasSuccessful;
}
