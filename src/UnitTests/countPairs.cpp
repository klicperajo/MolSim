/*
 * countPairs.cpp
 *
 *  Created on: Nov 3, 2014
 *      Author: johannes
 */

#include "countPairs.h"

countPairs::countPairs() {
	cnt = 0;
}

countPairs::~countPairs() {
}

/**
 * \brief This method increments cnt by 1.
 */
void countPairs::compute(Particle& P1, Particle& P2)
{
	cnt++;
}
