

#include "controller.h"


Controller::Controller(Actuator& actuator, Agent& agent):
  _actuator(actuator),
  _agent(agent)    
{}


Controller::~Controller()
{}


void
Controller::select_action(const Context& context) const
{
  const Action* const action = _agent.select_action(context);
  action->execute_on(_actuator);
  delete action;
};

