/*
 * This file is part of XCSF.
 *
 * XCSF is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * XCSF is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with XCSF.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <sstream>
#include <cmath>
#include <iomanip>

#include "rule.h"


using namespace std;
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
Interval::operator = (const Interval& other)
{
  _lower_bound = other._lower_bound;
  _upper_bound = other._upper_bound;
  return *this;
}


bool
Interval::operator == (const Interval& other) const
{
  return _lower_bound == other._lower_bound
    and _upper_bound == other._upper_bound;
}


bool
Interval::operator != (const Interval& other) const
{
  return not (*this == other);
}


const Value&
Interval::lower(void) const
{
  return _lower_bound;
}


const Value&
Interval::upper(void) const
{
  return _upper_bound;
}


bool
Interval::contains(const Value value) const
{
  return _lower_bound <= value and _upper_bound >= value;
}


ostream&
xcsf::operator << (ostream& out, const Interval& interval)
{
  out << "[" << interval._lower_bound
      << "," << interval._upper_bound
      << "]";
  return out;
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


bool
Rule::operator == (const Rule& other_rule) const
{
  return _intervals == other_rule._intervals
    and _outputs == other_rule._outputs;
}


bool
Rule::operator != (const Rule& other_rule) const
{
  return not (*this == other_rule);
}


vector<unsigned int>
Rule::as_vector(void) const {
  vector<unsigned int> result;
  for (auto each_constraint: _intervals) {
    result.push_back(each_constraint.lower().unsigned_int());
    result.push_back(each_constraint.upper().unsigned_int());
  }
  
  for (unsigned int index=0 ; index<_outputs.size() ; ++index) {
    result.push_back(_outputs[index].unsigned_int());
  }
  
  return result;
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

ostream&
xcsf::operator << (ostream& out, const Rule& rule)
{
  out << "{";
  for (auto each_constraint: rule._intervals) {
    out << each_constraint << " " ;
  }
  out << "} => " << rule._outputs;
  return out;
}



Performance::Performance(double fitness, double payoff, double error)
  :_fitness(fitness)
  ,_payoff(payoff)
  ,_error(error)
{}


Performance::Performance(const Performance& other)
  :_fitness(other._fitness)
  ,_payoff(other._payoff)
  ,_error(other._error)
{}


Performance::~Performance()
{}


Performance&
Performance::operator = (const Performance& other)
{
  _fitness = other._fitness;
  _payoff = other._payoff;
  _error = other._error;
  return *this;
}

bool
Performance::operator == (const Performance& other) const
{
  return _fitness == other._fitness
    and _payoff == other._payoff
    and _error == other._error;
  
}


bool
Performance::operator != (const Performance& other) const
{
  return not (*this == other);
}


ostream&
xcsf::operator << (ostream& out, const Performance performance)
{
  out << fixed <<  setprecision(2);
  out << "{ F = " << performance._fitness
      << " ; P = " << performance._payoff
      << " ; E = " << performance._error << " }";
    
  return out;
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


bool
RuleSet::operator == (const RuleSet& other) const
{
  return _rules == other._rules;
}


Rule&
RuleSet::operator [] (unsigned int index) const
{
  validate(index);
  return *_rules[index];
}


std::size_t
RuleSet::size(void) const
{
  return _rules.size();
}


void
RuleSet::validate(unsigned int index) const
{
  if (index < _rules.size()) return;

  stringstream error;
  error << "Invalid index " << index
	<< ". there are only " << _rules.size() << " rule(s)." << endl;
  throw std::invalid_argument(error.str());
}


RuleSet&
RuleSet::add(Rule& rule)
{
  _rules.push_back(&rule);
  return *this;
}

Rule&
RuleSet::remove(unsigned int index)
{
  validate(index);
  
  Rule& removed = *_rules[index];
  _rules.erase(_rules.begin()+ index);
  
  return removed;
}


unsigned int
RuleSet::worst(void) const
{
  if (_rules.empty())
    throw std::invalid_argument("Unable to find the worst rule: The rule set is empty.");

  unsigned int worst = 0;
  for (unsigned int index=1 ; index<_rules.size() ; ++index) {
    if (_rules[worst]->fitness() < _rules[index]->fitness()) {
      worst = index;
    }
  }
  
  return worst;
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


