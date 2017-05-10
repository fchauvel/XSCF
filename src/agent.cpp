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



#include "agent.h"


using namespace xcsf;


Agent::Agent(const RuleFactory& factory)
  :_factory(factory),
   _rules(),
   _rules_to_reward()
{
  _factory.initialise(_rules);
}


Agent::Agent(const Agent& other)
  :_factory(other._factory),
   _rules(other._rules),
   _rules_to_reward(other._rules_to_reward)
{}
   
	    
Agent::~Agent()
{}


const Vector&
Agent::predict(const Vector& input)
{
  ActivationGroup active_rules(_rules, input);
  if (active_rules.empty()) {
    _factory.create_rule_for(_rules, input);
    active_rules = ActivationGroup(_rules, input);
  }

  PredictionGroup predictions(active_rules);
  _rules_to_reward = predictions.rules_to_reward();
  _factory.evolve(_rules);
  return predictions.most_rewarding();
}


void
Agent::reward(double reward)
{
  _rules_to_reward.reward(reward);
}


void
Agent::display_on(std::ostream& out) const
{
  Formatter formatter(out);
  _rules.accept(formatter);
}
