#pragma once
// clang-format off
#include "pch_bll.h"
#include "simulation_data.h"
// clang-format on

namespace ss::bll::simulation
{
enum class DirectionsDeg
{
	LEFT = 0,
	UP = 90,
	RIGHT = 180,
	DOWN = 270,
};

class Entity
{
private:
	const ss::types::SimulationInfo& m_simInfo;
	float x, y;
	float m_facingAngle;
	float energy = 500;

public:
	Entity(const ss::types::SimulationInfo& t_simInfo,ss::bll::simulation::DirectionsDeg startingAngle)
		:m_simInfo(t_simInfo), m_facingAngle(static_cast<float>(startingAngle))
	{
		
	}
};

class Cycle
{
private:
public:
	Cycle()
	{
		
	}
};

class Simulation
{
  private:
    const ss::types::SimulationInfo m_simInfo;
	std::vector<ss::bll::simulation::Entity> m_entities;
	size_t m_currentCycle = 0;


  public:
    Simulation(const ss::types::SimulationInfo &t_simInfo);
};
} // namespace ss::bll::simulation