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


Dimensions::Dimensions(unsigned int input_count, unsigned int output_count)
  : _input_count(input_count)
  , _output_count(output_count)
{
  if (_input_count == 0) {
    throw std::invalid_argument("Expecting at least one input, but found none.");
  }
  if (_output_count == 0) {
    throw std::invalid_argument("Expecting at least one output, but found none.");
  }
}


void
Dimensions::validate_inputs(const Vector& input) const
{
  if (input.size() == _input_count) return;

  stringstream message;
  message << "Expected " << _input_count << " input(s), but only "
	  << input.size() << " found instead.";
  throw std::invalid_argument(message.str());
  
}

bool
Dimensions::operator == (const Dimensions& other) const
{
  return _input_count == other._input_count
    and _output_count == other._output_count;
}

bool
Dimensions::operator != (const Dimensions& other) const
{
  return not (*this == other);
}


Rule::Rule(const vector<Interval>& premises, const Vector& conclusion)
  : _premises(premises)
  , _conclusion(conclusion)
  , _dimensions(premises.size(), conclusion.size())
{}


bool
Rule::is_triggered_by(const Vector& context) const
{
  _dimensions.validate_inputs(context);
  
  for (unsigned int i=0 ; i<_premises.size() ; ++i) {
    const Interval& any_interval = _premises[i];
    if ( not any_interval.contains(context[i]) ) {
      return false;
    }
  }
  
  return true;
}


bool
Rule::subsumes(const Rule& other) const
{
  if (_dimensions != other._dimensions) return false;
  if (_conclusion != other._conclusion) return false;

  for (unsigned int i=0 ; i<_premises.size() ; ++i) {
    if (not _premises[i].includes(other._premises[i])) {
      return false;
    }
  }
  
  return true;
}


const Vector&
Rule::conclusion() const
{
  return _conclusion;
}

const Vector&
Rule::operator () (const Vector& context) const
{
  if (is_triggered_by(context)) return _conclusion;

  stringstream message;
  message << "Rule '" << *this
	  << "' is undefined for input '" << context << "'.";
  throw std::invalid_argument(message.str());
}


bool
Rule::operator == (const Rule& other) const
{
  return _premises == other._premises
    and _conclusion == other._conclusion;
}


bool
Rule::operator != (const Rule& other) const
{
  return not Rule::operator == (other);
}
 

std::ostream&
xcsf::operator << (std::ostream& out, const Rule& rule)
{
  out << "(";
  for (unsigned int i=0 ; i<rule._premises.size() ; ++i) {
    const Interval& each = rule._premises[i];
    out << "[";
    out << right << setw(3) << each.lower();
    out << ", ";
    out << right << setw(3) << each.upper();
    out << "]";
    if (i < rule._premises.size() - 1)  {
      out << ", ";
    }
  }
  out << ") => (";
  for(unsigned int i=0 ; i<rule._conclusion.size() ;++i) {
    out << right << setw(3) << rule._conclusion[i];
    if (i < rule._conclusion.size() - 1) {
      out << ", ";
    }
  }
  out << ")";
  return out;
}


Rule::operator vector<unsigned int> () const
{
  vector<unsigned int> result;
  
  for (auto each_constraint: _premises) {
    result.push_back(static_cast<unsigned int>(each_constraint.lower()));
    result.push_back(static_cast<unsigned int>(each_constraint.upper()));
  }
  
  for (unsigned int index=0 ; index<_conclusion.size() ; ++index) {
    result.push_back(static_cast<unsigned int>(_conclusion[index]));
  }
  
  return result;
}


Performance::Performance(double fitness, double payoff, double error)
  :_fitness(fitness)
  ,_payoff(payoff)
  ,_error(error)
{}


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
  out << right << fixed << setprecision(2);
  out << "{ F = " <<  performance._fitness
      << " ; P = " <<  performance._payoff
      << " ; E = " <<  performance._error
      << " }";
    
  return out;
}



MetaRule::MetaRule(const Rule& rule, const Performance& performance)
  : _rule(rule)
  , _performance(performance)
{}

bool
MetaRule::operator == (const MetaRule& other) const
{
  return _rule == other._rule;
}


bool
MetaRule::operator != (const MetaRule& other) const
{
  return not (*this == other);
}


void
MetaRule::accept(Formatter& formatter) const
{
  formatter.format(_rule, _performance);
}


vector<unsigned int>
MetaRule::as_vector(void) const {
  return static_cast<vector<unsigned int>>(_rule);
}


void
MetaRule::update(double fitness, double payoff, double error) {
  assert(std::isfinite(fitness) && "Infinite or NaN fitness");
  assert(std::isfinite(payoff) && "Infinite or NaN payoff");
  assert(std::isfinite(error) && "Infinite or NaN error");

  _performance = Performance(fitness, payoff, error);
}



double
MetaRule::fitness(void) const
{
  return _performance.fitness();
}


double
MetaRule::error(void) const
{
  return _performance.error();
}


double
MetaRule::payoff(void) const
{
  return _performance.payoff();
}


double
MetaRule::weighted_payoff(void) const
{
  return _performance.fitness() * _performance.payoff();
}


bool
MetaRule::match(const Vector& inputs) const
{
  return _rule.is_triggered_by(inputs);
}


const Vector&
MetaRule::outputs(void) const
{
  return _rule.conclusion();
}


ostream&
xcsf::operator << (ostream& out, const MetaRule& rule)
{
  out << rule._rule << " " << rule._performance;
  return out;
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


MetaRule&
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
RuleSet::add(MetaRule& rule)
{
  _rules.push_back(&rule);
  return *this;
}

MetaRule&
RuleSet::remove(unsigned int index)
{
  validate(index);
  
  MetaRule& removed = *_rules[index];
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



ActivationGroup::ActivationGroup(RuleSet& rules, const Vector& context)
  :RuleSet()
{
  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    MetaRule& any_rule = rules[index];
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
Formatter::format(const vector<MetaRule*>& rules)
{
  const unsigned int width(6);
  const unsigned int rule_width(27);
  const string line(rule_width + 3 * width, '-');
  _out << right << setw(width) << "F."
       << right << setw(width) << "P."
       << right << setw(width) << "E."
       << right << setw(3) << ""
       << left << "Rule"<< endl;
  _out << line << endl;
  for (auto each_rule: rules) {
    each_rule->accept(*this);
  }
  _out << line << endl;
  _out << rules.size() << " rule(s)." << endl; 
}


void
Formatter::format(const Rule& rule, const Performance& performance)
{

  _out << std::right << fixed << setprecision(1);
  _out << setw(6) << performance.fitness();
  _out << setw(6) << performance.payoff();
  _out << setw(6) << performance.error();
  _out << setw(3) << "";
  _out << std::left << rule;
  _out << endl;
}


MetaRulePool::~MetaRulePool()
{
  for (auto each_rule: _active_rules) {
    delete each_rule;
  }

  for (auto each_rule: _free_rules) {
    delete each_rule;
  }
}


MetaRule*
MetaRulePool::acquire(const Rule& rule, const Performance& performance)
{
  if (_free_rules.empty()) {
    MetaRule *meta_rule = new MetaRule(rule, performance);
    _active_rules.push_back(meta_rule);
    return meta_rule;
  }

  MetaRule *meta_rule = _free_rules.back();
  *meta_rule = MetaRule(rule, performance);
  _free_rules.pop_back();
  _active_rules.push_back(meta_rule);
  return meta_rule;
}

void
MetaRulePool::release(MetaRule *rule)
{
  if (is_free(rule)) { return; }

  if (is_active(rule)) {
    _active_rules.remove(rule);
    _free_rules.push_back(rule);
    return;
  }
  
  stringstream message;
  message << "Rule at address " << rule << " is not managed by this pool!";
  throw std::invalid_argument(message.str()); 
}


bool
MetaRulePool::is_active(MetaRule* rule) const
{
  return std::find(_active_rules.begin(), _active_rules.end(), rule) != std::end(_active_rules);
}

bool
MetaRulePool::is_free(MetaRule* rule) const
{
  return std::find(_free_rules.begin(), _free_rules.end(), rule) != std::end(_free_rules);
}


unsigned int
MetaRulePool::active_rule_count(void) const
{
  return _active_rules.size();
}


unsigned int
MetaRulePool::free_rule_count(void) const
{
  return _free_rules.size();
}
