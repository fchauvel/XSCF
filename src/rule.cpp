
#include <sstream>

#include "rule.h"


Interval::Interval(Value lower, Value upper)
  :_lower_bound(lower),
   _upper_bound(upper)
{}

Interval::Interval(const Interval& prototype)
  :_lower_bound(prototype._lower_bound),
   _upper_bound(prototype._upper_bound)
{}
  
Interval::~Interval()
{}

void
Interval::operator = (const Interval& prototype)
{
  _lower_bound = prototype._lower_bound;
  _upper_bound = prototype._upper_bound;
}

bool
Interval::contains(const Value value) const
{
  return _lower_bound <= value and _upper_bound >= value;
}



Rule::Rule(const vector<Interval>& constraints, Vector prediction, double fitness, double payoff):
  _intervals(constraints),
  _outputs(prediction),
  _fitness(fitness),
  _payoff(payoff)
{}


Rule::Rule(const Rule& prototype)
  :_intervals(prototype._intervals),
   _outputs(prototype._outputs),
   _fitness(prototype._fitness),
   _payoff(prototype._payoff)
{}


Rule::~Rule()
{}


double
Rule::fitness(void) const
{
  return _fitness;
}

double
Rule::weighted_payoff(void) const
{
  return _fitness * _payoff;
}


void
Rule::operator = (const Rule& prototype)
{
  _intervals = prototype._intervals;
  _outputs = prototype._outputs;
  _fitness = prototype._fitness;
  _payoff = prototype._payoff;
}


Interval&
Rule::operator [](unsigned int index)
{
  return _intervals[index];
}


bool
Rule::fits_better_than(const Rule& other) const
{
  return _fitness > other._fitness;
}


bool
Rule::match(const Vector& inputs) const
{
  if (inputs.size() != _intervals.size()) {
    stringstream message;
    message << "Invalid context! Expecting " << _intervals.size()
	    << " dimensions, but found only " << inputs.size()
	    << " instead."; 
    throw std::invalid_argument(message.str());
  }
  
  for(unsigned int index=0 ; index<_intervals.size() ; index++) {
    if (!_intervals[index].contains(inputs[index])) {
      return false;
    }
  }
  
  return true;
}


const Vector&
Rule::outputs(void) const
{
  return _outputs;
}




Population::Population():
  _rules()
{}


Population::Population(const Population& prototype)
  :_rules(prototype._rules)
{}


Population::~Population()
{}


void
Population::operator = (const Population& prototype)
{
  _rules = prototype._rules;
}


const Rule&
Population::operator [] (unsigned int index) const
{
  return *_rules[index];
}

std::size_t
Population::size(void) const
{
  return _rules.size();
}
  

const Rule&
Population::first(void)
{
  if (_rules.empty()) throw "Error, the population is empty!";
  return *_rules[0];
}

void
Population::add(const Rule& rule)
{
  _rules.push_back(&rule);
}

const Rule&
Population::fittest(void) const
{
  const Rule* fittest_rule = _rules[0];
  for (std::size_t index=0 ; index<_rules.size() ; ++index) {
    const Rule* any_rule = _rules[index];
    if (any_rule->fits_better_than(*fittest_rule)) {
      fittest_rule = any_rule;
    }
  }
  return *fittest_rule;
}


std::map<Vector, Population*>&
Population::groupByPredictions(void) const
{
  std::map<Vector, Population*>& byPrediction = *new std::map<Vector, Population*>();
  for(unsigned int index=0 ; index<_rules.size() ; index++){
    const Vector& prediction = _rules[index]->outputs();
    if (byPrediction.count(prediction) == 0) {
      Population* group = new Population();
      byPrediction[prediction] = group;
    }
    byPrediction[prediction]->add(*_rules[index]);
  }
  return byPrediction;
}


bool
Population::rewards_more_than(const Population& other) const
{
  return average_payoff() > other.average_payoff();
}

double
Population::average_payoff(void) const
{
  double total_fitness = 0;
  double total_weighted_payoff = 0;
  for(vector<const Rule*>::const_iterator each_rule = _rules.begin();
      each_rule != _rules.end();
      ++each_rule) {
    total_fitness += (*each_rule)->fitness();
    total_weighted_payoff += (*each_rule)->weighted_payoff();
  }
  return total_weighted_payoff / total_fitness;
}


RuleFactory::RuleFactory()
{}


RuleFactory::~RuleFactory()
{}


