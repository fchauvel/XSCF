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



#include "helpers.h"



TestRuleFactory::TestRuleFactory()
  :RuleFactory(),
   _rules()
{}


TestRuleFactory::~TestRuleFactory()
{
  for (auto each_rule: _rules) {
    delete each_rule;
  }
}


void
TestRuleFactory::initialise(RuleSet& rule_set) const
{
  for (auto each_predefined_rule: _rules) {
    rule_set.add(*each_predefined_rule);
  }
};


void
TestRuleFactory::define(Rule& rule)
{
  _rules.push_back(&rule);
}



TestableRandomizer::TestableRandomizer(double number)
  :Randomizer()
  ,_random_number(number)
{};


TestableRandomizer::~TestableRandomizer()
{};


double
TestableRandomizer::uniform(void) const
{
  return _random_number;
}

void
TestableRandomizer::define_number(double number)
{
  _random_number = number;
}



FixedDecision::FixedDecision(bool evolution)
  : Decision()
  , _evolution(evolution)
{}

FixedDecision::~FixedDecision()
{}

bool
FixedDecision::shall_evolve(void) const {
  return _evolution;
};
