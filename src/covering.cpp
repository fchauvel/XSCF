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


#include "covering.h"


using namespace xcsf;


AbstractCovering::AbstractCovering(MetaRulePool& pool, unsigned int strength)
  : _rule_pool(pool)
  , _strength(strength)
{}


AbstractCovering::~AbstractCovering()
{}



unsigned int
AbstractCovering::strength(void) const
{
  return _strength;
}


MetaRulePool&
AbstractCovering::rule_pool(void) const
{
  return _rule_pool;
}



RandomCovering::RandomCovering(MetaRulePool& pool, unsigned int strength, const Randomizer& randomizer)
  : AbstractCovering(pool, strength)
  , _generate(randomizer)
{}


void
RandomCovering::operator () (RuleSet& rules, const Vector& context)
{
  for (unsigned int i=0 ; i<strength() ; ++i) {

    vector<Interval> premises;
    for (unsigned int index=0 ; index<rules.dimensions().input_count() ; ++index) {
      Value lower = context[index] - _generate.unsigned_int(0, 20);
      Value upper = context[index] + _generate.unsigned_int(0, 20);
      premises.push_back(Interval(lower, upper));
    }
    
    vector<unsigned int> conclusion;
    for (unsigned int index=0 ; index<rules.dimensions().output_count() ; ++index) {
      conclusion.push_back(_generate.unsigned_int(0, Value::MAXIMUM));
    }

    MetaRule *rule = rule_pool().acquire(Rule(premises, conclusion), Performance(0, 0, 0));
    rules.add(*rule);
  }
  
}
