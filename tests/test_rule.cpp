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

#include <sstream>

#include "rule.h"


using namespace xcsf;


TEST_GROUP(TestDimensions)
{};


TEST(TestDimensions, invalid_input)
{
  CHECK_THROWS(std::invalid_argument, { Dimensions(0, 1); });
}

TEST(TestDimensions, invalid_output)
{
  CHECK_THROWS(std::invalid_argument, { Dimensions(1, 0); });
}


TEST_GROUP(TestRule)
{
  Rule *rule_1, *rule_2, *rule_3, *rule_4;
 

  void setup(void)
  {
    rule_1 = new Rule({ Interval(0, 10) }, { 50 });
    rule_2 = new Rule({ Interval(0, 20) }, { 50 });
    rule_3 = new Rule({ Interval(0, 10) }, { 60 });
    rule_4 = new Rule({ Interval(0, 10), Interval(11, 40) }, { 40, 50 });
  }
  
  void teardown()
  {
    delete rule_1;
    delete rule_2;
    delete rule_3;
    delete rule_4;
  }

};

TEST(TestRule, subsumption)
{
  Rule rule({ Interval(5, 9) }, { 50 } );
  CHECK(rule_1->subsumes(rule));
}

TEST(TestRule, subsumption_with_itself)
{
  CHECK(rule_1->subsumes(*rule_1));
}

TEST(TestRule, subsumption_different_output)
{
  Rule rule({ Interval(5, 9) }, { 40 } );
  CHECK(not rule_1->subsumes(rule));
}

TEST(TestRule, subsumption_different_dimensions)
{
  Rule rule({ Interval(5, 9) , Interval(10, 20) }, { 40, 50 } );
  CHECK(not rule_1->subsumes(rule));
}


TEST(TestRule, prediction)
{
  Vector expected = { 50 };
  CHECK((*rule_1)({ 5 }) == expected);
}


TEST(TestRule, prediction_of_invalid_context)
{
  CHECK_THROWS(std::invalid_argument,
	       {
		 (*rule_1)({ 12 });
	       });
}


TEST(TestRule, invalid_premises)
{
  vector<Interval> invalid_premises;

  CHECK_THROWS(std::invalid_argument,
	       {
		 Rule rule(invalid_premises, { 4 });
	       });
}


TEST(TestRule, invalid_conclusion)
{
  vector<unsigned int> invalid_conclusion;

  CHECK_THROWS(std::invalid_argument,
	       {
		 Rule rule( { Interval(0, 10) }, invalid_conclusion);
	       });
}


TEST(TestRule, invalid_activation)
{
  CHECK_THROWS(std::invalid_argument,
	       {
		 rule_1->is_triggered_by({ 1, 2, 3 });
	       });
}

TEST(TestRule, activation)
{
  CHECK(rule_1->is_triggered_by({ 5 }));
  CHECK(rule_4->is_triggered_by({ 5, 15 }));
  
	
  CHECK(not rule_1->is_triggered_by({ 11 }));
  CHECK(not rule_4->is_triggered_by({ 11, 15 }));
  CHECK(not rule_4->is_triggered_by({ 5, 45 })); 
}

TEST(TestRule, test_display_1D_rule)
{
  stringstream out;

  out << *rule_1;

  string expected("([  0,  10]) => ( 50)");
  CHECK_EQUAL(expected, out.str());
}

TEST(TestRule, test_display_2D_rule)
{
  stringstream out;

  out << *rule_4;

  string expected("([  0,  10], [ 11,  40]) => ( 40,  50)");
  CHECK_EQUAL(expected, out.str());
}


TEST(TestRule, test_equality_with_itself)
{
  CHECK(*rule_1 == *rule_1);
}

TEST(TestRule, equality_with_an_equivalent_rule)
{
  Rule rule({ Interval(0, 10) }, { 50 });
  
  CHECK(*rule_1 == rule);
  CHECK(rule == *rule_1);
}

TEST(TestRule, difference_in_premises)
{
  CHECK(*rule_1 != *rule_2);
  CHECK(*rule_2 != *rule_1);
}

TEST(TestRule, difference_in_conclusion)
{
  CHECK(*rule_1 != *rule_3);
  CHECK(*rule_3 != *rule_1);
}

TEST(TestRule, difference_in_dimension)
{
  CHECK(*rule_1 != *rule_4);
  CHECK(*rule_4 != *rule_1);
}



TEST_GROUP(TestPerformance)
{
  const double fitness = 12.0;
  const double payoff = 2313.0;
  const double error = 14.0;
  Performance perf;

  void setup(void)
  {
    perf = Performance(fitness, payoff, error);
  }

  void teardown(void)
  {}
  
};

TEST(TestPerformance, test_equals_itself)
{
  CHECK(perf == perf);
}

TEST(TestPerformance, test_equals_same)
{
  Performance equivalent(fitness, payoff, error);
  CHECK(perf == equivalent);
}

TEST(TestPerformance, test_detect_fitness_mismatch)
{
  Performance different(fitness + 1, payoff, error);

  CHECK(perf != different);
}

TEST(TestPerformance, test_detect_payoff_mismatch)
{
  Performance different(fitness, payoff + 1, error);

  CHECK(perf != different);
}

TEST(TestPerformance, test_detect_error_mismatch)
{
  Performance different(fitness, payoff, error + 1);

  CHECK(perf != different);
}

TEST(TestPerformance, test_copy_constructor)
{
  Performance copy(perf);

  CHECK(perf == copy);
}

TEST(TestPerformance, test_assignment)
{
  Performance copy = perf;

  CHECK(copy == perf);
};

TEST(TestPerformance, test_formatting)
{
  stringstream out;
  out << perf;

  CHECK_EQUAL("{ F = 12.00 ; P = 2313.00 ; E = 14.00 }", out.str());
}


TEST_GROUP(TestSimpleRule)
{
  const double fitness = 0.75;
  const double payoff = 0.75;
  const double error = 1.0;
  MetaRule* rule;

  void setup() {
    rule = new MetaRule(Rule({ Interval(0, 50) }, { 50 }),
			Performance(fitness, payoff, error));
  }
  
  void teardown() {
    delete rule;
  }

};


TEST(TestSimpleRule, test_weighted_payoff)
{
  DOUBLES_EQUAL(fitness * payoff, rule->weighted_payoff(), 1e-6);  
}


TEST(TestSimpleRule, test_invalid_context)
{
  Vector context({25, 23, 45});
  CHECK_THROWS(std::invalid_argument, { rule->match(context); });
}


TEST(TestSimpleRule, test_match)
{
  Vector context({ 25 });
  CHECK(rule->match(context));
}


TEST(TestSimpleRule, test_mismatch)
{
  Vector context({ 60 });
  CHECK_FALSE(rule->match(context));
}


TEST(TestSimpleRule, test_as_vector)
{
  vector<unsigned int> expected({ 0, 50, 50 });
  vector<unsigned int> actual = rule->as_vector();

  CHECK(expected == actual);
}


TEST_GROUP(Test3DRule)
{
  const double fitness = 0.75;
  const double payoff = 0.75;
  const double error = 0.1;
  const MetaRule& rule = *new MetaRule(Rule({Interval(20, 30), Interval(10, 15), Interval(80, 99)},
					    { 1 }),
				       Performance(fitness, payoff, error));
  
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

TEST(Test3DRule, test_as_vector)
{
  vector<unsigned int> expected({ 20, 30, 10, 15, 80, 99, 1 });
  vector<unsigned int> actual = rule.as_vector();

  CHECK(expected == actual);
}



TEST_GROUP(TestRuleSet)
{
  RuleSet rules;
  MetaRule *rule_1;
  double r1_fitness = 12, r2_fitness = 14;
  double r1_payoff = 2, r2_payoff = 3;
  double r1_error = 0, r2_error = 0;
  MetaRule *rule_2;

  void setup(void) {
    rule_1 = new MetaRule(Rule({ Interval(0, 25) }, { 12 }),
			  Performance(r1_fitness, r1_payoff, r1_error));
    rules.add(*rule_1);

    rule_2 = new MetaRule(Rule({ Interval(25, 50) }, { 37 }),
			  Performance(r2_fitness, r2_payoff, r2_error));
    rules.add(*rule_2);
  }


  void teardown(void) {
    delete rule_1;
    delete rule_2;
  }
};

TEST(TestRuleSet, test_remove_last_rule_added)
{
  MetaRule& deleted = rules.remove(1);
  CHECK_EQUAL(1, rules.size());
  CHECK(rules[0] == *rule_1);
}


TEST(TestRuleSet, test_worst)
{
  unsigned int worst_rule = rules.worst();

  CHECK(rule_1 == &(rules[worst_rule]));
}


TEST(TestRuleSet, test_total_fitness)
{
  double total_fitness = rules.total_fitness();
  DOUBLES_EQUAL(r1_fitness + r2_fitness, total_fitness, 1e-6);
}

TEST(TestRuleSet, test_total_weighted_payoff)
{
  double expected = r1_fitness * r1_payoff + r2_fitness * r2_payoff;
  double actual = rules.total_weighted_payoff();

  DOUBLES_EQUAL(expected, actual, 1e-6);
}


TEST(TestRuleSet, test_rule_printer)
{
  stringstream out;
  Formatter formatter(out);
  rules.accept(formatter);

  stringstream expected;
  expected << "    F.    P.    E.   Rule" << endl
	   << "---------------------------------------------" << endl
	   << "  12.0   2.0   0.0   ([  0,  25]) => ( 12)" << endl
	   << "  14.0   3.0   0.0   ([ 25,  50]) => ( 37)" << endl
    	   << "---------------------------------------------" << endl
	   << "2 rule(s)." << endl;
  
  CHECK_EQUAL(expected.str(), out.str());
}



TEST_GROUP(TestMetaRulePool)
{
  MetaRulePool pool;
};


TEST(TestMetaRulePool, test_automated_deallocation)
{
  pool.acquire(Rule({Interval(10, 20)}, { 35 }), Performance(0, 0, 0));

  CHECK_EQUAL(1, pool.active_rule_count());
}


TEST(TestMetaRulePool, test_automated_releasing)
{
  MetaRule *rule = pool.acquire(Rule({Interval(10, 20)}, { 35 }), Performance(0, 0, 0));

  pool.release(rule);

  CHECK_EQUAL(0, pool.active_rule_count());
  CHECK_EQUAL(1, pool.free_rule_count());
}


TEST(TestMetaRulePool, test_reusing_rules)
{
  MetaRule *rule = pool.acquire(Rule({Interval(10, 20)}, { 35 }), Performance(0, 0, 0));
  pool.release(rule);
  
  rule = pool.acquire(Rule({Interval(10, 20)}, { 45 }), Performance(0, 0, 0));

  CHECK_EQUAL(1, pool.active_rule_count());
  CHECK_EQUAL(0, pool.free_rule_count());
}


TEST(TestMetaRulePool, test_releasing_a_foreign_rule)
{
  MetaRule *foreign_rule = new MetaRule(Rule({Interval(10, 20)}, { 35 }), Performance(0, 0, 0));

  CHECK_THROWS(std::invalid_argument,
	       {
		 pool.release(foreign_rule);
	       });

  delete foreign_rule;
}

TEST(TestMetaRulePool, test_releasing_twice_the_same_rule)
{
  MetaRule *rule = pool.acquire(Rule({Interval(10, 20)}, { 35 }), Performance(0, 0, 0));
  pool.release(rule);
  pool.release(rule);

  CHECK_EQUAL(0, pool.active_rule_count());
  CHECK_EQUAL(1, pool.free_rule_count());
}


