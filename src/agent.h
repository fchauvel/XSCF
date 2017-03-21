

#ifndef XSCF_AGENT_H
#define XSCF_AGENT_H


#include "context.h"
#include "population.h"
#include "actions.h"


class Agent
{
 public:
  Agent(Population& classifiers);
  virtual ~Agent();
  
  virtual const Action* const select_action(const Context& context);
  
 private:
  Population& _classifiers;
  
};


#endif
