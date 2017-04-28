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


RandomDecision::RandomDecision(const Randomizer& generator, double evolution_probability, double allele_mutation_probability)
  :Decision()
  ,_generator(generator)
  ,_evolution_probability(evolution_probability)
  ,_allele_mutation_probability(allele_mutation_probability)
{}


RandomDecision::~RandomDecision()
{}


bool
RandomDecision::shall_evolve(void) const
{
  return _generator.uniform() > _evolution_probability;
}


bool
RandomDecision::shall_mutate(void) const
{
  return _generator.uniform() > _allele_mutation_probability;
}
  


Selection::~Selection()
{}


DummySelection::DummySelection()
{}

DummySelection::~DummySelection()
{}

vector<Rule*>
DummySelection::operator () (const RuleSet& rules) const
{
  vector<Rule*> selection;
  selection.push_back(&rules[0]);
  selection.push_back(&rules[1]);
  return selection;  
}


Evolution::Evolution(const Decision& decision,
		     const Crossover& crossover,
		     const Selection& selection,
		     const AlleleMutation& mutation,
		     unsigned int input_count,
		     unsigned int output_count)
  : _decision(decision)
  , _crossover(crossover)
  , _select_parents(selection)
  , _mutate(mutation)
  , _input_count(input_count)
  , _output_count(output_count)
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
    constraints.push_back(Interval(lower_bound, upper_bound));
  }

  vector<unsigned int> prediction;
  for (unsigned int index=_input_count*2 ; index<values.size() ; ++index) {
    prediction.push_back(values[index]);
  }

  Vector p = Vector(prediction);
  Rule *result = new Rule(constraints, p, 1.0, 1.0, 1.0);
  return result;
}  


vector<Rule*>
Evolution::breed(const Rule& father, const Rule& mother) const
{
  vector<Chromosome> children(2);
  _crossover(encode(father), encode(mother), children);

  vector<Rule*> children_rules;
  for (auto each_child: children) {
      mutate(each_child);
      Rule *rule = decode(each_child);
      children_rules.push_back(rule);
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
    }
  }
}
