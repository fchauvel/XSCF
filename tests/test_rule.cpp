#include "CppUTest/TestHarness.h"


#include "rule.h"


TEST_GROUP(TestRule)
{
};

TEST(TestRule, test_invalid_context){
  vector<int> values = {25, 23, 45};
  Vector context(values);
  Rule rule({Interval(0, 50)}, 1, 0.75);
  
  CHECK_THROWS(std::invalid_argument, {rule.match(context);});
}

TEST(TestRule, test_match_one_value){
  vector<int> values = { 25 };
  Vector context(values);
  Rule rule({Interval(20, 30)}, 1, 0.75);
  
  CHECK(rule.match(context));
}

TEST(TestRule, test_match_three_value){
  vector<int> values = { 25, 12, 85 };
  Vector context(values);
  Rule rule({Interval(20, 30), Interval(10, 15), Interval(80, 99)}, 1, 0.75);
  
  CHECK(rule.match(context));
}

TEST(TestRule, test_mismatch){
  vector<int> values = { 25 };
  Vector context(values);
  Rule rule({Interval(30, 40)}, 1, 0.75);
  
  CHECK_FALSE(rule.match(context));
}

TEST(TestRule, test_mismatch_three_value){
  vector<int> values = { 25, 12, 12 };
  Vector context(values);
  Rule rule({Interval(20, 30), Interval(10, 15), Interval(80, 99)}, 1, 0.75);
  
  CHECK_FALSE(rule.match(context));   
}
