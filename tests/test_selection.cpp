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

#include "selection.h"
#include "helpers.h"



TEST_GROUP(TestRouletteWheel)
{
  Randomizer *randomizer;
  MetaRule *rule_1, *rule_2, *rule_3;
  RuleSet *rules;

  void setup(void) {
    randomizer = new TestableRandomizer({ 1.0 });

    rules = new RuleSet();
    
    rule_1 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(1.0, 1.0, 1.0));
    rules->add(*rule_1);
    
    rule_2 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(2.0, 3.0, 1.0));
    rules->add(*rule_2);
    
    rule_3 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(3.0, 1.0, 1.0));
    rules->add(*rule_3);
  }

  void teardown(void) {
    delete randomizer;
    delete rule_1;
    delete rule_2;
    delete rule_3;
    delete rules;
  }

};


TEST(TestRouletteWheel, test_empty_rule_set)
{
  RouletteWheel selection(*randomizer);

  RuleSet empty;
  
  CHECK_THROWS(std::invalid_argument, {selection(empty);});
}


TEST(TestRouletteWheel, test_one_rule_rule_set)
{
  RouletteWheel selection(*randomizer);

  RuleSet one_rule;
  MetaRule rule(Rule( { Interval(23, 45) }, { 29 }), Performance(0, 0, 0));
  one_rule.add(rule);
  
  CHECK_THROWS(std::invalid_argument, {selection(one_rule);});
}


TEST(TestRouletteWheel, test_simple)
{
  RouletteWheel selection(*randomizer);

  vector<MetaRule*> selected_rules = selection(*rules);

  CHECK_EQUAL(2, selected_rules.size());
  CHECK(selected_rules[0] == rule_3);
  CHECK(selected_rules[1] == rule_2);
}


TEST_GROUP(TestZeroFitness)
{
  Randomizer *randomizer;
  MetaRule *rule_1, *rule_2, *rule_3;
  RuleSet *rules;

  void setup(void) {
    randomizer = new TestableRandomizer({ 1.0 });

    rules = new RuleSet();
    
    rule_1 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(0.0, 1.0, 1.0));
    rules->add(*rule_1);
    
    rule_2 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(0.0, 1.0, 1.0));
    rules->add(*rule_2);
    
    rule_3 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(0.0, 1.0, 1.0));
    rules->add(*rule_3);
  }

  void teardown(void) {
    delete randomizer;
    delete rule_1;
    delete rule_2;
    delete rule_3;
    delete rules;
  }

};


TEST(TestZeroFitness, test_simple)
{
  RouletteWheel selection(*randomizer);

  vector<MetaRule*> selected_rules = selection(*rules);

  CHECK_EQUAL(2, selected_rules.size());
  CHECK(selected_rules[0] == rule_1);
  CHECK(selected_rules[1] == rule_2);
}
