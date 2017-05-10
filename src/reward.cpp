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


#include "reward.h"

#include <cmath>


using namespace xcsf;



RewardFunction::~RewardFunction()
{}


NaiveReward::NaiveReward(double learning_rate)
  : _learning_rate(learning_rate)
{}


NaiveReward::~NaiveReward()
{}


void
NaiveReward::operator () (double reward, RuleSet& rules) const
{
  double payoff[rules.size()];
  double accuracy[rules.size()];
  double total_accuracy(0);

  for (unsigned int index=0 ; index<rules.size() ; ++index){
    MetaRule& each_rule = rules[index];
    payoff[index] = each_rule.payoff() + _learning_rate * (reward - each_rule.payoff());
    accuracy[index] = std::min(each_rule.payoff(), reward) / std::max(each_rule.payoff(), reward);
    total_accuracy += accuracy[index];
  }

  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    MetaRule& each_rule = rules[index];
    double relative_accuracy =
      (std::isnormal(total_accuracy))
      ? accuracy[index] / total_accuracy
      : 0;
    double fitness = each_rule.fitness() + _learning_rate * (relative_accuracy - each_rule.fitness());
    each_rule.update(fitness, payoff[index], 0);
  }
}


WilsonReward::WilsonReward(double learning_rate, double error, double v)
  : _learning_rate(learning_rate)
  , _error(error) 
  , _v(v)
{}


WilsonReward::~WilsonReward()
{}


void
WilsonReward::operator () (double reward, RuleSet& rules) const
{
  double error[rules.size()];
  double payoff[rules.size()];
  double accuracy[rules.size()];
  double total_accuracy(0);

  for (unsigned int index=0 ; index<rules.size() ; ++index){
    MetaRule& each_rule = rules[index];
    payoff[index] = each_rule.payoff() + _learning_rate * (reward - each_rule.payoff());
    error[index] = each_rule.error() + _learning_rate * abs(reward - payoff[index]);
    
    accuracy[index] = 1.0;
    if (error[index] > _error) {
      accuracy[index] = 0.1 * pow(error[index] / _error, -_v);
    }
    total_accuracy += accuracy[index];
  }

  for(unsigned int index=0 ; index<rules.size() ; ++index) {
    MetaRule& each_rule = rules[index];
    double relative_accuracy =
      (std::isnormal(total_accuracy))
      ? accuracy[index] / total_accuracy
      : 0;
    double fitness = each_rule.fitness() + _learning_rate * (relative_accuracy - each_rule.fitness());
    each_rule.update(fitness, payoff[index], error[index]);
  }
}
