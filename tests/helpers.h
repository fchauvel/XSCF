

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
  explicit TestableRandomizer(vector<double> sequence);
  virtual ~TestableRandomizer();

  virtual double uniform(void) const;

  void sequence(vector<double> sequence);
  
private:
  vector<double> _sequence;
  mutable unsigned int _next;

};



const bool NO_EVOLUTION = false;
const bool EVOLUTION = true;
const bool NO_MUTATION = false;
const bool MUTATION = true;


class FixedDecision: public Decision
{
public:
  FixedDecision(bool evolution, bool alelle);
  virtual ~FixedDecision();

  virtual bool shall_evolve(void) const;
  virtual bool shall_mutate(void) const;
  
private:
  bool _evolution;
  bool _allele;
  
};


#endif
