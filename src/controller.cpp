

#include "controller.h"


using namespace xcsf;


Controller::Controller(Actuator& actuator, Agent& agent):
  _actuator(actuator),
  _agent(agent)    
{}


Controller::~Controller()
{}


void
Controller::select_action(const Vector& inputs) const
{
  const Vector& prediction = _agent.select_action(inputs);
  Action action(prediction[0].unsigned_int());
  action.execute_on(_actuator);
};

