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


#ifndef XCSF_REWARD_H
#define XCSF_REWARD_H


#include "rule.h"


namespace xcsf
{

  class RewardFunction
  {
  public:
    virtual ~RewardFunction();

    virtual void operator () (double reward, RuleSet& rules) const = 0;
  };


  class NaiveReward
    : public RewardFunction
  {
  public:
    NaiveReward(double learning_rate);
    virtual ~NaiveReward();

    virtual void operator () (double reward, RuleSet& rules) const;
    
  private:
    double _learning_rate;
    
  };

  
  class WilsonReward
    : public RewardFunction
  {
  public:
    WilsonReward(double learning_rate, double error, double v);
    virtual ~WilsonReward();

    virtual void operator () (double reward, RuleSet& rules) const;

  private:
    double _learning_rate;
    double _error;
    double _v;

  };

}


#endif
