// clang-format off
#include "pch_bll.h"
#include "simulation.h"
// clang-format on

#include "random.hpp"

using Random = effolkronium::random_static;


ss::bll::simulation::Entity::Entity(const int t_worldSize)
    : m_worldSize(t_worldSize)
{
}

void ss::bll::simulation::Entity::update(const float elapsedTime)
{
    if (!m_isAlive)
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
}

// walk mf is for turn logic and moving (mf -> member function)
// move mf is for just moving forward based on pos and facingAngle
void ss::bll::simulation::Entity::walk(const float elapsedTime)
{
    // 0.
    // check if timeSinceLastTurn > 1.0f (i.e.)
    // if true: timeSinceLastTurn -= 1.0f
    // generate new turningAngle

    // 1.
    // turn angle for current frame =  turningAngle * turnRate * elapsedTime
    // add TAFCF to facingAngle and subtract TAFCF from turningAngle
    // if turningAngle really smol prolly best to set it to 0
    // thing about that last one a lil more

    // #THIS SHOULD BE SEPARATE FUNCTION probably named walk
    // 2.determine new position based on pos and angle
    /// check if new pos is out of bounds
    /// if true: isDoneWithCycle = false
    // #ENDTHIS

    // last step
    // timeSinceLastTurn += elapsedTime
}

const ss::types::fVec2 &ss::bll::simulation::Entity::getPos() const
{
    return m_pos;
}

float ss::bll::simulation::Entity::getFacingAngle() const
{
    return m_facingAngle;
}

ss::bll::simulation::Cycle::Cycle(std::vector<Entity>* t_entities, 
								  std::vector<Entity>::iterator* t_entitiesEndIter,
								  size_t t_worldSize)
	: m_entities(t_entities), m_entitiesEndIter(t_entitiesEndIter), m_worldSize(t_worldSize), activeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter))
{

}

void ss::bll::simulation::Cycle::CycleEnd()
{
    reproduceEntities(*m_entities, *m_entitiesEndIter);
    distributeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter), m_worldSize);
    // call reorder of foods
} 

//ss::bll::simulation::Cycle::~Cycle()
//{
//    reproduceEntities(m_entities, m_entitiesEndIter);
//    distributeEntities(Simulation::getActiveEntities(m_entities, m_entitiesEndIter), m_worldSize);
//}

void ss::bll::simulation::Cycle::reproduceEntities(std::vector<Entity>& entities,
                                                   std::vector<Entity>::iterator& entitiesEndIt)
{
    for (auto entity : Simulation::getActiveEntities(entities, entitiesEndIt))
    {
	    if (entity.m_shouldReproduce)
	    {
            Entity newEntity(entity.m_worldSize);
            newEntity.traits.sense = entity.traits.sense + Random::get(-Entity::traitPadding, Entity::traitPadding);
            newEntity.traits.speed = entity.traits.speed + Random::get(-Entity::traitPadding, Entity::traitPadding);
            entities.insert(entitiesEndIt, newEntity);

            entity.m_shouldReproduce = false;
	    }
    }

    Simulation::repositionEntitiesIter(entities, entitiesEndIt);
}

void ss::bll::simulation::Cycle::distributeEntities(std::span<Entity> entities, size_t wallSize)
{
    const size_t spacing = wallSize / (entities.size() / 4);
    size_t coordinate = 0;
    auto currentDirection = DirectionsDeg::LEFT;

    for (auto entity : entities)
    {
        entity.m_facingAngle = static_cast<float>(currentDirection);

	    switch (currentDirection)
	    {
	    case DirectionsDeg::LEFT:
            // { wallSize, coordinate }
            entity.m_pos = { static_cast<float>(wallSize),
                               static_cast<float>(coordinate) };
            break;
	    case DirectionsDeg::UP:
            // { coordinate, wallSize }
            entity.m_pos = { static_cast<float>(coordinate),
                               static_cast<float>(wallSize) };
            break;
	    case DirectionsDeg::RIGHT:
            // { 0, coordinate }
            entity.m_pos = { 0.0f,
                               static_cast<float>(coordinate) };
            break;
	    case DirectionsDeg::DOWN:
            // { coordinate, 0 }
            entity.m_pos = { static_cast<float>(coordinate),
                               0.0f };
            break;
	    }

        if (currentDirection == DirectionsDeg::DOWN)
        {
            currentDirection = DirectionsDeg::LEFT;
			coordinate += spacing;
        }
        else
        {
            currentDirection = static_cast<DirectionsDeg>(static_cast<size_t>(currentDirection) + 90);
        }
    }
}

void ss::bll::simulation::Cycle::update(float elapsedTime)
{
	// iterate through entities and update every single one
	// somehow determine when all the entities are done
}

std::span<ss::bll::simulation::Entity> ss::bll::simulation::Simulation::getActiveEntities(std::vector<Entity>& entities, std::vector<Entity>::iterator& iter)
{
    return std::span(entities.data(), std::distance(entities.begin(), iter));
}

void ss::bll::simulation::Simulation::repositionEntitiesIter(std::vector<Entity>& entities, std::vector<Entity>::iterator& iter)
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
ss::bll::simulation::Simulation::Simulation(const ss::types::SimulationInfo& t_simInfo) : m_simInfo(t_simInfo), m_currentCycle(Cycle(&m_entities, &m_entitiesEndIt, m_simInfo.worldSize))
{
    // m_entities = std::vector<Entity>(m_simInfo.startingEntityCount);
    // m_entitiesEndIt = m_entities.end();

	m_entities.reserve(m_simInfo.startingEntityCount);

    for (size_t i = 0; i < m_simInfo.startingEntityCount; ++i)
    {
        m_entities.emplace_back(t_simInfo.worldSize);
    }
    m_entitiesEndIt = m_entities.end();

    Cycle::distributeEntities(getActiveEntities(m_entities, m_entitiesEndIt), m_simInfo.worldSize);
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
    }

    m_currentCycle.update(elapsedTime);
}
