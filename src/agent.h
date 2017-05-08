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


#ifndef XCSF_AGENT_H
#define XCSF_AGENT_H


#include "context.h"
#include "rule.h"

namespace xcsf {

  class Agent
  {
  public:
    Agent(const RuleFactory& factory);
    Agent(const Agent& prototype);
    virtual ~Agent();

    void operator = (const Agent& prototype);
  
    virtual const Vector& predict(const Vector& input);
    void reward(double reward);
    void display_on(std::ostream& out) const;
  
  private:
    const RuleFactory& _factory;
    RuleSet _rules;
    RuleSet _rules_to_reward;
  };

}

#endif
