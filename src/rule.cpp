
#include <sstream>
#include <cmath>


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


Rule::Rule(const vector<Interval>& constraints, const Vector& prediction, double fitness, double payoff, double error):
  _intervals(constraints),
  _outputs(prediction),
  _fitness(fitness),
  _payoff(payoff),
  _error(error)
{}


Rule::Rule(const Rule& prototype)
  :_intervals(prototype._intervals),
   _outputs(prototype._outputs),
   _fitness(prototype._fitness),
   _payoff(prototype._payoff),
   _error(prototype._error)
{}


Rule::~Rule()
{}


Rule&
Rule::operator = (const Rule& prototype)
{
  _intervals = prototype._intervals;
  _outputs = prototype._outputs;
  _fitness = prototype._fitness;
  _payoff = prototype._payoff;
  _error = prototype._error;
  return *this;
}


void
Rule::reward(double reward) {
  const double BETA = 0.25;
  _payoff = _payoff + BETA * (reward - _payoff);
}


void
Rule::update(double payoff, double error, double fitness) {
  _payoff = payoff;
  _error = error;
  _fitness = fitness;
}


double
Rule::fitness(void) const
{
  return _fitness;
}


double
Rule::error(void) const
{
  return _error;
}


double
Rule::payoff(void) const
{
  return _payoff;
}


double
Rule::weighted_payoff(void) const
{
  return _fitness * _payoff;
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


Rule&
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
Population::add(Rule& rule)
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
  for(auto each_rule : _rules) {
    total_fitness += each_rule->fitness();
    total_weighted_payoff += each_rule->weighted_payoff();
  }
  return total_weighted_payoff / total_fitness;
}


void
Population::reward(double reward)
{
  const double BETA = 0.25;
  const double ERROR_THRESHOLD = 500;
  const unsigned int V_PARAM = 2;

  double error[_rules.size()];
  double payoff[_rules.size()];
  double accuracy[_rules.size()];
  double total_accuracy(0);

  for (unsigned int index=0 ; index<_rules.size() ; ++index){
    Rule& each_rule = *_rules[index];
    payoff[index] = each_rule.payoff() + BETA * (reward - each_rule.payoff());
    error[index] = each_rule.error() + BETA * abs(reward - payoff[index]);
    
    accuracy[index] = 1.0;
    if (error[index] > ERROR_THRESHOLD) {
      accuracy[index] = 0.1 * pow(error[index] / ERROR_THRESHOLD, -V_PARAM);
    }
    total_accuracy += accuracy[index];
  }

  for(unsigned int index=0 ; index<_rules.size() ; ++index) {
    Rule& each_rule = *_rules[index];
    double relative_accuracy = accuracy[index] / total_accuracy;
    double fitness = each_rule.fitness() + BETA * (relative_accuracy - each_rule.fitness());
    each_rule.update(payoff[index], error[index], fitness);
  }
}


ActivationGroup::ActivationGroup(Population& rules, const Vector& context)
  :Population()
{
  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    Rule& any_rule = rules[index];
    if (any_rule.match(context)) {
      add(any_rule);
    }
  }
}


ActivationGroup::~ActivationGroup()
{}



PredictionGroup::PredictionGroup(const Population& rules)
  :_predictions(),
   _most_rewarding()
{
  group_rules_by_prediction(rules);
  _most_rewarding = find_most_rewarding_rules();
}

Population&
PredictionGroup::find_most_rewarding_rules(void) const {
 std::map<Vector, Population*>::const_iterator most_rewarding = _predictions.begin();
  for(std::map<Vector, Population*>::const_iterator any = _predictions.begin() ;
      any != _predictions.end() ;
      ++any) {
    if (any->second->rewards_more_than(*(most_rewarding->second))) {
      most_rewarding = any;
    }
  }
  return *most_rewarding->second;
}


void
PredictionGroup::group_rules_by_prediction(const Population& rules) {
  for(unsigned int index=0 ; index<rules.size() ; index++){
    const Vector& prediction = rules[index].outputs();
    if (_predictions.count(prediction) == 0) {
      Population* group = new Population();
      _predictions[prediction] = group;
    }
    _predictions[prediction]->add(rules[index]);
  }
}


PredictionGroup::~PredictionGroup()
{
  for(std::map<Vector, Population*>::iterator each = _predictions.begin() ;
      each != _predictions.end() ;
      ++each) {
    delete each->second;
  }
}


const Vector&
PredictionGroup::most_rewarding(void) const {
  return _most_rewarding[0].outputs();
}





Population&
PredictionGroup::rules_to_reward(void) const {
  return _most_rewarding;
}


RuleFactory::RuleFactory()
{}


RuleFactory::~RuleFactory()
{}


