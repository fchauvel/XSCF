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


#ifndef XCSF_COVERING_H
#define XCSF_COVERING_H


#include "rule.h"


namespace xcsf
{

  class Covering
  {
  public:
    virtual ~Covering();

    virtual void
      operator () (RuleSet& rules, const Vector& context)
      const = 0;

  };



  class AbstractCovering
    : public Covering
  {
  public:
    virtual ~AbstractCovering();

    virtual void
      operator () (RuleSet& rules, const Vector& context)
      const = 0;

  protected:
    AbstractCovering(MetaRulePool& pool, unsigned int strength=1);

    unsigned int strength(void) const;
    MetaRulePool& rule_pool(void) const;

  private:
    MetaRulePool& _rule_pool;
    unsigned int _strength;

  };



  class RandomCovering
    : public AbstractCovering
  {
  public:
    RandomCovering(MetaRulePool&	pool,
		   unsigned int	strength,
		   const Randomizer&	randomizer);

    virtual void operator () (RuleSet& rules, const Vector& context) const;

  private:
    const Randomizer& _generate;

  };

}

#endif
