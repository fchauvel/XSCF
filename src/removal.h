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

#ifndef XCSF_REMOVAL_H
#define XCSF_REMOVAL_H


#include <functional>

#include "rule.h"


namespace xcsf {

  
  typedef std::function<unsigned int (RuleSet*)> Selector;

  
  class Remover
  {
  public:
    Remover(MetaRulePool& pool, Selector selector);
    virtual ~Remover();

    virtual void operator () (RuleSet& rule_set, unsigned int count=1);

  private:
    MetaRulePool& _pool;
    Selector _select_from;
    
  };
  
};


#endif
