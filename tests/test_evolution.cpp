
#include "CppUTest/TestHarness.h"

#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>


using namespace xcsf;


TEST_GROUP(TestCrossover)
{
  Rule *rule_1, *rule_2;

  void setup(void)
  {
    rule_1 = new Rule({Interval(0,50)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(50, 100)}, { 2 }, 1.0, 1.0, 1.0);
  }

  void teardown(void)
  {
    delete rule_1;
    delete rule_2;
  }
  
};



TEST(TestCrossover, simple_test)
{  
  Crossover crossover(1, 2);
  Evolution evolution(crossover);

  vector<Rule*> children = evolution.breed(*rule_1, *rule_2);

  Rule expected_child_A({Interval(0, 100)}, {4}, 1.0, 1.0, 1.0);
  Rule expected_child_B({Interval(50, 50)}, {2}, 1.0, 1.0, 1.0); 
   
  CHECK(2 == children.size());
  CHECK(expected_child_A == *children[0]);
  CHECK(expected_child_B == *children[1]);
}


TEST(TestCrossover, test_inverted_cut_points) {
  CHECK_THROWS(invalid_argument, {   Crossover crossover(2, 1); });
}


TEST(TestCrossover, test_invalid_cut_points) {
  Crossover crossover(2, 8);
  Evolution evolution(crossover);

  CHECK_THROWS(invalid_argument,{ evolution.breed(*rule_1, *rule_2); });
}
