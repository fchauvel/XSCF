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


#ifndef XCSF_SELECTION_H
#define XCSF_SELECTION_H

#include "utils.h"
#include "rule.h"


namespace xcsf
{

  
  class Selection
  {
  public:
    virtual ~Selection();
    
    virtual vector<Rule*> operator () (const RuleSet& rules) const = 0;
    
  };


  class RouletteWheel: public Selection
  {
  public:
    RouletteWheel(const Randomizer& generator);
    virtual ~RouletteWheel();

    virtual vector<Rule*> operator () (const RuleSet& rules) const;

  private:
    Rule* select_one(const RuleSet& rules, Rule* selected) const;
    const Randomizer& _generate;
    
  };


  class DummySelection: public Selection
  {
  public:
    virtual ~DummySelection();

    virtual vector<Rule*> operator () (const RuleSet& rules) const;
  };

  
}


#endif
