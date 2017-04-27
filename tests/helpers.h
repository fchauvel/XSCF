

#ifndef XCSF_TEST_HELPERS_H
#define XCSF_TEST_HELPERS_H


#include <vector>
#include "utils.h"
#include "rule.h"
#include "evolution.h"

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
  explicit TestableRandomizer(double number);
  virtual ~TestableRandomizer();

  virtual double uniform(void) const;

  void define_number(double number);
  
private:
  double _random_number;

};


const bool NO_EVOLUTION = false;


class FixedDecision: public Decision
{
public:
  FixedDecision(bool evolution);
  virtual ~FixedDecision();

  virtual bool shall_evolve(void) const;
  
private:
  bool _evolution;
  
};


#endif
