

#ifndef XCSF_TEST_HELPERS_H
#define XCSF_TEST_HELPERS_H


#include <vector>
#include "utils.h"
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


class TestableRandomizer: public Randomizer
{
public:
  TestableRandomizer(double number);
  virtual ~TestableRandomizer();

  virtual double uniform(void) const;
    
private:
  const double _random_number;

};


#endif
