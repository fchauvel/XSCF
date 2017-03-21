
#include "agent.h"


Agent::Agent(Population& classifiers):
  _classifiers(classifiers)
{}

Agent::~Agent()
{}


const Action* const
Agent::select_action(const Context& context)
{
  Action* selection = new Action(4);
  return selection;
}
