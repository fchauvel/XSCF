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

#include "CppUTest/TestHarness.h"

#include <sstream>

#include "rule.h"


using namespace xcsf;

const double TOLERANCE(1e-6);


TEST_GROUP(TestNaiveReward)
{
  const double learning_rate = 1;
  const double fitness = 0.0;
  const double payoff = 0.0;

  MetaRule *rule;
  vector<MetaRule*> rules;
  RewardFunction* reward;
  
  void setup(void) {
    rule = new MetaRule({ Interval(0, 100) }, { 50 }, fitness, payoff, 0);
    rules.push_back(rule);
    reward = new NaiveReward(learning_rate);
  }

  void teardown(void) {
    delete rule;
    delete reward;
  }
  
};


TEST(TestNaiveReward, zero_reward)
{
  (*reward)(0, rules);

  DOUBLES_EQUAL(0, rule->fitness(), TOLERANCE);
  DOUBLES_EQUAL(0, rule->payoff(), TOLERANCE);
}

TEST(TestNaiveReward, 100_reward)
{
  (*reward)(100, rules);

  DOUBLES_EQUAL(0., rule->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule->payoff(), TOLERANCE);
}

TEST(TestNaiveReward, 2x100_reward)
{
  (*reward)(100, rules);
  (*reward)(100, rules);

  DOUBLES_EQUAL(1., rule->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule->payoff(), TOLERANCE);
}


TEST_GROUP(TestNaiveRewardWithTwoRules)
{
  const double learning_rate = 1;
  const double fitness = 0.0;
  const double payoff = 0.0;

  MetaRule *rule_1, *rule_2;
  vector<MetaRule*> rules;
  RewardFunction* reward;
  
  void setup(void) {
    rule_1 = new MetaRule({ Interval(0, 100) }, { 50 }, fitness, payoff, 0);
    rules.push_back(rule_1);
    rule_2 = new MetaRule({ Interval(0, 100) }, { 50 }, fitness, payoff, 0);
    rules.push_back(rule_2);
    reward = new NaiveReward(learning_rate);
  }

  void teardown(void) {
    delete rule_1;
    delete rule_2;
    delete reward;
  }
  
};


TEST(TestNaiveRewardWithTwoRules, zero_reward)
{
  (*reward)(0, rules);

  DOUBLES_EQUAL(0, rule_1->fitness(), TOLERANCE);
  DOUBLES_EQUAL(0, rule_1->payoff(), TOLERANCE);
}

TEST(TestNaiveRewardWithTwoRules, 100_reward)
{
  (*reward)(100, rules);

  DOUBLES_EQUAL(0, rule_1->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule_1->payoff(), TOLERANCE);

  DOUBLES_EQUAL(0, rule_2->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule_2->payoff(), TOLERANCE);
}

TEST(TestNaiveRewardWithTwoRules, 2x100_reward)
{
  (*reward)(100, rules);
  (*reward)(100, rules);

  DOUBLES_EQUAL(0.5, rule_1->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule_1->payoff(), TOLERANCE);

  DOUBLES_EQUAL(0.5, rule_2->fitness(), TOLERANCE);
  DOUBLES_EQUAL(100, rule_2->payoff(), TOLERANCE);
}
