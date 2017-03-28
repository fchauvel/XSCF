

#ifndef XSCF_AGENT_H
#define XSCF_AGENT_H


#include "context.h"
#include "rule.h"


class Agent
{
 public:
  Agent(const RuleFactory& factory);
  Agent(const Agent& prototype);
  virtual ~Agent();

  void operator = (const Agent& prototype);
  
  virtual const Vector& select_action(const Vector& input);
  
 private:
  const RuleFactory& _factory;
  Population& _rules;
  
};


#endif
