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
  public:
    // ^ remove me (Debug only)
    int m_worldSize;
    std::vector<Food> *m_foods = nullptr;

    types::fVec2 m_pos = {0.0f, 0.0f};
    /* const */ float m_turnRate = 1.0f;
    /* const */ float m_maxTurnAngle = 100.0f;
    float m_facingAngle = 0.0f;
    float m_turningAngle = 0.0f;
    float m_timeSinceLastTurn = 0.0f;

    Food *m_targetFood = nullptr;

    float m_energyMax = 500;
    float m_currentEnergy = m_energyMax;

    types::Trait m_traits;
    constexpr inline static float traitPadding = 0.8f;

    bool m_isAlive = true;
    bool m_isDoneWithCycle = false;
    bool m_shouldReproduce = false;

    EntityFoodStage m_foodStage = EntityFoodStage::ZERO_FOOD;

  public:
    Entity(const int t_worldSize, const types::Trait &t_traits, std::vector<Food> *t_foods);
    // Entity& operator=(const Entity&) = default;

  private:
    void update(const float elapsedTime);

    std::optional<float> getAngleToClosestFoodInRange();
    float getAngleToClosestWall();

    void generateNewTurningAngle();
    bool isOutOfBounds() const;
    bool handleFoodCollision();

    void reset();

    void walk(const float elapsedTime);
    void move(const float elapsedTime);

  public:
    [[nodiscard]] const types::fVec2 &getPos() const;
    [[nodiscard]] float getFacingAngle() const;

    friend class Cycle;
    friend class Simulation;
};

class Cycle
{
    std::vector<Entity> *m_entities;
    std::vector<Entity>::iterator *m_entitiesEndIter;
    std::span<Entity> activeEntities;

    std::vector<Food> *m_foods;

    size_t m_worldSize;

    bool m_isCycleDone = false;

  public:
    Cycle();
    Cycle(std::vector<Entity> *t_entities, std::vector<Entity>::iterator *t_entitiesEndIter, size_t t_worldSize,
          std::vector<Food> *t_foods);

    void CycleEnd();
    // ~Cycle();

    static void reproduceEntities(std::vector<Entity> &entities, std::vector<Entity>::iterator &entitiesEndIt,
                                  std::vector<Food> *foods);
    static void distributeEntities(std::span<Entity> entities, size_t wallSize);
    static void randomizeFoodPositions(std::span<Food> foods, size_t worldSize);

  public:
    void update(float elapsedTime);

    friend class Simulation;
};

class Simulation
{
  public:
    const ss::types::SimulationInfo m_simInfo;

    std::vector<Entity> m_entities;
    std::vector<Entity>::iterator m_entitiesEndIt;
    std::vector<Food> m_foods;

    size_t m_currentCycle_n = 1;
    Cycle m_currentCycle;

  public:
    bool isSimulationDone = false;

  public:
    // Make sure that all the active entities are included. (especially the last active one)
    [[nodiscard]] static std::span<Entity> getActiveEntities(std::vector<Entity> &entities,
                                                             std::vector<Entity>::iterator &iter);
    const std::vector<Food> &getFoods() const;
    static void repositionEntitiesIter(std::vector<Entity> &entities, std::vector<Entity>::iterator &iter);

  public:
    Simulation(const ss::types::SimulationInfo t_simInfo);
    void update(float elapsedTime);
    friend class Cycle;
};
} // namespace ss::bll::simulation