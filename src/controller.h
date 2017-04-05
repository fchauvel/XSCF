
#ifndef XCSF_CONTROLLER_H
#define XCSF_CONTROLLER_H

#include "agent.h"


namespace xcsf {

  
  class Controller
  {
  public:
    Controller(Actuator& actuator, Agent&  agent);
    ~Controller();
  
    void select_action(const Vector& input) const;
  
  private:
    Actuator& _actuator;
    Agent& _agent;
  };


}

#endif
