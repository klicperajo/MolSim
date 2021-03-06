/*
 * FileReader.h
 *
 *  Created on: 23.02.2010
 *      Author: eckhardw
 */

#ifndef FILE_READER_H_
#define FILE_READER_H_

#include "Particle.h"
#include <list>

/**
 * \brief Class providing a method for reading particle information from an input file.
 */
class FileReader {

public:
	FileReader();
	virtual ~FileReader();

	void readFile(std::list<Particle>& particles, const char* filename);

};

#endif /* FILE_READER_H_ */
