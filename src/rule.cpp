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

#include <algorithm>
#include <sstream>
#include <cmath>
#include <cassert>
#include <iomanip>

#include "rule.h"


using namespace std;
using namespace xcsf;


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


void
Rule::accept(Formatter& formatter) const
{
  formatter.format(_intervals, _outputs, Performance(_fitness, _payoff, _error));
}


vector<unsigned int>
Rule::as_vector(void) const {
  vector<unsigned int> result;

  for (auto each_constraint: _intervals) {
    result.push_back(static_cast<unsigned int>(each_constraint.lower()));
    result.push_back(static_cast<unsigned int>(each_constraint.upper()));
  }
  
  for (unsigned int index=0 ; index<_outputs.size() ; ++index) {
    result.push_back(static_cast<unsigned int>(_outputs[index]));
  }
  
  return result;
}


void
Rule::update(double fitness, double payoff, double error) {
  assert(std::isfinite(fitness) && "Infinite or NaN fitness");
  assert(std::isfinite(payoff) && "Infinite or NaN payoff");
  assert(std::isfinite(error) && "Infinite or NaN error");

  _fitness = fitness;
  _payoff = payoff;
  _error = error;
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


double
Performance::fitness(void) const
{
  return _fitness;
}

double
Performance::payoff(void) const
{
  return _payoff;
}

double
Performance::error(void) const
{
  return _error;
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


RewardFunction::~RewardFunction()
{}


NaiveReward::NaiveReward(double learning_rate)
  : _learning_rate(learning_rate)
{}


NaiveReward::~NaiveReward()
{}


void
NaiveReward::operator () (double reward, vector<Rule*>& rules) const
{
  double payoff[rules.size()];
  double accuracy[rules.size()];
  double total_accuracy(0);

  for (unsigned int index=0 ; index<rules.size() ; ++index){
    Rule& each_rule = *rules[index];
    payoff[index] = each_rule.payoff() + _learning_rate * (reward - each_rule.payoff());
    accuracy[index] = std::min(each_rule.payoff(), reward) / std::max(each_rule.payoff(), reward);
    total_accuracy += accuracy[index];
  }

  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    Rule& each_rule = *rules[index];
    double relative_accuracy =
      (std::isnormal(total_accuracy))
      ? accuracy[index] / total_accuracy
      : 0;
    double fitness = each_rule.fitness() + _learning_rate * (relative_accuracy - each_rule.fitness());
    each_rule.update(fitness, payoff[index], 0);
  }
}


WilsonReward::WilsonReward(double learning_rate, double error, double v)
  : _learning_rate(learning_rate)
  , _error(error) 
  , _v(v)
{}


WilsonReward::~WilsonReward()
{}


void
WilsonReward::operator () (double reward, vector<Rule*>& rules) const
{
  double error[rules.size()];
  double payoff[rules.size()];
  double accuracy[rules.size()];
  double total_accuracy(0);

  for (unsigned int index=0 ; index<rules.size() ; ++index){
    Rule& each_rule = *rules[index];
    payoff[index] = each_rule.payoff() + _learning_rate * (reward - each_rule.payoff());
    error[index] = each_rule.error() + _learning_rate * abs(reward - payoff[index]);
    
    accuracy[index] = 1.0;
    if (error[index] > _error) {
      accuracy[index] = 0.1 * pow(error[index] / _error, -_v);
    }
    total_accuracy += accuracy[index];
  }

  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    Rule& each_rule = *rules[index];
    double relative_accuracy =
      (std::isnormal(total_accuracy))
      ? accuracy[index] / total_accuracy
      : 0;
    double fitness = each_rule.fitness() + _learning_rate * (relative_accuracy - each_rule.fitness());
    each_rule.update(fitness, payoff[index], error[index]);
  }
}


RuleSet::RuleSet()
  : _rules()
{}


RuleSet::~RuleSet()
{}


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
RuleSet::accept(Formatter& formatter) const
{
  formatter.format(_rules);
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


bool
RuleSet::empty(void) const
{
  return _rules.size() == 0;
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
  _rules.erase(_rules.begin() + index);
  
  return removed;
}


unsigned int
RuleSet::worst(void) const
{
  if (_rules.empty())
    throw std::invalid_argument("Unable to find the worst rule: The rule set is empty.");

  unsigned int worst = 0;
  for (unsigned int index=1 ; index<_rules.size() ; ++index) {
    if (_rules[worst]->weighted_payoff() > _rules[index]->weighted_payoff()) {
      worst = index;
    }
  }
  
  return worst;
}


double
RuleSet::total_fitness(void) const
{
  double total = 0;
  for(auto each_rule: _rules) {
    total += each_rule->fitness();
  }

  assert(std::isfinite(total) && "Infinite (or NaN) total fitness!");
  return total;
}


double
RuleSet::total_weighted_payoff(void) const
{
  double total = 0;
  for (auto each_rule: _rules) {
    total += each_rule->weighted_payoff();
  }

  assert(std::isfinite(total) && "Non finite total weighted payoff!");
  return total;
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
  WilsonReward strategy(0.25, 500, 2);
  //NaiveReward strategy(0.25);
  strategy(reward, _rules);
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


RuleFactory::~RuleFactory()
{}


Formatter::Formatter(std::ostream& out)
  : _out(out)
{}


void
Formatter::format(const vector<Rule*>& rules)
{
  const unsigned int rule_width(21);
  const string line(rule_width + 3 * 5, '-');
  _out << left << setw(rule_width) << "Rule"
       << right << setw(5) << "F."
       << right << setw(5) << "P."
       << right << setw(5) << "E." << endl;
  _out << line << endl;
  for (auto each_rule: rules) {
    each_rule->accept(*this);
  }
  _out << line << endl;
  _out << rules.size() << " rule(s)." << endl; 
}


void
Formatter::format(const vector<Interval>& antecedent,
		  const Vector& conclusion,
		  const Performance& performance)
{
  for(unsigned int i=0 ; i<antecedent.size() ; ++i) {
    _out << "("
	 << right << setw(2) << antecedent[i].lower()
	 << ", "
	 << right << setw(2) << antecedent[i].upper()
	 << ")";
  }

  _out << " => ";
  for(unsigned int i=0 ; i<conclusion.size() ; ++i) {
    _out << "("
	 << conclusion[i]
	 << ")";
  }
  _out << setw(5) << "";
  _out << std::right << setw(5) << fixed << setprecision(1) << performance.fitness();
  _out << std::right << setw(5) << fixed << setprecision(1) << performance.payoff();
  _out << std::right << setw(5) << fixed << setprecision(1) << performance.error();
  _out << endl;
}
