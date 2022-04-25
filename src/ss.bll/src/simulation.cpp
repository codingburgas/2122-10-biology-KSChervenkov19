// clang-format off
#include "pch_bll.h"
#include "simulation.h"
// clang-format on

#include "random.hpp"
#include "utils.h"

using Random = effolkronium::random_static;

ss::bll::simulation::Entity::Entity(const int t_worldSize, const types::Trait& t_traits = {1.0f, 1.0f})
	: m_worldSize(t_worldSize), m_traits(t_traits)
{
}

void ss::bll::simulation::Entity::update(const float elapsedTime)
{
    if (!m_isAlive)
        return;

    if (m_isDoneWithCycle)
        return;

    switch (m_foodStage)
    {
    case EntityFoodStage::ZERO_FOOD:
        // if foodCheck:
        //   facingAngle = getAngle;
        //   move();
        //   checkFoodCollision(); // if tru: foodStage = ONE_FOOD
        // else:
        //   walk();
        break;
    case EntityFoodStage::ONE_FOOD:
        // if foodCheck:
        //   facingAngle = getAngle;
        // else:
        //   set facing angle based on closest wall using DirectionsDeg
        //
        // move();
        break;
    case EntityFoodStage::TWO_FOOD:
        // set facing angle based on closest wall using DirectionsDeg
        // move();
        break;
    }

    // Debugging
    walk(elapsedTime);
}

void ss::bll::simulation::Entity::generateNewTurningAngle()
{
    m_turningAngle = Random::get(-m_maxTurnAngle, m_maxTurnAngle);
}

bool ss::bll::simulation::Entity::isOutOfBounds()
{
    return (m_pos.x < 0.0f || m_pos.x > m_worldSize) ||
        (m_pos.y < 0.0f || m_pos.y > m_worldSize);
}

// walk mf is for turn logic and moving (mf -> member function)
// move mf is for just moving forward based on pos and facingAngle
void ss::bll::simulation::Entity::walk(const float elapsedTime)
{
    // 0.
    // check if timeSinceLastTurn > 1.0f (i.e.)
    // if true: timeSinceLastTurn -= 1.0f
    // generate new turningAngle

    if (m_timeSinceLastTurn > 1.6f)
    {
        m_timeSinceLastTurn -= 1.6f;
        generateNewTurningAngle();
    }

    // 1.
    // turn angle for current frame = turnRate * elapsedTime
    // add TAFCF to facingAngle and subtract TAFCF from turningAngle
    // if turningAngle really smol prolly best to set it to 0
    // think about that last one a lil more

    if (abs(m_turningAngle) < 0.1f )
    {
        m_turningAngle = 0.0f;
    }

    if (m_turningAngle != 0.0f)
    {
        const bool isNegative = signbit(m_turningAngle);
        m_facingAngle += isNegative ? -m_turnRate : m_turnRate;
        m_turningAngle += isNegative ? m_turnRate : -m_turnRate;
    }

    // #THIS SHOULD BE SEPARATE FUNCTION probably named walk
    // 2.determine new position based on pos and angle
    /// check if new pos is out of bounds
    /// if true: isDoneWithCycle = true
    /// if true: isAlive = false
    // #ENDTHIS

	move(elapsedTime);

    // last step
    // timeSinceLastTurn += elapsedTime

    m_timeSinceLastTurn += elapsedTime;
}

void ss::bll::simulation::Entity::move(const float elapsedTime)
{
    float facingRadian = utils::toRadian(m_facingAngle + 180);

    float XX = m_pos.x + m_traits.speed * elapsedTime * cos(facingRadian);
    float YY = m_pos.y + m_traits.speed * elapsedTime * sin(facingRadian);

    m_pos = { XX, YY };

    if (isOutOfBounds())
        m_isDoneWithCycle = true;
}

const ss::types::fVec2 &ss::bll::simulation::Entity::getPos() const
{
    return m_pos;
}

float ss::bll::simulation::Entity::getFacingAngle() const
{
    return m_facingAngle;
}

ss::bll::simulation::Cycle::Cycle()
    : m_entities(nullptr), m_entitiesEndIter(nullptr), m_worldSize(0)
{
}

ss::bll::simulation::Cycle::Cycle(std::vector<Entity> *t_entities, std::vector<Entity>::iterator *t_entitiesEndIter,
                                  size_t t_worldSize)
    : m_entities(t_entities), m_entitiesEndIter(t_entitiesEndIter), m_worldSize(t_worldSize),
      activeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter))
{
}

void ss::bll::simulation::Cycle::CycleEnd()
{
    reproduceEntities(*m_entities, *m_entitiesEndIter);
    distributeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter), m_worldSize);
    Simulation::repositionEntitiesIter(*m_entities, *m_entitiesEndIter);
    // call reorder of foods
}

// ss::bll::simulation::Cycle::~Cycle()
//{
//    reproduceEntities(m_entities, m_entitiesEndIter);
//    distributeEntities(Simulation::getActiveEntities(m_entities, m_entitiesEndIter), m_worldSize);
//}

void ss::bll::simulation::Cycle::reproduceEntities(std::vector<Entity> &entities,
                                                   std::vector<Entity>::iterator &entitiesEndIt)
{
    for (auto entity : Simulation::getActiveEntities(entities, entitiesEndIt))
    {
        if (entity.m_shouldReproduce)
        {
            Entity newEntity(entity.m_worldSize);
            newEntity.m_traits.sense = entity.m_traits.sense + Random::get(-Entity::traitPadding, Entity::traitPadding);
            newEntity.m_traits.speed = entity.m_traits.speed + Random::get(-Entity::traitPadding, Entity::traitPadding);
            entities.insert(entitiesEndIt, newEntity);

            entity.m_shouldReproduce = false;
        }
    }

    Simulation::repositionEntitiesIter(entities, entitiesEndIt);
}

void ss::bll::simulation::Cycle::distributeEntities(std::span<Entity> entities, size_t wallSize)
{
    const size_t entitiesCountRegularWall = round(static_cast<float>(entities.size()) / 4.0f);
    const size_t entitiesCountLastWall = entities.size() - entitiesCountRegularWall * 3;

    // Failed attempt at optimizing TODO: debug me please
    size_t facingAngle = 0;
    size_t spacing = wallSize / (entitiesCountRegularWall + 1);
    size_t coordinate = spacing;
  //  size_t count = 1;
  //  for (size_t i = 0; i < entitiesCountRegularWall * 3; ++i)
  //  {
  //      if (i > count * entitiesCountLastWall)
  //      // if (i > (facingAngle ? facingAngle : 1) / 90 * entitiesCountRegularWall)
  //      {
  //          facingAngle += 90;
  //          coordinate = spacing;
  //          ++count;
  //      }

  //      entities[i].m_facingAngle = facingAngle;

		//switch (static_cast<DirectionsDeg>(facingAngle))
  //      {
  //      case DirectionsDeg::LEFT:
  //          // { wallSize, coordinate }
  //          entities[i].m_pos = { static_cast<float>(wallSize), static_cast<float>(coordinate) };
  //          break;
  //      case DirectionsDeg::UP:
  //          // { coordinate, wallSize }
  //          entities[i].m_pos = { static_cast<float>(coordinate),static_cast<float>(wallSize) };
  //          break;
  //      case DirectionsDeg::RIGHT:
  //          // { 0, coordinate }
  //          entities[i].m_pos = { 0.0f, static_cast<float>(coordinate) };
  //          break;
  //      default:
  //          break;
  //      }
  //  }

    for (size_t i = 0; i < entitiesCountRegularWall; ++i)
    {
        entities[i].m_facingAngle = 0.0f;
        entities[i].m_pos = { static_cast<float>(wallSize), static_cast<float>(coordinate) };

        coordinate += spacing;
    }

    coordinate = spacing;
    for (size_t i = entitiesCountRegularWall; i < entitiesCountRegularWall * 2; ++i)
    {
        entities[i].m_facingAngle = 90.0f;
        entities[i].m_pos = { static_cast<float>(coordinate),static_cast<float>(wallSize) };

        coordinate += spacing;
    }

    coordinate = spacing;
    for (size_t i = entitiesCountRegularWall * 2; i < entitiesCountRegularWall * 3; ++i)
    {
        entities[i].m_facingAngle = 180.0f;
        entities[i].m_pos = { 0.0f, static_cast<float>(coordinate) };

        coordinate += spacing;
    }

    spacing = wallSize / (entitiesCountLastWall + 1);
    coordinate = spacing;
    for (size_t i = entitiesCountRegularWall * 3; i < entities.size(); ++i)
    {
        entities[i].m_facingAngle = 270.0f;
        entities[i].m_pos = { static_cast<float>(coordinate), 0.0f };

        coordinate += spacing;
    }

    // !!!BROKEN Uniform method -------------------------------------
	//   const size_t padding = 1;
	//   // wallSize -= padding;
	//   const size_t entitiesCountRegularWall = round(static_cast<float>(entities.size()) / 4.0f);
	//   const size_t entitiesCountLastWall = entities.size() - entitiesCountRegularWall * 3;

	 //   size_t facingAngle = 0;
	 //   size_t coordinate = padding;
	 //   size_t spacing = (wallSize - padding) / entitiesCountRegularWall;

	 //   for (size_t i = 0; i < entitiesCountRegularWall * 3; ++i)
	 //   {
	 //       entities[i].m_facingAngle = static_cast<float>(facingAngle);

		//    if (i > facingAngle / 90 * entitiesCountRegularWall)
		//    {
	 //           facingAngle += 90;
	 //           coordinate = 0;
		//    }

	 //       switch (static_cast<DirectionsDeg>(facingAngle))
	 //       {
	 //       case DirectionsDeg::LEFT:
	 //           // { wallSize, coordinate }
	 //           entities[i].m_pos = { static_cast<float>(wallSize), static_cast<float>(coordinate) };
	 //           break;
	 //       case DirectionsDeg::UP:
	 //           // { coordinate, wallSize }
	 //           entities[i].m_pos = { static_cast<float>(coordinate), static_cast<float>(wallSize) };
	 //           break;
	 //       case DirectionsDeg::RIGHT:
	 //           // { 0, coordinate }
	 //           entities[i].m_pos = { 0.0f, static_cast<float>(coordinate) };
	 //           break;
	 //       default:
	 //           break;
	 //       }

	 //       coordinate += spacing;
	 //   }

	 //   coordinate = padding;
	 //   spacing = (wallSize - padding) / entitiesCountLastWall;

	 //   for (size_t i = entitiesCountRegularWall * 3; i < entities.size(); ++i)
	 //   {
	 //       entities[i].m_facingAngle = 270.0f;
	 //       entities[i].m_pos = { static_cast<float>(coordinate), 0.0f };

	 //       coordinate += spacing;
	 //   }

    // Random method -------------------------------------

    //size_t spacing = wallSize / (entities.size() / 4);
    //size_t coordinate = 0;
    //auto currentDirection = DirectionsDeg::LEFT;

    //for (auto& entity : entities)
    //{
    //    entity.m_facingAngle = static_cast<float>(currentDirection);

    //    switch (currentDirection)
    //    {
    //    case DirectionsDeg::LEFT:
    //        // { wallSize, coordinate }
    //        entity.m_pos = {static_cast<float>(wallSize), static_cast<float>(Random::get(0, (int)wallSize))};
    //        break;
    //    case DirectionsDeg::UP:
    //        // { coordinate, wallSize }
    //        entity.m_pos = {static_cast<float>(Random::get(0, (int)wallSize)), static_cast<float>(wallSize)};
    //        break;
    //    case DirectionsDeg::RIGHT:
    //        // { 0, coordinate }
    //        entity.m_pos = {0.0f, static_cast<float>(Random::get(0, (int)wallSize))};
    //        break;
    //    case DirectionsDeg::DOWN:
    //        // { coordinate, 0 }
    //        entity.m_pos = {static_cast<float>(Random::get(0, (int)wallSize)), 0.0f};
    //        break;
    //    }

    //    if (currentDirection == DirectionsDeg::DOWN)
    //    {
    //        currentDirection = DirectionsDeg::LEFT;
    //        coordinate += spacing;
    //    }
    //    else
    //    {
    //        currentDirection = static_cast<DirectionsDeg>(static_cast<size_t>(currentDirection) + 90);
    //    }
    //}
}

void ss::bll::simulation::Cycle::randomizeFoodPositions(std::span<Food> foods, size_t worldSize)
{
    for (auto& food : foods)
    {
        food = { { Random::get<Random::common>(0.0f, static_cast<int>(worldSize)),
                      Random::get<Random::common>(0.0f, static_cast<int>(worldSize)) } };
    }
}

void ss::bll::simulation::Cycle::update(float elapsedTime)
{
    if (m_isCycleDone)
        return;

    bool areAllEntitiesDone = true;

	auto activeEntities = Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter);

    for (auto& entity : activeEntities)
    {
        if (!entity.m_isDoneWithCycle)
            areAllEntitiesDone = false;

        entity.update(elapsedTime);
    }

    if (areAllEntitiesDone)
        m_isCycleDone = true;

    // iterate through entities and update every single one
    // somehow determine when all the entities are done
}

std::span<ss::bll::simulation::Entity> ss::bll::simulation::Simulation::getActiveEntities(
    std::vector<Entity> &entities, std::vector<Entity>::iterator &iter)
{
    return std::span(entities.data(), std::distance(entities.begin(), iter));
}

const std::vector<ss::bll::simulation::Food>& ss::bll::simulation::Simulation::getFoods() const
{
    return m_foods;
}

void ss::bll::simulation::Simulation::repositionEntitiesIter(std::vector<Entity> &entities,
                                                             std::vector<Entity>::iterator &iter)
{
    auto current = entities.begin();
    while (current != entities.end() && current->m_isAlive)
    {
        ++current;
    }
    iter = current;
}

/// Constructor for the Simulation class.
///
/// Constructs the Simulation class with approriate ss::types::SimulationInfo.
/// @param t_simInfo object of user defined type ss::types::SimulationInfo holding the data.
ss::bll::simulation::Simulation::Simulation(const ss::types::SimulationInfo t_simInfo)
    : m_simInfo(t_simInfo), m_foods(std::vector<Food>(m_simInfo.foodCount))
{
    // m_entities = std::vector<Entity>(m_simInfo.startingEntityCount);
    // m_entitiesEndIt = m_entities.end();

    m_entities.reserve(m_simInfo.startingEntityCount);

    for (size_t i = 0; i < m_simInfo.startingEntityCount; ++i)
    {
        m_entities.emplace_back(t_simInfo.worldSize, m_simInfo.initialTraits);
    }
    m_entitiesEndIt = m_entities.end();

    Cycle::distributeEntities(getActiveEntities(m_entities, m_entitiesEndIt), m_simInfo.worldSize);

    m_currentCycle = Cycle(&m_entities, &m_entitiesEndIt, m_simInfo.worldSize);
    Cycle::randomizeFoodPositions(m_foods, m_simInfo.worldSize);
}

void ss::bll::simulation::Simulation::update(float elapsedTime)
{
    // calls update of current cycle

    if (isSimulationDone)
    {
        return;
    }

    if (m_currentCycle_n > m_simInfo.cyclesCount)
    {
        isSimulationDone = true;
        return;
    }

    if (m_currentCycle.m_isCycleDone)
    {
        m_currentCycle.CycleEnd();
        m_currentCycle = Cycle(&m_entities, &m_entitiesEndIt, m_simInfo.worldSize);
        ++m_currentCycle_n;
    }

    m_currentCycle.update(elapsedTime);
}
