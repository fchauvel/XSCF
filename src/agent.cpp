
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
  std::map<Vector, Population*>& byPredictions = _rules.groupByPredictions();  

  std::map<Vector, Population*>::iterator most_rewarding = byPredictions.begin();
  for(std::map<Vector, Population*>::iterator any = byPredictions.begin() ;
      any != byPredictions.end() ;
      ++any) {
    if (any->second->rewards_more_than(*(most_rewarding->second))) {
      most_rewarding = any;
    }
  }

  const Vector& prediction = most_rewarding->second->first().outputs();
  
  for(std::map<Vector, Population*>::iterator each = byPredictions.begin() ;
      each != byPredictions.end() ;
      ++each) {
    delete each->second;
  }

  delete &byPredictions;
  return prediction;
}
