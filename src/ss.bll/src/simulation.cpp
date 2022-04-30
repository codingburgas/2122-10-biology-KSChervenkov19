// clang-format off
#include "pch_bll.h"
#include "simulation.h"
// clang-format on

#include <iostream>

#include "random.hpp"
#include "utils.h"

using Random = effolkronium::random_static;

ss::bll::simulation::Entity::Entity(size_t t_id, const int t_worldSize, const types::Trait& t_traits = { 1.0f, 1.0f },
	std::vector<Food>* t_foods = nullptr, size_t t_cycleBornAt = 0)
	: m_id(t_id), m_worldSize(t_worldSize), m_foods(t_foods), m_traits(t_traits), m_cycleBornAt(t_cycleBornAt)
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
	case EntityFoodStage::ZERO_FOOD: {

		if (const auto angleToClosestFoodInRange = getAngleToClosestFoodInRange(); angleToClosestFoodInRange)
		{
			m_facingAngle = *angleToClosestFoodInRange;
			move(elapsedTime);
			if (handleFoodCollision(elapsedTime))
			{
				m_foodStage = EntityFoodStage::ONE_FOOD;
			}
		}
		else
		{
			walk(elapsedTime);
		}
		break;
	}
	case EntityFoodStage::ONE_FOOD: {
		if (const auto angleToClosestFoodInRange = getAngleToClosestFoodInRange(); angleToClosestFoodInRange)
		{
			m_facingAngle = *angleToClosestFoodInRange;
			move(elapsedTime);
			if (handleFoodCollision(elapsedTime))
			{
				m_foodStage = EntityFoodStage::TWO_FOOD;
			}
		}
		else
		{
			const float angle = getAngleToClosestWall();
			m_facingAngle = angle;
			move(elapsedTime);
		}
		break;
	}
	case EntityFoodStage::TWO_FOOD: {
		const float angle = getAngleToClosestWall();
		m_facingAngle = angle;
		move(elapsedTime);
		break;
	}
	}

	hanldeEnergy(elapsedTime);

	// Debugging
	// walk(elapsedTime);
}

std::optional<float> ss::bll::simulation::Entity::getAngleToClosestFoodInRange()
{
	struct FoodDistance
	{
		Food& food;
		float distance;
	};

	std::vector<FoodDistance> foodDistance;

	for (auto& food : *m_foods)
	{
		if (!food.isEaten)
		{
			if (const float distance = utils::getDistance(m_pos, food.pos); distance <= m_traits.sense)
			{
				foodDistance.push_back({ food, distance });
			}
		}
	}

	if (foodDistance.empty())
	{
		return {};
	}

	const auto closestFood = *std::ranges::min_element(
		foodDistance, [](const FoodDistance& lhs, const FoodDistance& rhs) { return lhs.distance < rhs.distance; });

	m_targetFood = &closestFood.food;

	return utils::getAngle(closestFood.food.pos, m_pos);
}

float ss::bll::simulation::Entity::getAngleToClosestWall()
{
	using namespace utils;

	types::fVec2 pos;
	float top = getDistance(m_pos, { m_pos.x, 0.0f });
	float right = getDistance(m_pos, { static_cast<float>(m_worldSize), m_pos.y });
	float bottom = getDistance(m_pos, { m_pos.x, static_cast<float>(m_worldSize) });
	float left = getDistance(m_pos, { 0.0f, m_pos.y });

	const std::vector distances{ top, right, bottom, left };

	switch (std::distance(distances.begin(), std::min_element(distances.begin(), distances.end())))
	{
	case 0:
		return 90.0f;
	case 1:
		return 180.0f;
	case 2:
		return 270.0f;
	case 3:
		return 0.0f;
	}

	return {};
}

void ss::bll::simulation::Entity::generateNewTurningAngle()
{
	m_turningAngle = Random::get(-m_maxTurnAngle, m_maxTurnAngle);
}

bool ss::bll::simulation::Entity::isOutOfBounds() const
{
	return (m_pos.x < 0.0f || m_pos.x > m_worldSize) || (m_pos.y < 0.0f || m_pos.y > m_worldSize);
}

bool ss::bll::simulation::Entity::handleFoodCollision(const float elaspedTime)
{
	if (m_targetFood)
	{
		if (float distance = utils::getDistance(m_pos, m_targetFood->pos); distance <= m_traits.speed * elaspedTime)
		{
			m_targetFood->isEaten = true;
			m_targetFood = nullptr;
			// m_foodStage = EntityFoodStage::ONE_FOOD;
			return true;
		}
	}

	return false;
}

void ss::bll::simulation::Entity::reset()
{
	m_turningAngle = 0;
	m_timeSinceLastTurn = 0;

	m_currentEnergy = m_energyMax;

	m_isDoneWithCycle = false;
	m_foodStage = EntityFoodStage::ZERO_FOOD;
}

void ss::bll::simulation::Entity::hanldeEnergy(const float elapsedTime)
{
	m_currentEnergy -= (pow(m_traits.speed, 2) + pow(m_traits.sense, 3)) * elapsedTime;

	if (m_currentEnergy < 0.0f)
	{
		m_isDoneWithCycle = true;
		m_foodStage = EntityFoodStage::ZERO_FOOD;
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

	if (abs(m_turningAngle) < 0.1f)
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

ss::types::EntityTarget ss::bll::simulation::Entity::getBrain() const
{
	if (m_isDoneWithCycle)
	{
		return types::EntityTarget::IDLE;
	}

	if (m_targetFood)
	{
		return types::EntityTarget::GOINGFOOD;
	}

	if (m_foodStage == EntityFoodStage::ONE_FOOD || m_foodStage == EntityFoodStage::TWO_FOOD)
	{
		return types::EntityTarget::GOINGHOME;
	}

	return types::EntityTarget::SEARCHINGFOOD;
}

const ss::types::fVec2& ss::bll::simulation::Entity::getPos() const
{
	return m_pos;
}

float ss::bll::simulation::Entity::getFacingAngle() const
{
	return m_facingAngle;
}

ss::bll::simulation::Cycle::Cycle() : m_entities(nullptr), m_entitiesEndIter(nullptr), m_worldSize(0)
{
}

ss::bll::simulation::Cycle::Cycle(std::vector<Entity>* t_entities, std::vector<Entity>::iterator* t_entitiesEndIter,
	size_t t_worldSize, std::vector<Food>* t_foods, size_t t_cycleId,
	size_t* t_lastEntityId = nullptr)
	: m_entities(t_entities), m_entitiesEndIter(t_entitiesEndIter), m_worldSize(t_worldSize),
	activeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter)), m_foods(t_foods),
	m_cycleId(t_cycleId), m_lastEntityId(t_lastEntityId)
{
}

void ss::bll::simulation::Cycle::CycleEnd()
{
	for (auto& entity : *m_entities)
	{
		switch (entity.m_foodStage)
		{
		case EntityFoodStage::ZERO_FOOD:
			entity.m_isAlive = false;
			entity.m_cyclesLived = m_cycleId - entity.m_cycleBornAt;
			break;
		case EntityFoodStage::TWO_FOOD:
			entity.m_shouldReproduce = true;
			break;
		default:;
		}
	}

	reproduceEntities(*m_entities, *m_entitiesEndIter, m_lastEntityId, m_cycleId, m_foods);
	distributeEntities(Simulation::getActiveEntities(*m_entities, *m_entitiesEndIter), m_worldSize);

	for (auto& entity : *m_entities)
	{
		entity.reset();
	}
}

void ss::bll::simulation::Cycle::reproduceEntities(std::vector<Entity>& entities,
	std::vector<Entity>::iterator& entitiesEndIt, size_t* lastEntityId,
	size_t cycleId, std::vector<Food>* foods)
{
	for (size_t i = 0; i < entities.size(); ++i)
		// for (auto& entity : entities)
	{
		if (entities[i].m_isAlive)
		{
			if (entities[i].m_shouldReproduce)
			{

				float sense = entities[i].m_traits.sense + Random::get(-Entity::traitPadding, Entity::traitPadding);
				float speed = entities[i].m_traits.speed + Random::get(-Entity::traitPadding, Entity::traitPadding);

				entities.push_back({ *lastEntityId, entities[i].m_worldSize, {sense, speed}, foods, cycleId });
				++(*lastEntityId);
				// broken v
				// entities.insert(entitiesEndIt, newEntity);

				entities[i].m_shouldReproduce = false;
			}
		}
	}

	// std::remove_if(entities.begin(), entities.end(), [](const Entity& et) { return !et.m_isAlive; });

	std::ranges::sort(entities, [](const Entity& lhs, const Entity& rhs) { return lhs.m_isAlive > rhs.m_isAlive; });

	Simulation::repositionEntitiesIter(entities, entitiesEndIt);
}

void ss::bll::simulation::Cycle::distributeEntities(std::span<Entity> entities, size_t wallSize)
{
	if (wallSize / (round(static_cast<float>(entities.size()) / 4.0f) + 1) > 1)
	{
		const size_t entitiesCountRegularWall = round(static_cast<float>(entities.size()) / 4.0f);
		const size_t entitiesCountLastWall = entities.size() - entitiesCountRegularWall * 3;
		size_t spacing = wallSize / (entitiesCountRegularWall + 1);
		size_t coordinate = spacing;
		std::cout << entitiesCountRegularWall;

		for (size_t i = 0; i < entitiesCountRegularWall; ++i)
		{
			entities[i].m_facingAngle = 0.0f;
			entities[i].m_pos = { static_cast<float>(wallSize), static_cast<float>(coordinate) };

			coordinate += spacing;
		}

		if (entities.size() >= 2)
		{
			coordinate = spacing;
			for (size_t i = entitiesCountRegularWall; i < entitiesCountRegularWall * 2; ++i)
			{
				entities[i].m_facingAngle = 90.0f;
				entities[i].m_pos = { static_cast<float>(coordinate), static_cast<float>(wallSize) };

				coordinate += spacing;
			}
		}

		if (entities.size() >= 3)
		{
			coordinate = spacing;
			for (size_t i = entitiesCountRegularWall * 2; i < entitiesCountRegularWall * 3; ++i)
			{
				entities[i].m_facingAngle = 180.0f;
				entities[i].m_pos = { 0.0f, static_cast<float>(coordinate) };

				coordinate += spacing;
			}
		}

		if (entities.size() >= 4)
		{
			spacing = wallSize / (entitiesCountLastWall + 1);
			coordinate = spacing;
			for (size_t i = entitiesCountRegularWall * 3; i < entities.size(); ++i)
			{
				entities[i].m_facingAngle = 270.0f;
				entities[i].m_pos = { static_cast<float>(coordinate), 0.0f };

				coordinate += spacing;
			}
		}
	}
	else
	{
	    size_t spacing = wallSize / (entities.size() / 4);
	    size_t coordinate = 0;
	    auto currentDirection = DirectionsDeg::LEFT;

	    for (auto& entity : entities)
	    {
	        entity.m_facingAngle = static_cast<float>(currentDirection);

	        switch (currentDirection)
	        {
	        case DirectionsDeg::LEFT:
	            // { wallSize, coordinate }
	            entity.m_pos = { static_cast<float>(wallSize), static_cast<float>(Random::get(0, (int)wallSize)) };
	            break;
	        case DirectionsDeg::UP:
	            // { coordinate, wallSize }
	            entity.m_pos = { static_cast<float>(Random::get(0, (int)wallSize)), static_cast<float>(wallSize) };
	            break;
	        case DirectionsDeg::RIGHT:
	            // { 0, coordinate }
	            entity.m_pos = { 0.0f, static_cast<float>(Random::get(0, (int)wallSize)) };
	            break;
	        case DirectionsDeg::DOWN:
	            // { coordinate, 0 }
	            entity.m_pos = { static_cast<float>(Random::get(0, (int)wallSize)), 0.0f };
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
}

void ss::bll::simulation::Cycle::randomizeFoodPositions(std::span<Food> foods, size_t worldSize)
{
	for (auto& food : foods)
	{
		food = { {Random::get<Random::common>(0.0f, static_cast<int>(worldSize)),
				 Random::get<Random::common>(0.0f, static_cast<int>(worldSize))} };
	}
}

void ss::bll::simulation::Cycle::update(const float elapsedTime)
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
}

void ss::bll::simulation::Simulation::cleanEntities()
{
	std::vector<int> indexesForDeletion;

	for (size_t i = 0; i < m_entities.size(); ++i)
	{
		if (m_entities.at(i).m_cyclesLived == 0)
		{
			m_entities[i].m_cyclesLived = m_simInfo.cyclesCount + 1 - m_entities[i].m_cycleBornAt;
		}

		if (m_entities.at(i).m_cycleBornAt > m_simInfo.cyclesCount)
		{
			indexesForDeletion.push_back(i);
		}
	}

	for (auto index : indexesForDeletion)
	{
		m_entities.erase(m_entities.begin() + index);
	}
}

std::span<ss::bll::simulation::Entity> ss::bll::simulation::Simulation::getActiveEntities(
	std::vector<Entity>& entities, std::vector<Entity>::iterator& iter)
{
	return std::span(entities.data(), std::distance(entities.begin(), iter));
}

const std::vector<ss::bll::simulation::Food>& ss::bll::simulation::Simulation::getFoods() const
{
	return m_foods;
}

void ss::bll::simulation::Simulation::repositionEntitiesIter(std::vector<Entity>& entities,
	std::vector<Entity>::iterator& iter)
{
	auto current = entities.begin();
	while (current != entities.end() && current->m_isAlive)
	{
		++current;
	}
	iter = current;
}

const ss::bll::simulation::Entity* ss::bll::simulation::Simulation::getEntityById(size_t id)
{
	auto activeEntities = getActiveEntities(m_entities, m_entitiesEndIt);
	if (auto entity = std::ranges::find_if(activeEntities, [id](const Entity& et) { return et.m_id == id; });
		entity != activeEntities.end())
	{
		return &*entity;
	}

	return nullptr;
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
		m_entities.emplace_back(m_lastEntityId++, t_simInfo.worldSize, m_simInfo.initialTraits, &m_foods,
			m_currentCycle_n);
	}
	m_entitiesEndIt = m_entities.end();

	Cycle::distributeEntities(getActiveEntities(m_entities, m_entitiesEndIt), m_simInfo.worldSize);

	++m_currentCycle_n;
	m_currentCycle =
		Cycle(&m_entities, &m_entitiesEndIt, m_simInfo.worldSize, &m_foods, m_currentCycle_n, &m_lastEntityId);
	Cycle::randomizeFoodPositions(m_foods, m_simInfo.worldSize);
}

size_t ss::bll::simulation::Simulation::getTotalAliveEntities() const
{
	return m_entities.size();
}

size_t ss::bll::simulation::Simulation::getTotalDiedEntities() const
{
	size_t diedCnt = 0;

	for (const auto& entity : m_entities)
	{
		if (!entity.m_isAlive)
		{
			++diedCnt;
		}
	}

	return diedCnt;
}

ss::types::Trait ss::bll::simulation::Simulation::getAvgTraits() const
{
	types::Trait avgTraits = { 0.0f, 0.0f };

	for (const auto& entity : m_entities)
	{
		avgTraits.sense += entity.m_traits.sense;
		avgTraits.speed += entity.m_traits.speed;
	}

	const float sizeF = m_entities.size();

	avgTraits.sense = avgTraits.sense / sizeF;
	avgTraits.speed = avgTraits.speed / sizeF;

	return avgTraits;
}

void ss::bll::simulation::Simulation::saveSimulationInfo(std::optional<std::string> fileName) const
{
	std::vector<types::Cycle> cycles;
	cycles.reserve(m_currentCycle_n - 2);

	for (size_t i = 0; i < m_currentCycle_n - 2; ++i)
	{
		size_t cycleId = i + 1;

		types::Cycle cycle;
		cycle.lastedEntities = 0;

		for (const auto& entity : m_entities)
		{
			// if (entity.m_cycleBornAt + entity.m_cyclesLived > cycleId)
			if (entity.m_cycleBornAt <= cycleId && entity.m_cycleBornAt + entity.m_cyclesLived > cycleId)
			{
				++cycle.lastedEntities;
				cycle.traitsInfo.push_back(entity.m_traits);
			}
		}

		cycles.push_back(cycle);
	}

	if (fileName)
	{
		dal::StatisticsStore::saveStatisticTo(*fileName, cycles);
	}
	else
	{
		dal::StatisticsStore::saveStatisticTo(cycles);
	}
}

void ss::bll::simulation::Simulation::update(const float elapsedTime)
{
	if (isSimulationDone)
	{
		return;
	}

	if (m_currentCycle_n > m_simInfo.cyclesCount + 1)
	{
		isSimulationDone = true;
		cleanEntities();
		return;
	}

	if (m_currentCycle.m_isCycleDone)
	{
		++m_currentCycle_n;
		m_currentCycle.CycleEnd();
		m_currentCycle =
			Cycle(&m_entities, &m_entitiesEndIt, m_simInfo.worldSize, &m_foods, m_currentCycle_n, &m_lastEntityId);
		Cycle::randomizeFoodPositions(m_foods, m_simInfo.worldSize);
	}

	m_currentCycle.update(elapsedTime);
}
