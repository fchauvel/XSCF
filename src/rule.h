#ifndef XSCF_RULE_H
#define XSCF_RULE_H


#include <vector>

#include "context.h"
#include "actions.h"


using namespace std;


class Interval
{
 public:
  Interval(Value lower, Value upper);
  Interval(const Interval& prototype);
  ~Interval();

  void operator = (const Interval& prototype);
  bool contains(const Value value) const;
  
 private:
  Value _lower_bound;
  Value _upper_bound;

};


class Rule {

 public:
  Rule(const vector<Interval>& constraints, Vector prediction, double fitness);
  Rule(const Rule& prototype);
  ~Rule();

  void operator = (const Rule& prototype);
  bool fits_better_than(const Rule& other) const;
  Interval& operator [](unsigned int index);
  bool match(const Vector& input) const;
  const Vector& outputs(void) const;
  
 private:
  vector<Interval> _intervals;
  Vector _outputs;
  double _fitness;

};


class Population
{
 public:
  Population(void);
  Population(const Population& prototype);
  ~Population(void);

  void operator = (const Population& population);
  std::size_t size(void) const;
  const Rule& operator [] (unsigned int index) const;
  const Rule& first(void);
  void add(const Rule& rule);

  const Rule& fittest(void) const;
  
 private:
  vector<const Rule*> _rules;
  
};


class RuleFactory
{
 public:
  RuleFactory(void);
  virtual ~RuleFactory();
  
  virtual void initialise(Population& rule_set) const = 0;
  
};

#endif
