/*
 * ForceCalculator_Gravity.h
 *
 *  Created on: 25.10.2014
 *      Author: Jochannes, DanielCAlbert
 */

#ifndef FORCECALCULATOR_GRAVITY_H_
#define FORCECALCULATOR_GRAVITY_H_


#include "ForceCalculator.h"
#include "Particle.h"


/**
 * \brief Class for adding gravitational force to particles.
 */
class ForceCalculator_Gravity : public ForceCalculator
{
public:
	double g;		//!< Acceleration coefficient.
	unsigned int direction;	//!< Direction. 0: x-axis; 1: y-axis (default); 2: z-axis
	int type;		//!< Type of particles to which the gravity shall be exclusively applied. All particles: -1 (default)

	ForceCalculator_Gravity(double arg_g, unsigned int arg_direction = 1, int arg_type = -1);
	virtual ~ForceCalculator_Gravity();

	/**
	 * /brief Function for adding the gravitational force to a particle.
	 */
	virtual void compute(Particle& p);

	/**
	 * /brief Function for calculating the force between two particles.
	 *
	 * Since this is a non-interacting force, this method does nothing.
	 */
	virtual void compute(Particle& p1, Particle& p2) {}
};


#endif /* FORCECALCULATOR_GRAVITY_H_ */
