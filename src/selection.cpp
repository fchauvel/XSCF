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



#include <stdexcept>

#include "selection.h"



using namespace xcsf;



Selection::~Selection()
{}


RouletteWheel::RouletteWheel(const Randomizer& randomizer)
  :Selection()
  ,_generate(randomizer)
{}


RouletteWheel::~RouletteWheel()
{}

vector<Rule*>
RouletteWheel::operator () (const RuleSet& rules) const
{
  vector<Rule*> selected_rules(2);
  
  selected_rules[0] = select_one(rules, nullptr);
  selected_rules[1] = select_one(rules, selected_rules[0]);
 
  return selected_rules;
}


Rule*
RouletteWheel::select_one(const RuleSet& rules, Rule* selected) const
{
  double total_fitness = 0;
  for (unsigned int index=0 ; index<rules.size() ; ++index) {
    if (&rules[index] != selected) {
      total_fitness += rules[index].fitness();
    }
  }
  
  const double threshold = _generate.uniform() * total_fitness;
  double sum = 0;
  for (unsigned int index=0 ; index<rules.size() ; ++index) {
    if (&rules[index] != selected) {
      sum += rules[index].fitness();
      if (sum >= threshold) {
	return &rules[index];
      }
    }
  }

  throw std::logic_error("Roulette wheel error: No rule selected!");
}


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

