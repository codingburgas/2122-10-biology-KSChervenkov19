#pragma once
// clang-format off
#include "pch_bll.h"
#include "simulation_data.h"
// clang-format on

namespace ss::bll::simulation
{
///
/// @brief An enumeration type describing the directions of the board.
///
enum class DirectionsDeg
{
    LEFT = 0,
    UP = 90,
    RIGHT = 180,
    DOWN = 270,
};

enum class EntityFoodStage
{
    ZERO_FOOD, // Entity has not found food yet. Seeking for food.
    ONE_FOOD,  // Entity has found 1 piece of food. Going home. Could take more food on the way.
    TWO_FOOD   // Entity has found 2 pieces of food. Going home. No more detours for food.
};

class Entity
{
  private:
    const int &m_worldSize;

    types::fVec2 pos;
    const float m_turnRate = 1.0f;
    float m_facingAngle;
    float m_turningAngle = 0.0f;
    float timeSinceLastTurn = 0.0f;

    const float m_energyMax = 500;
    float m_currentEnergy = m_energyMax;

    bool m_isAlive = true;
    bool m_isDoneWithCycle = false;

    EntityFoodStage m_foodStage = EntityFoodStage::ZERO_FOOD;

  public:
    Entity(const int &t_worldSize, DirectionsDeg startingAngle);

    void update(const float elapsedTime);

  private:
    void walk(const float elapsedTime);
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