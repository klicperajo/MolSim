/*
 * UTest_BoundaryCondition.cpp
 *
 *  Created on: Nov 27, 2014
 *      Author: johannes
 */

#include "UTest_BoundaryCondition.h"
#include "UnitTests/Handler/CountParticles.h"
#include "handler/ForceCalculator_LennardJones.h"
#include "UnitTests/Handler/Check_Reflection.h"
#include "BoundaryCondition/Reflection.h"
#include "BoundaryCondition/Periodic.h"
#include "UnitTests/BoundCondCount.h"

namespace unitTest {

// Registration of the test suite
CPPUNIT_TEST_SUITE_REGISTRATION(UTest_BoundaryCondition);

/**
 * \brief Initializes the domainSize, cutoff radius and cell container
 *
 * The domainSize is set to (40, 40, 40) and the cutoff radius to 3.0.
 * numParticles has to be a multiple of 3.
 */
UTest_BoundaryCondition::UTest_BoundaryCondition() :
		numParticles(99), numHalo(10), domainSize(
				utils::Vector<double, 3>(41)), cutoff(3), cellCont(
				CellContainer(domainSize, cutoff)) {
}

/**
 * \brief Set up a cell container.
 *
 * The cell container is set up with UTest_BoundaryCondition::numParticles particles.
 * The particles are placed along the border of the cell container.
 * Additionally, UTest_BoundaryCondition::numHalo particles are placed inside the
 * halo region, half of them at either end of the diagonal.
 */
void UTest_BoundaryCondition::setUp() {
	double x[] = { 1, 1, 1 };
	double v[] = { 0, 0, 0 };
	double m = 1;
	Particle p;

	cellCont = CellContainer(domainSize, cutoff);
	for (int dim = 0; dim < 3; dim++) {
		for (int i = 0; i < numParticles / 3; i++) {
			x[dim] += 3 * (cellCont.effDomain[dim]-2) / numParticles;
			p = Particle(x, v, m);
			cellCont.add(p);
		}
	}

	//Add particles in halo region
	for (int j = 0; j < 3; j++) {
		x[j] = -cutoff * 0.5;
	}
	for (int i = 0; i < numHalo; i++) {
		if (i >= numHalo / 2) {
			for (int j = 0; j < 3; j++) {
				x[j] = cellCont.effDomain[j] + cutoff / 2;
			}
		}
		p = Particle(x, v, m);
		cellCont.add(p);
	}
}

/**
 * \brief Free the used resources.
 */
void UTest_BoundaryCondition::tearDown() {
	for (int i = 0; i < 6; i++) {
		delete(cellCont.boundConds[i]);
	}
}

/**
 * \brief Test the outflow boundary condition.
 *
 * This is done by imposing this condition and
 * checking afterwards, if all particles but no
 * others were deleted.
 * This method further uses that Outflow is the
 * standard boundary condition.
 */
void UTest_BoundaryCondition::testOutflow() {
	cellCont.impose_boundConds();
	CountParticles cntPart = CountParticles();
	cellCont.iterate_halo(cntPart);
	CPPUNIT_ASSERT(cntPart.cnt == 0);
	CPPUNIT_ASSERT(numParticles == cellCont.size());
}

/**
 * \brief Test the reflection boundary condition.
 *
 *
 */
void UTest_BoundaryCondition::testReflectionCnt() {

	//Impose boundary conditions
	for (int i = 0; i < 6; i++) {
		cellCont.boundConds[i] = new Reflection(&cellCont, i);
	}
	cellCont.impose_boundConds();

	//Check result
	double limit = pow(2,1/6.0) * Particle::def_sigma;

	Check_Reflection check_force(limit, cellCont.effDomain);
	cellCont.iterate_boundary(check_force);
	CPPUNIT_ASSERT(check_force.notZero);
}

/**
 * \brief Test the periodic boundary condition.
 *
 *
 */
void UTest_BoundaryCondition::testPeriodicCnt() {

	//Impose boundary conditions
	for (int i = 0; i < 6; i++) {
		cellCont.boundConds[i] = new Periodic(&cellCont, i);
	}
	cellCont.impose_boundConds();

	/* Impose a second time due to interferences between multiple periodic boundaries.
	 *
	 * Alternative solution: Change modus of iteration in
	 * CellContainer to iterate one boundary after the other.
	 */
	cellCont.impose_boundConds();


	//Check if all boundary particles have virtual particles inside of the domain

	//Count expected number of virtual particles
	for (int i = 0; i < 6; i++) {
		cellCont.boundConds[i] = new BoundCondCount(&cellCont, i, true, false, true);
	}
	cellCont.impose_boundConds();
	int expVirt = 0;
	for (int i = 0; i < 6; i++) {
		expVirt += ((BoundCondCount*)cellCont.boundConds[i])->cntPart;
	}

	CountParticles cntPart = CountParticles();
	cellCont.iterate_halo(cntPart);
	CPPUNIT_ASSERT(cntPart.cnt == expVirt);

	//Check if all particles are now inside of the domain
	CountParticles cntReal = CountParticles(true);
	cellCont.iterate_halo(cntReal);
	CPPUNIT_ASSERT(cntReal.cnt == 0);
	CPPUNIT_ASSERT(numParticles + numHalo == cellCont.size());
}

}
