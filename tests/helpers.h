

#ifndef XCSF_TEST_HELPERS_H
#define XCSF_TEST_HELPERS_H


#include <vector>

#include "rule.h"


using namespace xcsf;


class TestRuleFactory: public RuleFactory
{
public:
  TestRuleFactory();
  virtual ~TestRuleFactory();
  
  virtual void initialise(RuleSet& rule_set) const;

  void define(Rule& rule);

private:
  mutable vector<Rule*> _rules;
  
};


class Helper {
 public:
  static RuleFactory& one_rule(std::vector<int> predictions);

};


#endif
