

#ifndef XCSF_TEST_HELPERS_H
#define XCSF_TEST_HELPERS_H


#include <vector>

#include "rule.h"


class TestRuleFactory: public RuleFactory
{
public:
  TestRuleFactory();
  virtual ~TestRuleFactory();
  
  virtual void initialise(Population& rule_set) const;

  void define(const Rule& rule);

private:
  mutable vector<const Rule*> _rules;
  
};


class Helper {
 public:
  static RuleFactory& one_rule(std::vector<int> predictions);

};


#endif
