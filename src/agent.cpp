
#include "agent.h"
#include <map>


Agent::Agent(const RuleFactory& factory)
  :_factory(factory),
   _rules(),
   _rules_to_reward()
{
  _factory.initialise(_rules);
}


Agent::Agent(const Agent& prototype)
  :_factory(prototype._factory),
   _rules(prototype._rules),
   _rules_to_reward(prototype._rules_to_reward)
{}
   
	    
Agent::~Agent()
{}


const Vector&
Agent::select_action(const Vector& input)
{
  ActivationGroup active_rules(_rules, input);
  PredictionGroup predictions(active_rules);
  _rules_to_reward = predictions.rules_to_reward();
  return predictions.most_rewarding();
}


void
Agent::reward(double reward)
{
  _rules_to_reward.reward(reward);
}
