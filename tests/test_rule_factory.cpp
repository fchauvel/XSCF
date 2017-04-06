
#include "CppUTest/TestHarness.h"

#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>


using namespace xcsf;


TEST_GROUP(TestCrossover)
{};



TEST(TestCrossover, simple_test) {
  Rule *rule_1 = new Rule({Interval(0,50)}, { 4 }, 1.0, 1.0, 1.0);
  Rule *rule_2 = new Rule({Interval(50, 100)}, { 2 }, 1.0, 1.0, 1.0);

  Evolution evolution;

  vector<Rule*> children = evolution.breed(*rule_1, *rule_2, 1, 2);

  Rule *expected_child_A = new Rule({Interval(0, 100)}, {4}, 1.0, 1.0, 1.0);
  Rule *expected_child_B = new Rule({Interval(50, 50)}, {2}, 1.0, 1.0, 1.0); 
   
  CHECK(2 == children.size());
  CHECK(*expected_child_A == *children[0]);
  CHECK(*expected_child_B == *children[1]);

  delete rule_1;
  delete rule_2;
  delete expected_child_A;
  delete expected_child_B;
}
