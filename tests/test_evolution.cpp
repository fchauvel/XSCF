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

#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>


using namespace xcsf;

const bool NO_EVOLUTION = false;

class FixedDecision: public Decision
{
public:
  FixedDecision(bool evolution)
    :Decision()
    ,_evolution(evolution)
  {}

  virtual ~FixedDecision() {}

  virtual bool shall_evolve(void) const {
    return _evolution;
  };

private:
  bool _evolution;
  
};


TEST_GROUP(TestEvolution)
{
  RuleSet *rules;
  Crossover *crossover;

  void setup(void)
  {
    rules = new RuleSet();
    crossover = new Crossover(1, 2);
  }

  void teardown(void)
  {
    delete rules;
    delete crossover;
  }
  
};


TEST(TestEvolution, test_no_evolution)
{
  RuleSet before_evolution(*rules);
  FixedDecision decision(NO_EVOLUTION);
  
  Evolution evolution(decision, *crossover);
  evolution.evolve(*rules);
  
  CHECK(*rules == before_evolution);
}


TEST_GROUP(TestMutation)
{
  Chromosome *chromosome;

  void setup(void)
  {
    chromosome = new Chromosome({50, 50, 50});
  }


  void teardown(void)
  {
    delete chromosome;
  }
  
};


TEST(TestMutation, simple_mutation)
{
  Mutation mutation(1, 10);

  mutation.apply_to(*chromosome);

  Chromosome expected = { 50, 60, 50 }; 
  CHECK(*chromosome == expected);
}


TEST(TestMutation, test_excessive_positive_mutation)
{
  Mutation mutation(1, 200);

  mutation.apply_to(*chromosome);

  Chromosome expected = { 50, 100, 50 }; 
  CHECK(*chromosome == expected);
}


TEST(TestMutation, test_excessive_negative_mutation)
{
  Mutation mutation(1, -200);

  mutation.apply_to(*chromosome);

  Chromosome expected = { 50, 0, 50 }; 
  CHECK(*chromosome == expected);
}


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
  FixedDecision decision(NO_EVOLUTION);
  Crossover crossover(1, 2);
  Evolution evolution(decision, crossover);

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
  FixedDecision decision(NO_EVOLUTION);
  Crossover crossover(2, 8);
  Evolution evolution(decision, crossover);

  CHECK_THROWS(invalid_argument,{ evolution.breed(*rule_1, *rule_2); });
}


TEST_GROUP(TestCrossoverWithInvalidRules)
{
  Rule *rule_1, *rule_2;

  void setup(void)
  {
    rule_1 = new Rule({Interval(0,50)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(50, 100), Interval(75, 100)}, { 2 }, 1.0, 1.0, 1.0);
  }

  void teardown(void)
  {
    delete rule_1;
    delete rule_2;
  }
};


TEST(TestCrossoverWithInvalidRules, test)
{
  FixedDecision decision(NO_EVOLUTION);
  Crossover crossover(1, 2);
  Evolution evolution(decision, crossover);

  CHECK_THROWS(invalid_argument,{ evolution.breed(*rule_1, *rule_2); });
}


TEST_GROUP(TestCrossoverOperators)
{};


TEST(TestCrossoverOperators, test_equals)
{
  Crossover c1(1, 2);

  CHECK(c1 == c1);
}


TEST(TestCrossoverOperators, test_assignment)
{
  Crossover c1(1, 2);
  Crossover c2(2, 3);

  c1 = c2;
  CHECK(c1 == c2);
}
