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

#include <cassert>
#include <sstream>
#include <stdexcept>

#include "removal.h"


using namespace xcsf;


Remover::Remover(MetaRulePool& pool, Selector selector)
  : _pool(pool)
  , _select_from(selector)
{}

Remover::~Remover()
{}


void
Remover::operator () (RuleSet& rule_set, unsigned int count)
{
  if (count > rule_set.size()) {
    stringstream message;
    message << "Impossible to remove " << count
	    << "from a rule set that contains only " << rule_set.size() << "!";
    throw std::invalid_argument(message.str());
  }
  
  for (unsigned int i=0 ; i<count ; ++i) {
    unsigned int worst_rule = _select_from(&rule_set);
    MetaRule& rule = rule_set.remove(worst_rule);
    _pool.release(&rule);
    //    _listener.on_rule_deleted(rule);
  }
  
}
