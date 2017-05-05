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


#include "evolution.h"
#include <sstream>


using namespace xcsf;





Decision::Decision()
{}


Decision::~Decision()
{}


RandomDecision::RandomDecision(const Randomizer& generator,
			       double evolution_probability,
			       double allele_mutation_probability)
  : Decision()
  , _generator(generator)
  , _evolution_probability(evolution_probability)
  , _allele_mutation_probability(allele_mutation_probability)
{}


RandomDecision::~RandomDecision()
{}


bool
RandomDecision::shall_evolve(void) const
{
  return _generator.uniform() <= _evolution_probability;
}


bool
RandomDecision::shall_mutate(void) const
{
  return _generator.uniform() <= _allele_mutation_probability;
}



EvolutionListener::~EvolutionListener(void)
{};




LogListener::LogListener(std::ostream& out)
  : _out(out)
{};


LogListener::~LogListener()
{}


void
LogListener::on_rule_added(const Rule& rule) const
{
  _out << "New rule '" << rule << "'" << endl;
}


void
LogListener::on_rule_deleted(const Rule& rule) const
{
  _out << "Deleted rule '" << rule << "'" << endl;
}


void
LogListener::on_breeding(const Rule& father, const Rule& mother) const
{
  _out << "Breeding:" << endl
       << " - Father: " << father << endl
       << " - Mother: " << mother << endl;
}


void
LogListener::on_mutation(const Chromosome& subject, const Allele& locus) const
{
  _out << "Mutation of " << subject << " at " << locus << endl;
}


Evolution::Evolution(const Decision& decision,
		     const Crossover& crossover,
		     const Selection& selection,
		     const AlleleMutation& mutation,
		     const EvolutionListener& listener,
		     unsigned int input_count,
		     unsigned int output_count,
		     unsigned int capacity)
  : _decision(decision)
  , _crossover(crossover)
  , _select_parents(selection)
  , _mutate(mutation)
  , _listener(listener)
  , _input_count(input_count)
  , _output_count(output_count)
  , _capacity(capacity)
  , _rules()
{}


Evolution::~Evolution()
{
  for(auto each_rule: _rules) {
    delete each_rule;
  }
}


void
Evolution::evolve(RuleSet& rules) const
{
  if (not _decision.shall_evolve())
    return;

  vector<Rule*> parents = _select_parents(rules);
  
  vector<Rule*> children = breed(*parents[0], *parents[1]);
  for (auto each_child: children) {
    rules.add(*each_child);
  }

  enforce_capacity(rules);
}


void
Evolution::enforce_capacity(RuleSet& rules) const
{
  if (rules.size() > _capacity) {
    unsigned int excess = rules.size() - _capacity;

    for (unsigned int index=0 ; index<excess ; ++index) {
      unsigned int worst_rule = rules.worst();
      Rule& rule = rules.remove(worst_rule);
      _listener.on_rule_deleted(rule);
    }
  }
}


void
Evolution::create_rule_for(RuleSet& rules, const Vector& context) const
{
  create_rule(rules, context, 10, 50);
  enforce_capacity(rules);
}


void
Evolution::initialise(RuleSet& rules) const
{
  const Value delta(25);

  Vector rule_1 = { 25 };
  create_rule(rules, rule_1, delta, 25);
  
  Vector rule_2 = { 75 };
  create_rule(rules, rule_2, delta, 75);
}


void
Evolution::create_rule(RuleSet& rules, const Vector& seed, const Value& tolerance, const Value& prediction) const
{
  vector<Interval> constraints;
  for (unsigned int index=0 ; index<_input_count ; ++index) {
    constraints.push_back(Interval(seed[index]-tolerance, seed[index]+tolerance));
  }

  vector<unsigned int> predictions;
  for (unsigned int index=0 ; index<_output_count ; ++index) {
    predictions.push_back(static_cast<unsigned int>(prediction));
  }

  Rule *rule = make_rule(constraints, predictions);
  rules.add(*rule);
}


const double DEFAULT_PAYOFF(100);
const double DEFAULT_ERROR(0);
const double DEFAULT_FITNESS(100);


Rule*
Evolution::make_rule(std::vector<Interval> constraints, std::vector<unsigned int> predictions) const
{
  Rule* rule = new Rule(constraints, predictions, DEFAULT_FITNESS, DEFAULT_PAYOFF, DEFAULT_ERROR);
  _rules.push_back(rule);
  _listener.on_rule_added(*rule);
  return rule;
}


Chromosome
Evolution::encode(const Rule& rule) const
{
  return Chromosome(rule.as_vector());
}


Rule*
Evolution::decode(const Chromosome& values) const
{
  using namespace std;

  if (values.size() != 2 * _input_count + _output_count) {
    ostringstream error;
    error << "Input and output size ("
	  << _input_count << " & " << _output_count
	  << ") do not match the given vector size ("
	  << values.size() << ")!";
    throw invalid_argument(error.str());
  }

  vector<Interval> constraints;
  for (unsigned int index=0 ; index < _input_count ; ++index) {
    unsigned int lower_bound = values[index * 2];
    unsigned int upper_bound = values[index * 2 + 1];
    if (lower_bound > upper_bound) {
      std::swap(lower_bound, upper_bound);
    }
    constraints.push_back(Interval(lower_bound, upper_bound));
  }

  vector<unsigned int> prediction;
  for (unsigned int index=_input_count*2 ; index<values.size() ; ++index) {
    prediction.push_back(values[index]);
  }

  Vector p = Vector(prediction);
  Rule *result = new Rule(constraints, p, DEFAULT_FITNESS, DEFAULT_PAYOFF, DEFAULT_ERROR);
  return result;
}  


vector<Rule*>
Evolution::breed(const Rule& father, const Rule& mother) const
{
  vector<Chromosome> children;
  _crossover(encode(father), encode(mother), children);

  _listener.on_breeding(father, mother);
  
  vector<Rule*> children_rules;
  for (auto each_child: children) {
      mutate(each_child);
      Rule *rule = decode(each_child);
      children_rules.push_back(rule);
      _listener.on_rule_added(*rule);
      _rules.push_back(rule);
  }
  
  return children_rules;
}


void
Evolution::mutate(Chromosome& child) const
{
  unsigned int length = _input_count * 2 + _output_count;
  for(Allele each_locus=0 ; each_locus<length ; ++each_locus) {
    if (_decision.shall_mutate()) {
      _mutate(child, each_locus);
      _listener.on_mutation(child, each_locus);
    }
  }
}
