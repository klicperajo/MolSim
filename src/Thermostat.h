/*
 * Thermostat.h
 *
 *  Created on: 01.12.2014
 *      Author: Jochannes, DanielCAlbert
 */

#ifndef THERMOSTAT_H_
#define THERMOSTAT_H_

#include "ParticleContainer/ParticleContainer.h"
#include "handler/TemperatureAdjustHandler.h"
#include "handler/KineticEnergyHandler.h"
#include "UnitTests/UTest_Thermostat.h"

#include <cmath>

/**
 * /brief A Thermostat controlling the temperatures of a ParticleContainer.
 *
 * The thermostat sets the velocities of the particles after a specified amount
 * of iterations to match a specific temperature.
 * The said temperature can be constant or rising/falling to a specified target
 * temperature. The temperature will then be changed gradually in temperature
 * steps after a specified amount iterations - until the target is reached.
 */
class Thermostat {
	friend class unitTest::UTest_Thermostat;

private:
	ParticleContainer& particles;//!< ParticleContainer which temperatures the thermostat should control.

	const int num_dimensions;		//!< number of dimensions
	const int steps_thermostat;	//!< number of time steps after which the thermostat is applied

	const double target_temp;//!< temperature of the particles the thermostat is targeting.
	double next_temp;//!< temperature of the particles the thermostat will apply in the next step.
	const bool constant;//!< determines whether the target temperature is constant.
	const int steps_tempchange;	//!< number of timesteps after which the temperature is changed
	double delta_temp;//!< step size in which the temperature is changed

	const int type;
	const int direction;

	static const double mass = 1.0;		//!< mass of the particles

	/**
	 * /brief Auxilliary function for calculating the kinetic energy of all particles.
	 *
	 * @return The kinetic energy of all particles in the ParticleContainer.
	 */
	double calculateKineticEnergy();

	/**
	 * /brief Applies the brownian motion to the particles.
	 */
	void applyBrown();

	/**
	 * /brief Changes the next temperature using 'delta_temp'.
	 * This method should be called after 'steps_tempchange' time steps have passed.
	 */
	void changeTemperature();

	/**
	 * /brief Adjusts the temperatures of all particles to match the target temperature.
	 * This method should be called after 'steps_thermostat' time steps have passed.
	 */
	void adjustParticleTemperatures();

public:
	static const double k_B = 1.3806488;	//!< Boltzmann constant

	/**
	 * /brief Initialize the Thermostat. A constant temperature will be used.
	 *
	 * @param param_particles The ParticleContainer which temperatures the thermostat should control.
	 * @param param_num_dimensions The number of dimensions.
	 * @param param_init_temp The initial temperature of the particles.
	 * @param param_steps_thermostat The number of time steps after which the temperature is applied.
	 * @param param_applyBrown Determines whether the brownian motion should be applied to the particles. Default: yes.
	 * @param param_type Determines to which type of particles the temperature control is applied. Default: all.
	 * @param param_direction Determines in which direction the velocity of particles should not contribute to temperature. Default: none.
	 */
	Thermostat(ParticleContainer& param_particles, int param_num_dimensions,
			double param_init_temp, int param_steps_thermostat,
			bool param_applyBrown = true, int param_type = -1, int param_direction = -1);

	/**
	 * /brief Initialize the Thermostat. The temperatures will change until a target is reached.
	 *
	 * @param param_particles The ParticleContainer which temperatures the thermostat should control.
	 * @param param_num_dimensions The number of dimensions.
	 * @param param_init_temp The initial temperature of the particles.
	 * @param param_steps_thermostat The number of time steps after which the temperature is applied.
	 * @param param_target_temp The temperature of the particles the thermostat is targeting.
	 * @param param_delta_temp The step size in which the temperature is changed.
	 * @param param_steps_tempchange The number of timesteps after which the temperature is changed.
	 * @param param_applyBrown Determines whether the brownian motion should be applied to the particles. Default: yes.
	 * @param param_type Determines to which type of particles the temperature control is applied. Default: all.
	 * @param param_direction Determines in which direction the velocity of particles should not contribute to temperature. Default: none.
	 */
	Thermostat(ParticleContainer& param_particles, int param_num_dimensions,
			double param_init_temp, int param_steps_thermostat,
			double param_target_temp, double param_delta_temp,
			int param_steps_tempchange, bool param_applyBrown = true,
			int param_type = -1, int param_direction = -1);

	~Thermostat();

	/**
	 * /brief Changes the next temperature and adjusts the particle temperatures when the right time has come.
	 *
	 * @param iteration The number of the current iteration of the simulation.
	 */
	void handle(int iteration);
};

#endif /* THERMOSTAT_H_ */
