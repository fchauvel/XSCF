
#include "agent.h"


Agent::Agent(const RuleFactory& factory)
  :_factory(factory),
   _rules(*new Population())
{
  _factory.initialise(_rules);
}


Agent::Agent(const Agent& prototype)
  :_factory(prototype._factory),
   _rules(prototype._rules)
{}
   
	    
Agent::~Agent()
{
  delete &_rules;
}


void
Agent::operator = (const Agent& prototype)
{
  
}


const Vector&
Agent::select_action(const Vector& input)
{
  return _rules.fittest().outputs();
}
