#include "CppUTest/TestHarness.h"


#include "rule.h"

TEST_GROUP(TestSimpleRule)
{
  const double fitness = 0.75;
  const double payoff = 0.75;
  Rule* rule;

  void setup() {
    rule = new Rule({ Interval(0, 50) }, { 50 }, fitness, payoff);
  }
  
  void teardown() {
    delete rule;
  }

};


TEST(TestSimpleRule, test_weighted_payoff)
{
  DOUBLES_EQUAL(fitness * payoff, rule->weighted_payoff(), 1e-6);  
}


TEST(TestSimpleRule, test_invalid_context){
  Vector context({25, 23, 45});
  CHECK_THROWS(std::invalid_argument, { rule->match(context); });
}


TEST(TestSimpleRule, test_match){
  Vector context({ 25 });
  CHECK(rule->match(context));
}


TEST(TestSimpleRule, test_mismatch){
  Vector context({ 60 });
  CHECK_FALSE(rule->match(context));
}

TEST(TestSimpleRule, test_reward){
  const double reward(10);
  rule->reward(10);

  const double expected = fitness * (payoff + 0.25 * (reward - payoff));
  DOUBLES_EQUAL(expected, rule->weighted_payoff(), 1e-6)
}


TEST_GROUP(Test3DRule)
{
  const double fitness = 0.75;
  const double payoff = 0.75;
  const Rule& rule = * new Rule({Interval(20, 30), Interval(10, 15), Interval(80, 99)},
				{ 1 },
				fitness, payoff);
  
  void teardown() {
    delete &rule;
  }

};


TEST(Test3DRule, test_match_three_value){
  Vector context({ 25, 12, 85 });
  
  CHECK(rule.match(context));
}


TEST(Test3DRule, test_mismatch_three_value){
  Vector context( { 25, 12, 12 });
  
  CHECK_FALSE(rule.match(context));   
}


