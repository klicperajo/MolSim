#include "ParticleContainer/ParticleContainer.h"
#include "ParticleContainer/CellContainer.h"
#include "XMLInput/XMLInput.h"
#include "ParticleOutput.h"
#include "handler/PositionCalculator.h"
#include "handler/VelocityCalculator.h"
#include "handler/ForceCalculator.h"

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include <log4cxx/stream.h>
#include <typeinfo>

//UnitTests
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>

using namespace std;
using namespace log4cxx;

/**** forward declaration of the calculation functions ****/

/**
 * \brief Parse parameters.
 * @param argc number of parameters
 * @param argsv array containing the parameter c-strings
 */
void parseParameters(int argc, char* argsv[]);

/**
 * \brief Run a unit test.
 * @param test name of the test to run
 */
int runUnitTest(const char* test);

/**
 * \brief Calculate the force for all particles.
 */
void calculateF();

/**
 * \brief Calculate the position for all particles.
 */
void calculateX();

/**
 * \brief Calculate the velocity for all particles.
 */
void calculateV();

/**
 * \brief Plot the particles.
 */
void plotParticles(int iteration);

// global variables
double start_time = 0.0;	//!< Starting time of the simulation.
double end_time = 1000; 	//!< End time of the simulation.
double delta_t = 0.014; 	//!< Time step size of the simulation.
int    output_freq = 10;	//!< Output frequency of the simulation.

ParticleContainer* particles;		//!< Container for encapsulating the particle list.
ParticleOutput* particleOut = NULL; //!< Object for defining the output method to be used.
PositionCalculator* xcalc = NULL; 	//!< Object for defining the coordinate calculator to be used in the simulation.
VelocityCalculator* vcalc = NULL; 	//!< Object for defining the velocity calculator to be used in the simulation.
ForceCalculator* fcalc = NULL; 		//!< Object for defining the force calculator to be used in the simulation.

LoggerPtr logger(Logger::getLogger("MolSim")); //!< Object for handling general logs.

int main(int argc, char* argsv[]) {

	// configure logging
	PropertyConfigurator::configure("./MolSim.logging.conf");

	LOG4CXX_INFO(logger, "Hello from MolSim for PSE!");

	parseParameters(argc, argsv);

	// the forces are needed to calculate x, but are not given in the input file.
	calculateF();

	double current_time = start_time;

	int iteration = 0;

	// for this loop, we assume: current x, current f and current v are known
	while (current_time < end_time) {

		calculateX(); // calculate new coordinates
		calculateF(); // calculate new forces
		calculateV(); // calculate new velocities

		iteration++;
		if (iteration % output_freq == 0) {
			plotParticles(iteration);
		}
		LOG4CXX_DEBUG(logger, "Iteration " << iteration << " finished.");

		current_time += delta_t;
	}

	LOG4CXX_INFO(logger, "output written. Terminating...");

	return 0;
}

/**
 * Parses the parameters from the commandline
 * and configures the program accordingly.
 */
void parseParameters(int argc, char* argsv[]) {
	bool input_given = false;	// determines whether a XML file was read.

	// commandline must at least contain one parameter
	if (argc < 2) {
		LOG4CXX_FATAL(logger, "Invalid program call!");
		exit(1);
	}

	// handle options
	for (int i = 1; i < argc; i++) {
		if (argsv[i][0] == '-') {
			const char* option = argsv[i] + 1;
			const char* value = NULL;

			// if a value for the option is given, set the variable
			// accordingly. Otherwise the variable is NULL.
			if ((argc > i + 1) && (argsv[i + 1][0] != '-')) {
				value = argsv[i + 1];
				i++;
			}

			if (strcmp(option, "help") == 0) {
				cout << "usage: MolSim OPTIONS...\n"
						"\n"
						"Options:\n"
						"  -help			Prints this information.\n"
						"  -sim XMLFILE		Starts a simulation with parameters read from XMLFILE.\n"
						"  -test [NAME]		Runs a unit test. Optionally the name of the test suite can be specified by NAME.\n"
						"\n" << endl;
			}
			else if (strcmp(option, "test") == 0) {
				LOG4CXX_DEBUG(logger, "starting unit test.");

				int ret_val = runUnitTest(value);
				exit(ret_val);
			}
			else if (strcmp(option, "sim") == 0) {
				if(input_given) {
					LOG4CXX_FATAL(logger, "Error! More than one input file specified.");
					exit(1);
				}

				LOG4CXX_DEBUG(logger, "reading parameters for simulation.");

				if(value == NULL) {
					LOG4CXX_FATAL(logger, "Error! No input file specified.");
					exit(1);
				}

				// read XML file. set global variables accordingly.
				XMLInput input;
				input.ReadFile(value);

				input_given = true;
			}
		} else {
			LOG4CXX_INFO(logger, "Ignored parameter: " << argsv[i]);
		}
	} // end for

	if(!input_given) {
		// no simulation parameters specified, only tests or information.
		exit(0);
	}

	// check if there are any particles
	if (particles->empty()) {
		LOG4CXX_FATAL(logger, "Error! No particles given.");
		exit(1);
	}
}

/**
 * Runs a CppUnit unit test specified by parameter `test`.
 */
int runUnitTest(const char* test) {
	CppUnit::TextUi::TestRunner runner;

	//retrieve instance of TestFactoryRegistry
	CppUnit::TestFactoryRegistry &registry =
			CppUnit::TestFactoryRegistry::getRegistry();

	//retrieve test suite created by the TestFactoryRegistry
	runner.addTest(registry.makeTest());

	//check if a single test or test suite has been chosen
	if (test != NULL) {
		return runner.run(test) ? 0 : 1;
	} else {
		return runner.run() ? 0 : 1;
	}
}

/**
 * The forces are calculated by first resetting them to 0
 * and then using the iteration function in ParticleHandler
 * to call the force calculation method saved in `fcalc` for each particle.
 */
void calculateF() {
	// update OldF and set F to 0.0
	particles->prepare_forces();

	// calculate forces
	particles->iterate_pairs_half(*fcalc);
}

/**
 * The positions are calculated by using the iteration function in ParticleHandler
 * to call the position calculation method saved in `xcalc` for each particle.
 */
void calculateX() {
	particles->iterate_all(*xcalc);
	if (typeid(*particles) == typeid(CellContainer)){
		((CellContainer*)particles)->update_cells();
	}
}

/**
 * The velocities are calculated by using the iteration function in ParticleHandler
 * to call the velocity calculation method saved in `xcalc` for each particle.
 */
void calculateV() {
	particles->iterate_all(*vcalc);
}

/**
 * The particles for this time step are saved using
 * the function defined in the object particleOut.
 */
void plotParticles(int iteration) {
	particleOut->output(iteration);
}
