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
#include <stdexcept>
#include <cmath>
#include <cassert>

#include "selection.h"



using namespace xcsf;



Selection::~Selection()
{}


RouletteWheel::RouletteWheel(const Randomizer& randomizer)
  : Selection()
  , _generate(randomizer)
{}


RouletteWheel::~RouletteWheel()
{}

vector<MetaRule*>
RouletteWheel::operator () (const RuleSet& rules) const
{
  if (rules.size() < 2) {
    stringstream error;
    error << "Selecting parents requires at least two rules (found " << rules.size() << ")." << endl;
    throw invalid_argument(error.str());
  }
  
  vector<MetaRule*> selected_rules(2);
  
  selected_rules[0] = select_one(rules, nullptr);
  selected_rules[1] = select_one(rules, selected_rules[0]);
 
  return selected_rules;
}


MetaRule*
RouletteWheel::select_one(const RuleSet& rules, MetaRule* selected) const
{

  double total_payoff = rules.total_weighted_payoff();
  if (selected != nullptr) {
    total_payoff -= selected->weighted_payoff();
  }

  const double threshold = _generate.uniform() * total_payoff;

  double sum = 0;
  for (unsigned int index=0 ; index<rules.size() ; ++index) {
    if (&rules[index] != selected) {
      sum += rules[index].weighted_payoff();
      if (sum >= threshold) {
	return &rules[index];
      }
    }
  }

  throw std::logic_error("Roulette wheel error: No rule selected!");
}


DummySelection::~DummySelection()
{}


vector<MetaRule*>
DummySelection::operator () (const RuleSet& rules) const
{
  vector<MetaRule*> selection;
  selection.push_back(&rules[0]);
  selection.push_back(&rules[1]);
  return selection;  
}

