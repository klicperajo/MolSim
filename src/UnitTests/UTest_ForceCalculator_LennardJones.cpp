/*
 * UTest_ForceCalculator_LennardJones.cpp
 *
 *  Created on: Nov 4, 2014
 *      Author: johannes
 */

#include "UTest_ForceCalculator_LennardJones.h"

namespace unitTest {

// Registration of the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(UTest_ForceCalculator_LennardJones);

void UTest_ForceCalculator_LennardJones::setUp() {
	double x[] = { 0, 0, 0 };
	double v[] = { 1, 1, 1 };
	double m = 1;
	p1 = Particle(x, v, m, 1);
	p2 = Particle(x, v, m, 2);
}

/**
 * \brief Free the used resources.
 */
void UTest_ForceCalculator_LennardJones::tearDown() {
}

/**
 * \brief Method for testing if the force calculated by ForceCalculator_LennardJones is 0 in the potential well.
 */
void UTest_ForceCalculator_LennardJones::testPotentialWell() {

	//Test potential well for x-coordinate
	p2.getX()[0] = r_well;
	fLennard.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));
	fLJ_cutoff.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));

	//Test potential well for y-coordinate
	p2.getX()[0] = 0;
	p2.getX()[1] = r_well;
	fLennard.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));
	fLJ_cutoff.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));

	//Test potential well for z-coordinate
	p2.getX()[1] = 0;
	p2.getX()[2] = r_well;
	fLennard.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));
	fLJ_cutoff.compute(p1, p2);
	CPPUNIT_ASSERT(p1.getF().L2Norm() < pow(10, -12));
}

/**
 * \brief Method for testing if the force is attractive at farther distances than the potential well.
 */
void UTest_ForceCalculator_LennardJones::testAttraction() {

	//Test the force for 19 distances farther than the potential well
	for (int i = 1; i < 20; i++) {
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		p2.getX()[0] = r_well * (1 + i * 0.1);
		fLennard.compute(p1, p2);
		CPPUNIT_ASSERT(p1.getF()[0] > 0);
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		fLJ_cutoff.compute(p1, p2);
		CPPUNIT_ASSERT(p2.getX()[0] > cutoff || p1.getF()[0] > 0);
	}
}

/**
 * \brief Method for testing if the force is repulsive at distances closer than the potential well.
 */
void UTest_ForceCalculator_LennardJones::testRepulsion() {

	//Test the force for 19 distances closer than the potential well
	for (int i = 1; i < 20; i++) {
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		p2.getX()[0] = r_well * (1 - i * 0.05);
		fLennard.compute(p1, p2);
		CPPUNIT_ASSERT(p1.getF()[0] < 0);
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		fLJ_cutoff.compute(p1, p2);
		CPPUNIT_ASSERT(p1.getF()[0] < 0);
	}
}

/**
 * \brief Method for testing if the force is equal for both particles (Newton's third law).
 */
void UTest_ForceCalculator_LennardJones::testNewton() {

	//Test the force for 29 distances, both closer and farther than the potential well
	for (int i = 1; i < 30; i++) {
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		p2.getX()[0] = r_well * (i * 0.1);
		fLennard.compute(p1, p2);
		CPPUNIT_ASSERT((p1.getF() + p2.getF()).L2Norm() == 0);
		p1.getF() = 0.0;
		p2.getF() = 0.0;
		fLJ_cutoff.compute(p1, p2);
		CPPUNIT_ASSERT((p1.getF() + p2.getF()).L2Norm() == 0);
	}
}

}
