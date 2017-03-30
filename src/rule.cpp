
#include <sstream>

#include "rule.h"


Interval::Interval(const Value& lower, const Value& upper)
  :_lower_bound(lower),
   _upper_bound(upper)
{}


Interval::Interval(const Interval& prototype)
  :_lower_bound(prototype._lower_bound),
   _upper_bound(prototype._upper_bound)
{}


Interval::~Interval()
{}


Interval&
Interval::operator = (const Interval& prototype)
{
  _lower_bound = prototype._lower_bound;
  _upper_bound = prototype._upper_bound;
  return *this;
}


bool
Interval::contains(const Value value) const
{
  return _lower_bound <= value and _upper_bound >= value;
}



Rule::Rule(const vector<Interval>& constraints, const Vector& prediction, double fitness, double payoff):
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


Rule&
Rule::operator = (const Rule& prototype)
{
  _intervals = prototype._intervals;
  _outputs = prototype._outputs;
  _fitness = prototype._fitness;
  _payoff = prototype._payoff;
  return *this;
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


Population&
Population::operator = (const Population& prototype)
{
  _rules = prototype._rules;
  return *this;
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


Population&
Population::add(const Rule& rule)
{
  _rules.push_back(&rule);
  return *this;
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



ActivationGroup::ActivationGroup(const Population& rules, const Vector& context)
  :Population()
{
  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    const Rule& any_rule = rules[index];
    if (any_rule.match(context)) {
      add(any_rule);
    }
  }
}


ActivationGroup::~ActivationGroup()
{}



PredictionGroup::PredictionGroup(const Population& rules)
  :_predictions()
{
  for(unsigned int index=0 ; index<rules.size() ; index++){
    const Vector& prediction = rules[index].outputs();
    if (_predictions.count(&prediction) == 0) {
      Population* group = new Population();
      _predictions[&prediction] = group;
    }
    _predictions[&prediction]->add(rules[index]);
  }
}


PredictionGroup::~PredictionGroup()
{
  for(std::map<const Vector*, Population*>::iterator each = _predictions.begin() ;
      each != _predictions.end() ;
      ++each) {
    delete each->second;
  }
}


const Vector&
PredictionGroup::most_rewarding(void) const {
  std::map<const Vector*, Population*>::const_iterator most_rewarding = _predictions.begin();
  for(std::map<const Vector*, Population*>::const_iterator any = _predictions.begin() ;
      any != _predictions.end() ;
      ++any) {
    if (any->second->rewards_more_than(*(most_rewarding->second))) {
      most_rewarding = any;
    }
  }
  return *(most_rewarding->first);
}


RuleFactory::RuleFactory()
{}


RuleFactory::~RuleFactory()
{}


