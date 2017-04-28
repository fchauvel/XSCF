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

#include "helpers.h"
#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>


using namespace xcsf;


TEST_GROUP(TestCrossover)
{
  Randomizer* randomizer;
  AlleleMutation* mutations;
  Rule *rule_1, *rule_2;

  void setup(void)
  {
    randomizer = new TestableRandomizer(0);
    mutations = new RandomAlleleMutation(*randomizer);
    rule_1 = new Rule({Interval(0,50)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(50, 100)}, { 2 }, 1.0, 1.0, 1.0);
  }

  void teardown(void)
  {
    delete mutations;
    delete randomizer;
    delete rule_1;
    delete rule_2;
  }
  
};


TEST(TestCrossover, simple_test)
{
  FixedDecision decision(NO_EVOLUTION, NO_MUTATION);
  TwoPointCrossover crossover(1, 2);
  DummySelection selection;
  Evolution evolution(decision, crossover, selection, *mutations);

  vector<Rule*> children = evolution.breed(*rule_1, *rule_2);

  Rule expected_child_A({Interval(0, 100)}, {4}, 1.0, 1.0, 1.0);
  Rule expected_child_B({Interval(50, 50)}, {2}, 1.0, 1.0, 1.0); 
   
  CHECK(2 == children.size());
  CHECK(expected_child_A == *children[0]);
  CHECK(expected_child_B == *children[1]);
}


TEST(TestCrossover, test_inverted_cut_points) {
  CHECK_THROWS(invalid_argument, {   TwoPointCrossover crossover(2, 1); });
}


TEST(TestCrossover, test_invalid_cut_points) {
  FixedDecision decision(NO_EVOLUTION, NO_MUTATION);
  TwoPointCrossover crossover(2, 8);
  DummySelection selection;
  Evolution evolution(decision, crossover, selection, *mutations);

  CHECK_THROWS(invalid_argument,{ evolution.breed(*rule_1, *rule_2); });
}


TEST_GROUP(TestCrossoverWithInvalidRules)
{
  Randomizer* randomizer;
  AlleleMutation* mutations;
  Rule *rule_1, *rule_2;

  void setup(void)
  {
    randomizer = new TestableRandomizer(0);
    mutations = new RandomAlleleMutation(*randomizer);
    rule_1 = new Rule({Interval(0,50)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(50, 100), Interval(75, 100)}, { 2 }, 1.0, 1.0, 1.0);
  }

  void teardown(void)
  {
    delete rule_1;
    delete rule_2;
    delete mutations;
    delete randomizer;
  }
  
};


TEST(TestCrossoverWithInvalidRules, test)
{
  FixedDecision decision(NO_EVOLUTION, NO_MUTATION);
  TwoPointCrossover crossover(1, 2);
  DummySelection selection; 
  Evolution evolution(decision, crossover, selection, *mutations);

  CHECK_THROWS(invalid_argument,{ evolution.breed(*rule_1, *rule_2); });
}


TEST_GROUP(TestCrossoverOperators)
{};


TEST(TestCrossoverOperators, test_equals)
{
  TwoPointCrossover c1(1, 2);

  CHECK(c1 == c1);
}


TEST(TestCrossoverOperators, test_assignment)
{
  TwoPointCrossover c1(1, 2);
  TwoPointCrossover c2(2, 3);

  c1 = c2;
  CHECK(c1 == c2);
}
