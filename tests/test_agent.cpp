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

#include <cmath>
#include <sstream>

#include "agent.h"
#include "rule.h"

#include "helpers.h"


using namespace std;
using namespace xcsf;


TEST_GROUP(OneRuleAgent)
{
  Covering *covering;
  RewardFunction *reward;
  TestRuleFactory evolution;
  Agent* agent;
  vector<int> predictions = { 4 };
  MetaRule *rule;

  void setup(void)
  {
    covering = new FakeCovering();
    reward = new WilsonReward(0.25, 500, 2);
    rule = new MetaRule(Rule({Interval(0, 50)}, predictions),
			Performance(1.0, 1.0, 1.0));
    evolution.define(*rule);
    agent = new Agent(evolution, *covering, *reward);
  }

  void teardown(void)
  {
    delete agent;
    delete reward;
    delete covering;
  }

};


TEST(OneRuleAgent, test_predict_the_single_active_rule)
{
  const Vector context({ 1 });

  const Vector& actual = agent->predict(context);

  CHECK(Vector(predictions) == actual);
}


TEST(OneRuleAgent, test_reward)
{
  const Vector context({ 1 });

  agent->predict(context);
  agent->reward(10);

  DOUBLES_EQUAL(3.25, rule->weighted_payoff(), 1e-6);
}



TEST_GROUP(TwoRulesAgent)
{
  Covering *covering;
  RewardFunction *reward;
  TestRuleFactory evolution;
  Agent* agent;
  MetaRule *rule_1, *rule_2;

  void setup(void)
  {
    covering = new FakeCovering();
    reward = new WilsonReward(0.25, 500, 2);

    rule_1 = new MetaRule(Rule({Interval(0, 49)}, { 4 }),
			  Performance(1.0, 1.0, 1.0));
    evolution.define(*rule_1);

    rule_2 = new MetaRule(Rule({Interval(40, 100)}, { 3 }),
			  Performance(1.0, 1.0, 1.0));
    evolution.define(*rule_2);

    agent = new Agent(evolution, *covering, *reward);
  }

  void teardown(void)
  {
    delete agent;
    delete reward;
    delete covering;
  }

};


TEST(TwoRulesAgent, test_display)
{
  stringstream out;
  agent->display_on(out);

  stringstream expected;
  expected << "    F.    P.    E.   Rule" << endl
	   << "---------------------------------------------" << endl
	   << "   1.0   1.0   1.0   ([  0,  49]) => (  4)" << endl
	   << "   1.0   1.0   1.0   ([ 40, 100]) => (  3)" << endl
	   << "---------------------------------------------" << endl
	   << "2 rule(s)." << endl;

  CHECK_EQUAL(expected.str(), out.str());
}


TEST(TwoRulesAgent, test_predict_active_rule)
{
  const Vector context({ 25 });
  const Vector& actual = agent->predict(context);

  CHECK(Vector({ 4 }) == actual);
}



TEST_GROUP(OverlappingRulesAgent)
{
  Covering		*covering;
  RewardFunction	*reward; 
  TestRuleFactory	 evolution;
  Agent			*agent;
  MetaRule		*rule_1, *rule_2, *rule_3;

  void setup(void)
  {
    covering = new FakeCovering();
    reward   = new WilsonReward(0.25, 500, 2);
    rule_1   = new MetaRule(Rule({Interval(0, 100)}, { 4 }),
			  Performance(1.0, 1.0, 1.0));
    rule_2   = new MetaRule(Rule({Interval(0, 100)}, { 4 }),
			  Performance(0.8, 0.8, 1.0));
    rule_3   = new MetaRule(Rule({Interval(0, 100)}, { 3 }),
			  Performance(0.5, 0.5, 1.0));
    evolution.define(*rule_1);
    evolution.define(*rule_2);
    evolution.define(*rule_3);
    agent    = new Agent(evolution, *covering, *reward);
  }

  void teardown(void)
  {
    delete agent;
    delete covering;
    delete reward;
  }

};


TEST(OverlappingRulesAgent, test_predict_the_most_relevant_rule)
{
  const Vector& actual = agent->predict(Vector({ 50 }));

  CHECK(Vector({ 4 }) == actual);
}


TEST(OverlappingRulesAgent, test_reward)
{
  const Vector	context({ 50 });

  agent->predict(context);
  agent->reward(10);

  DOUBLES_EQUAL(2.84375, rule_1->weighted_payoff(), 1e-6);
  DOUBLES_EQUAL(2.2475, rule_2->weighted_payoff(), 1e-6);
}



TEST_GROUP(TestAgentEvolution)
{
  MetaRulePool		 pool;
  RewardFunction	*reward;
  Randomizer		 randomizer;
  Decision		*decisions;
  Selection		*selection;
  Crossover		*crossover;
  AlleleMutation	*mutation;
  DefaultEvolution	*evolution;
  EvolutionListener	*listener;
  Covering		*covering;
  Agent			*agent;
  Codec			*codec;

  void setup(void)
  {
    reward    = new WilsonReward(0.25, 500, 2);
    decisions = new RandomDecision(randomizer, 0.25, 0.1);
    selection = new RouletteWheel(randomizer);
    crossover = new TwoPointCrossover(randomizer);
    mutation  = new RandomAlleleMutation(randomizer);
    listener  = new LogListener(cout);
    codec     = new Codec(pool);
    evolution = new DefaultEvolution(pool,
				     *codec,
				     *decisions,
				     *crossover,
				     *selection,
				     *mutation,
				     *listener);
    covering  = new FakeCovering();
    agent     = new Agent(*evolution, *covering, *reward);
  }

  void teardown(void)
  {
    delete codec;
    delete decisions;
    delete selection;
    delete crossover;
    delete mutation;
    delete listener;
    delete evolution;
    delete agent;
    delete covering;
    delete reward;
  }

  static constexpr double MAX_REWARD = 100;
  static constexpr double TOLERANCE = 25;

  double compute_reward(double expected, double actual)
  {
    double error = expected - actual;
    return MAX_REWARD * exp(-pow(error, 2) / (2 * pow(TOLERANCE, 2)));
  }

};


IGNORE_TEST(TestAgentEvolution, test_long_run)
{
  for (int round=0 ; round< 10 ; ++round){
    for (int value=0 ; value < 100 ; ++value) {
      Vector prediction  = agent->predict(Vector({ value }));
      unsigned int actual = static_cast<unsigned int>(prediction[0]);
      double prize = compute_reward(value, actual);
      cout << "X=" << value << " ; P=" << actual << "; R=" << prize << endl;
      agent->reward(prize);
    }
  }
}
