#pragma once
// clang-format off
#include "pch_bll.h"
#include "simulation_data.h"
// clang-format on



namespace ss::bll::simulation
{
struct Food
{
    types::fVec2 pos = {0, 0};
    bool isEaten = false;
};

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

///
/// @brief An enumeration type describing the three states of an Entity.
///
enum class EntityFoodStage
{
    ZERO_FOOD, // Entity has not found food yet. Seeking for food.
    ONE_FOOD,  // Entity has found 1 piece of food. Going home. Could take more food on the way.
    TWO_FOOD   // Entity has found 2 pieces of food. Going home. No more detours for food.
};

class Entity
{
// public:
	int m_worldSize;

    types::fVec2 m_pos = {0.0f, 0.0f};
    float m_turnRate = 1.0f;
    float m_facingAngle = 0.0f;
    float m_turningAngle = 0.0f;
    float m_timeSinceLastTurn = 0.0f;

    float m_energyMax = 500;
    float m_currentEnergy = m_energyMax;

    types::Trait traits{ 1.0f, 1.0f };
    constexpr inline static float traitPadding = 0.3f;

    bool m_isAlive = true;
    bool m_isDoneWithCycle = false;
    bool m_shouldReproduce = false;

    EntityFoodStage m_foodStage = EntityFoodStage::ZERO_FOOD;

public:
    Entity(const int t_worldSize, DirectionsDeg startingAngle);
    // Entity& operator=(const Entity&) = default;
    
private:
    void update(const float elapsedTime);

    void walk(const float elapsedTime);

  public:
    [[nodiscard]] const types::fVec2 &getPos() const;
    [[nodiscard]] float getFacingAngle() const;

    friend class Cycle;
    friend class Simulation;
};

class Cycle
{
    Cycle();

    static void reproduceEntities(std::vector<Entity> &entities, std::vector<Entity>::iterator entitiesEndIt);
    static void distributeEntities(std::span<Entity> entities, size_t wallSize);

  public:
    friend class Simulation;
};

class Simulation
{
  private:
    const ss::types::SimulationInfo m_simInfo;

    std::vector<Entity> m_entities;
    std::vector<Entity>::iterator m_entitiesEndIt;

    size_t m_currentCycle = 0;

    // Make sure that all the active entities are included. (especially the last active one)
    [[nodiscard]] static std::span<Entity> getActiveEntities(std::vector<Entity>& entities, std::vector<Entity>::iterator& iter);
    static void repositionEntitiesIter(std::vector<Entity>& entities, std::vector<Entity>::iterator& iter);
    
  public:
    Simulation(const ss::types::SimulationInfo &t_simInfo);

    friend class Cycle;
};
} // namespace ss::bll::simulation