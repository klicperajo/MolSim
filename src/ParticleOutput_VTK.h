/*
 * ParticleOutput_VTK.h
 *
 *  Created on: 26.10.2014
 *      Author: Jochannes, DanielCAlbert
 */

#ifndef PARTICLEOUTPUT_VTK_H_
#define PARTICLEOUTPUT_VTK_H_

#include "ParticleOutput.h"

#include <string>


/**
 * \brief Class implementing particle output to a VTK-file using the class outputWriter.VTKWriter.
 */
class ParticleOutput_VTK : public ParticleOutput
{
protected:
	std::string out_name;

public:
	ParticleOutput_VTK(ParticleContainer& param_container, std::string param_out_name = "MD_vtk")
		: ParticleOutput(param_container), out_name(param_out_name) {
	}

	virtual ~ParticleOutput_VTK() {
	}

	virtual void output(int iteration);

};


#endif /* PARTICLEOUTPUT_VTK_H_ */