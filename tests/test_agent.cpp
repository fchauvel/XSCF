
#include "CppUTest/TestHarness.h"


#include "agent.h"
#include "rule.h"

#include "helpers.h"


TEST_GROUP(SingleRuleAgent)
{
  TestRuleFactory factory;
  Agent* agent;
  vector<int> predictions = { 4 };
  double fitness = 0.75;
  double payoff = 0.5;
  Rule rule = Rule({Interval(0, 100)}, predictions, fitness, payoff);

  void setup(void)
  {
    factory.define(rule);
    agent = new Agent(factory);
  }

  void teardown(void)
  {
    delete agent;
  }

};


TEST(SingleRuleAgent, test_predict_the_single_active_rule)
{
  factory.define(Rule({Interval(0, 100)}, predictions, 0.75, 0.5));

  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({ 1 }));

  CHECK(Vector(predictions) == actual);
}


TEST(SingleRuleAgent, test_reward)
{
  agent->select_action({ 1 });
  agent->reward(10);

  DOUBLES_EQUAL(0.375, rule.weighted_payoff(), 1e-6);
}
  



TEST_GROUP(TestAgent)
{
  TestRuleFactory factory;
  
};


TEST(TestAgent, test_predict_the_single_active_rule)
{
  vector<int> predictions = {4};
  factory.define(Rule({Interval(0, 100)}, predictions, 0.75, 0.5));

  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({1}));

  CHECK(Vector(predictions) == actual);
}


TEST(TestAgent, test_predict_the_fittest_active_rule)
{
  factory.define(Rule({Interval(0, 100)}, { 3 }, 0.5, 0.5));

  vector<int> predictions = { 4 };
  factory.define(Rule({Interval(0, 100)}, predictions, 0.75, 0.6));
  
  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({50}));
  
  CHECK(Vector(predictions) == actual);
}

TEST(TestAgent, test_predict_the_most_interesting)
{
  vector<int> predictions = { 4 };
  factory.define(Rule({Interval(0, 100)}, predictions, 0.25, 0.6));
  factory.define(Rule({Interval(10, 90)}, predictions, 0.25, 0.9));
  factory.define(Rule({Interval(0, 100)}, { 3 }, 0.3, 0.2));
  
  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({50}));
  
  CHECK(Vector(predictions) == actual);
}

TEST(TestAgent, test_predict_only_active_rule)
{
  vector<int> expected_prediction = { 4 };
  factory.define(Rule({Interval(0, 50)}, expected_prediction, 0.25, 0.25));
  factory.define(Rule({Interval(50, 100)}, { 3 }, 0.90, 0.90));

  Agent agent(factory);
  const Vector& actual_prediction = agent.select_action(Vector({ 25 }));

  CHECK(Vector(expected_prediction) == actual_prediction);
}
