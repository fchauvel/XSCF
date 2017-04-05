
#include <sstream>
#include <cmath>


#include "rule.h"


using namespace xcsf;


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




RuleSet::RuleSet():
  _rules()
{}


RuleSet::RuleSet(const RuleSet& prototype)
  :_rules(prototype._rules)
{}


RuleSet::~RuleSet()
{}


RuleSet&
RuleSet::operator = (const RuleSet& prototype)
{
  _rules = prototype._rules;
  return *this;
}


Rule&
RuleSet::operator [] (unsigned int index) const
{
  return *_rules[index];
}


std::size_t
RuleSet::size(void) const
{
  return _rules.size();
}


RuleSet&
RuleSet::add(Rule& rule)
{
  _rules.push_back(&rule);
  return *this;
}


bool
RuleSet::rewards_more_than(const RuleSet& other) const
{
  return average_payoff() > other.average_payoff();
}


double
RuleSet::average_payoff(void) const
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
RuleSet::reward(double reward)
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


ActivationGroup::ActivationGroup(RuleSet& rules, const Vector& context)
  :RuleSet()
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



PredictionGroup::PredictionGroup(const RuleSet& rules)
  :_predictions(),
   _most_rewarding()
{
  _group_rules_by_prediction(rules);
  _most_rewarding = _find_most_rewarding_rules();
}

RuleSet&
PredictionGroup::_find_most_rewarding_rules(void) const {
 std::pair<Vector, RuleSet*> most_rewarding = *_predictions.begin();
 for(auto any: _predictions) {
    if (any.second->rewards_more_than(*most_rewarding.second)) {
      most_rewarding = any;
    }
  }
  return *most_rewarding.second;
}


void
PredictionGroup::_group_rules_by_prediction(const RuleSet& rules) {
  for(unsigned int index=0 ; index<rules.size() ; index++){
    const Vector& prediction = rules[index].outputs();
    if (_predictions.count(prediction) == 0) {
      RuleSet* group = new RuleSet();
      _predictions[prediction] = group;
    }
    _predictions[prediction]->add(rules[index]);
  }
}


PredictionGroup::~PredictionGroup()
{
  for(auto each : _predictions) {
    delete each.second;
  }
}


const Vector&
PredictionGroup::most_rewarding(void) const {
  return _most_rewarding[0].outputs();
}


RuleSet&
PredictionGroup::rules_to_reward(void) const {
  return _most_rewarding;
}


RuleFactory::RuleFactory()
{}


RuleFactory::~RuleFactory()
{}


