
#ifndef XCSF_CONTROLLER_H
#define XCSF_CONTROLLER_H

#include "agent.h"

class Controller
{
 public:
  Controller(Actuator& actuator, Agent&  agent);
  
  void select_action(const Context& context) const;
  
 private:
  Actuator& _actuator;
  Agent& _agent;
};


#endif
