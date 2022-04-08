#pragma once

#include "pch_bll.h"
#include "simulation_data.h"

namespace ss::bll
{
	class Simulation
	{
	private:
		const ss::types::SimulationInfo m_simInfo;

	public:
		Simulation(const ss::types::SimulationInfo& t_simInfo);

		// TODO: Create Entity(Bob) class
		// TODO: update method
		// TODO: 
	};
}