
#include "CppUTest/TestHarness.h"


#include "agent.h"
#include "rule.h"

#include "helpers.h"



TEST_GROUP(TestAgent)
{
  TestRuleFactory factory;
  
};


TEST(TestAgent, test_predict_the_single_active_rule)
{
  vector<int> predictions = {4};
  factory.define(Rule({Interval(0, 100)}, predictions, 0.75));

  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({1}));

  CHECK(Vector(predictions) == actual);
}


TEST(TestAgent, test_predict_the_fittest_active_rule)
{
  factory.define(Rule({Interval(0, 100)}, { 3 }, 0.5));

  vector<int> predictions = { 4 };
  factory.define(Rule({Interval(0, 100)}, predictions, 0.75));
  
  Agent agent(factory);
  const Vector& actual = agent.select_action(Vector({50}));
  
  CHECK(Vector(predictions) == actual);
}


