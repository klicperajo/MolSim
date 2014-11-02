/*
 * ForceCalculatorLennardJones.cpp
 *
 *  Created on: Oct 30, 2014
 *      Author: johannes
 */

#include "ForceCalculator_LennardJones.h"

ForceCalculator_LennardJones::ForceCalculator_LennardJones() {
}

ForceCalculator_LennardJones::~ForceCalculator_LennardJones() {
}

/**
 * \brief Function for computing the forces caused by the Lennard-Jones-Potential.
 *
 * Calculates the forces between two particles caused by the Lennard-Jones-Potential
 * and adds it to the force acting on the first particle.
 * The force is calculated using the following equation:
 * \f[
 * F_{ij} = \frac{24 \cdot \epsilon}{\left( \left\lVert x_i - x_j \right\rVert _2 \right) ^2} \cdot \left( \left( \frac{\sigma}{\left\lVert x_i - x_j \right\rVert _2}^6 - 2 \cdot \left( \frac{\sigma}{\left\lVert x_i - x_j \right\rVert _2} \right) ^{12} \right) \cdot \left( x_j - x_i \right)
 * \f]
 */
void ForceCalculator_LennardJones::compute(Particle& P1, Particle& P2)
{
	double sigma = 1;
	double epsilon = 1;

	double recDistance = 1 / (P1.getX() - P2.getX()).L2Norm();

	utils::Vector<double, 3> force = P2.getX() - P1.getX(); //directional vector
	force = (pow(sigma*recDistance,6) - 2 * pow(sigma*recDistance,12)) * force; //bracket
	force = 24 * epsilon * recDistance * recDistance * force; //prefactor

	P1.getF() = P1.getF() + force;
	//P2.getF() = P1.getF() - force;		// according to Newton's third law: F_P1 = -F_P2, use alternative iteration method
}
