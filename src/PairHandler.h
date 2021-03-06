/*
 * PairHandler.h
 *
 *  Created on: 25.10.2014
 *      Author: Jochannes, DanielCAlbert
 */

#ifndef PAIRHANDLER_H_
#define PAIRHANDLER_H_

#include "Particle.h"

/**
 * \brief Abstract class for defining an interface
 *        for processing a pair particles when iterating
 *        over a particle container
 */
class PairHandler {
public:
	PairHandler() {
	}

	virtual ~PairHandler() {
	}

	/*
	 * function to execute for each pair of particles.
	 *
	 * @param p1 First particle of the pair
	 * @param p2 Second particle of the pair
	 */
	virtual void compute(Particle& p1, Particle& p2) = 0;

};

#endif /* PAIRHANDLER_H_ */
