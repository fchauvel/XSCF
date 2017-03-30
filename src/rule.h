#ifndef XSCF_RULE_H
#define XSCF_RULE_H


#include <vector>
#include <map>

#include "context.h"
#include "actions.h"


using namespace std;


class Interval
{
 public:
  Interval(const Value& lower, const Value& upper);
  Interval(const Interval& prototype);
  ~Interval();

  Interval& operator = (const Interval& prototype);

  bool contains(const Value value) const;
  
 private:
  Value _lower_bound;
  Value _upper_bound;

};


class Rule
{
 public:
  Rule(const vector<Interval>& constraints, const Vector& prediction, double fitness, double payoff);
  Rule(const Rule& prototype);
  ~Rule();

  Rule& operator = (const Rule& prototype);

  double fitness(void) const;
  double weighted_payoff(void) const;
  bool match(const Vector& input) const;
  const Vector& outputs(void) const;
  
 private:
  vector<Interval> _intervals;
  Vector _outputs;
  double _fitness;
  double _payoff;

};


class Population
{
 public:
  Population(void);
  Population(const Population& prototype);
  virtual ~Population(void);

  Population& operator = (const Population& population);
  const Rule& operator [] (unsigned int index) const;
 
  size_t size(void) const;
  
  Population& add(const Rule& rule);
  bool rewards_more_than(const Population& other) const;
  double average_payoff(void) const;

 private:
  vector<const Rule*> _rules;
  
};


class ActivationGroup: public Population
{
 public:
  ActivationGroup(const Population& rules, const Vector& context);
  ~ActivationGroup();

};


class PredictionGroup
{
 public:
  PredictionGroup(const Population& rules);
  //PredictionGroup(const PredictionGroup& other);
  ~PredictionGroup();
  
  //PredictionGroup& operator = (const PredictionGroup& other);
  const Vector& most_rewarding(void) const;
  
 private:
  map<const Vector*, Population*> _predictions;

};


class RuleFactory
{
 public:
  RuleFactory(void);
  virtual ~RuleFactory();
  
  virtual void initialise(Population& rule_set) const = 0;
  
};

#endif
