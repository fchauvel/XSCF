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


FakeCovering::~FakeCovering()
{}


void
FakeCovering::operator () (RuleSet& rules, const Vector& context) const
{}



TestRuleFactory::TestRuleFactory()
  : Evolution()
  , _rules()
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
TestRuleFactory::evolve(RuleSet& rules) const
{}


void
TestRuleFactory::create_rule_for(RuleSet& rules, const Vector& context) const
{
  MetaRule* covering_rule = new MetaRule(Rule({ Interval(0, 100) }, { 50 }),
					 Performance(1., 1., 1.));
  _rules.push_back(covering_rule);
  rules.add(*covering_rule);
}


void
TestRuleFactory::define(MetaRule& rule)
{
  _rules.push_back(&rule);
}



TestableRandomizer::TestableRandomizer(vector<double> sequence)
  : Randomizer()
  , _sequence(sequence)
  , _next(0)
{};


TestableRandomizer::~TestableRandomizer()
{};


double
TestableRandomizer::uniform(void) const
{
  double random_number = _sequence[_next];
  _next += 1;
  if (_next >= _sequence.size()) _next = 0;
  return random_number;
}

void
TestableRandomizer::sequence(vector<double> sequence)
{
  _sequence = sequence;
}



FixedDecision::FixedDecision(bool evolution, bool allele)
  : Decision()
  , _evolution(evolution)
  , _allele(allele)
{}

FixedDecision::~FixedDecision()
{}

bool
FixedDecision::shall_evolve(void) const {
  return _evolution;
};

bool
FixedDecision::shall_mutate(void) const {
  return _allele;
}
