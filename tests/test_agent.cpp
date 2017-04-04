
#include "CppUTest/TestHarness.h"


#include "agent.h"
#include "rule.h"

#include "helpers.h"


TEST_GROUP(OneRuleAgent)
{
  TestRuleFactory factory;
  Agent* agent;
  vector<int> predictions = { 4 };
  Rule *rule;

  void setup(void)
  {
    rule = new Rule({Interval(0, 100)}, predictions, 1.0, 1.0, 1.0);
    factory.define(*rule);
    agent = new Agent(factory);
  }

  void teardown(void)
  {
    delete agent;
  }

};


TEST(OneRuleAgent, test_predict_the_single_active_rule)
{
  const Vector& actual = agent->select_action(Vector({ 1 }));
  
  CHECK(Vector(predictions) == actual);
}


TEST(OneRuleAgent, test_reward)
{
  agent->select_action(Vector({ 1 }));
  agent->reward(10);
  
  DOUBLES_EQUAL(3.25, rule->weighted_payoff(), 1e-6);
}



TEST_GROUP(TwoRulesAgent)
{
  TestRuleFactory factory;
  Agent* agent;
  Rule *rule_1, *rule_2;

  void setup(void)
  {
    rule_1 = new Rule({Interval(0, 49)}, { 4 }, 1.0, 1.0, 1.0);
    factory.define(*rule_1);
    
    rule_2 = new Rule({Interval(40, 100)}, { 3 }, 1.0, 1.0, 1.0);
    factory.define(*rule_2);
    
    agent = new Agent(factory);
  }

  void teardown(void)
  {
    delete agent;
  }

};


TEST(TwoRulesAgent, test_predict_active_rule)
{
  const Vector& actual = agent->select_action(Vector({ 25 }));

  CHECK(Vector({ 4 }) == actual);
}



TEST_GROUP(OverlappingRulesAgent)
{
  TestRuleFactory factory;
  Agent *agent;
  Rule *rule_1, *rule_2, *rule_3;

  void setup(void)
  {
    rule_1 = new Rule({Interval(0, 100)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(0, 100)}, { 4 }, 0.8, 0.8, 1.0);
    rule_3 = new Rule({Interval(0, 100)}, { 3 }, 0.5, 0.5, 1.0);
    factory.define(*rule_1);
    factory.define(*rule_2);
    factory.define(*rule_3);
    agent = new Agent(factory);
  }

  void teardown(void)
  {
    delete agent;
  }

};


TEST(OverlappingRulesAgent, test_predict_the_most_relevant_rule)
{
  const Vector& actual = agent->select_action(Vector({ 50 }));
  
  CHECK(Vector({ 4 }) == actual);
}


TEST(OverlappingRulesAgent, test_reward)
{ 
  agent->select_action(Vector({ 50 }));
  agent->reward(10);
  
  DOUBLES_EQUAL(2.84375, rule_1->weighted_payoff(), 1e-6);
  DOUBLES_EQUAL(2.2475, rule_2->weighted_payoff(), 1e-6);
}









