// clang-format off
#include "pch_bll.h"
#include "simulation.h"
// clang-format on

ss::bll::simulation::Entity::Entity(const int& t_worldSize, 
	DirectionsDeg startingAngle)
	: m_worldSize(t_worldSize), m_facingAngle(static_cast<float>(startingAngle))
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

/// Constructor for the Simulation class.
///
/// Constructs the Simulation class with approriate ss::types::SimulationInfo.
/// @param t_simInfo object of user defined type ss::types::SimulationInfo holding the data.
ss::bll::simulation::Simulation::Simulation(const ss::types::SimulationInfo &t_simInfo) : m_simInfo(t_simInfo)
{

}
