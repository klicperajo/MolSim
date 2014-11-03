/*
 * countPairs.h
 *
 *  Created on: Nov 3, 2014
 *      Author: johannes
 */

#ifndef COUNTPAIRS_H_
#define COUNTPAIRS_H_

#include "Particle.h"
#include "PairHandler.h"

/**
 * \brief Class for counting the number of iterated particle pairs for test purposes.
 */
class countPairs: public PairHandler
{
public:
	int cnt;
	countPairs();
	virtual ~countPairs();

	virtual void compute(Particle& P1, Particle& P2);
};

#endif /* COUNTPAIRS_H_ */
