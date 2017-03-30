
#include "agent.h"
#include <map>


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


const Vector&
Agent::select_action(const Vector& input)
{
  ActivationGroup active_rules(_rules, input);
  PredictionGroup predictions(active_rules);
  return predictions.most_rewarding();
}
