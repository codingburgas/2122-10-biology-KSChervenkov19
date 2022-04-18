// clang-format off
#include "pch_bll.h"
#include "simulation.h"
// clang-format on

/// Constructor for the Simulation class.
///
/// Constructs the Simulation class with approriate ss::types::SimulationInfo.
/// @param t_simInfo object of user defined type ss::types::SimulationInfo holding the data.
ss::bll::simulation::Simulation::Simulation(const ss::types::SimulationInfo &t_simInfo) : m_simInfo(t_simInfo)
{
}
