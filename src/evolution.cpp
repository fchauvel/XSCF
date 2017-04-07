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


Crossover::Crossover(unsigned int cut_point_A, unsigned int cut_point_B)
  : _cut_point_A(cut_point_A)
  , _cut_point_B(cut_point_B)
{
  if (_cut_point_A >= _cut_point_B) {
    ostringstream error;
    error << "Invalid cut-points! Left cut-point must be smaller than the right one";
    throw invalid_argument(error.str());
  }
}


Crossover::Crossover(const Crossover& other)
  : _cut_point_A(other._cut_point_A)
  , _cut_point_B(other._cut_point_B)
{}


Crossover::~Crossover()
{}


Crossover& 
Crossover::operator = (const Crossover& other)
{
  _cut_point_A = other._cut_point_A;
  _cut_point_B = other._cut_point_B;
  return *this;
}


bool
Crossover::operator == (const Crossover& other) const
{
  return _cut_point_A == other._cut_point_A
    and _cut_point_B == other._cut_point_B;
}


void
validate(const Chromosome father, const Chromosome& mother)
{ 
  if (father.size() != mother.size()) {
    ostringstream error;
    error << "Parents chromosomes have different length!"
	  << "(father=" << father.size() << ", but mother="
	  << mother.size() << ")";
    throw invalid_argument(error.str());
  }  
}


void
Crossover::breed(const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const
{
  validate(father, mother);

  if (_cut_point_B >= father.size()) {
    throw invalid_argument("The chromosomes are too short for the selected cut_points");
  }
  
  if (children.size() < 2) {
    throw invalid_argument("Crossovers generates at two children!");
  }
  
  for(unsigned int index=0 ; index<father.size() ; ++index) {
    if (_cut_point_A <= index and index < _cut_point_B) {
      children[0].push_back(mother[index]);
      children[1].push_back(father[index]);
    } else {
      children[0].push_back(father[index]);
      children[1].push_back(mother[index]);
    }
  }
}


Evolution::Evolution(const Crossover& crossover, unsigned int input_count, unsigned int output_count)
  : _crossover(crossover)
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


Chromosome
Evolution::encode(const Rule& rule) const {
  return Chromosome(rule.as_vector());
}


Rule*
Evolution::decode(const Chromosome& values) const {
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
Evolution::breed(const Rule& father, const Rule& mother)
{
  
  vector<Chromosome> children(2);
  _crossover.breed(encode(father), encode(mother), children);

  vector<Rule*> children_rules;
  for (auto each_child: children) {
      Rule *rule = decode(each_child);
      children_rules.push_back(rule);
      _rules.push_back(rule);
  }
  
  return children_rules;
}

