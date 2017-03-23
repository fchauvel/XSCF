
#include "agent.h"


Agent::Agent(Population& classifiers):
  _classifiers(classifiers)
{}

Agent::~Agent()
{}


const Action* const
Agent::select_action(const Context& context)
{
  return new Action(_classifiers.first()->outputs()[0].to_unsigned_int());
}
