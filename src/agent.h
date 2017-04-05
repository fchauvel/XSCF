

#ifndef XCSF_AGENT_H
#define XCSF_AGENT_H


#include "context.h"
#include "rule.h"

namespace xcsf {

  class Agent
  {
  public:
    Agent(const RuleFactory& factory);
    Agent(const Agent& prototype);
    virtual ~Agent();

    void operator = (const Agent& prototype);
  
    virtual const Vector& select_action(const Vector& input);
    void reward(double reward);
  
  private:
    const RuleFactory& _factory;
    Population _rules;
    Population _rules_to_reward;
  };

}

#endif
