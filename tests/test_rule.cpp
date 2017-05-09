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


TEST_GROUP(TestPerformance)
{
  const double fitness = 12.0;
  const double payoff = 13.0;
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

  CHECK(out.str() == "{ F = 12.00 ; P = 13.00 ; E = 14.00 }" );
}


TEST_GROUP(TestSimpleRule)
{
  const double fitness = 0.75;
  const double payoff = 0.75;
  const double error = 1.0;
  Rule* rule;

  void setup() {
    rule = new Rule({ Interval(0, 50) }, { 50 }, fitness, payoff, error);
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
  const Rule& rule = * new Rule({Interval(20, 30), Interval(10, 15), Interval(80, 99)},
				{ 1 },
				fitness, payoff, error);
  
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
  Rule *rule_1;
  double r1_fitness = 12, r2_fitness = 14;
  double r1_payoff = 2, r2_payoff = 3;
  double r1_error = 0, r2_error = 0;
  Rule *rule_2;

  void setup(void) {
    rule_1 = new Rule({ Interval(0, 25) }, { 12 }, r1_fitness, r1_payoff, r1_error);
    rules.add(*rule_1);

    rule_2 = new Rule({ Interval(25, 50) }, { 37 }, r2_fitness, r2_payoff, r2_error);
    rules.add(*rule_2);
  }


  void teardown(void) {
    delete rule_1;
    delete rule_2;
  }
};


TEST(TestRuleSet, test_reward)
{
  rules.reward(10);

  CHECK(rules[0].fitness() != r1_fitness);
  CHECK(rules[1].fitness() != r2_fitness);

  CHECK(rules[0].payoff() != r2_payoff);
  CHECK(rules[1].payoff() != r2_payoff);

  CHECK(rules[0].error() != r2_error);
  CHECK(rules[1].error() != r2_error);

  
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
  expected << "Rule                    F.   P.   E." << endl
	   << "------------------------------------" << endl
	   << "( 0, 25) => (12)      12.0  2.0  0.0" << endl
	   << "(25, 50) => (37)      14.0  3.0  0.0" << endl
    	   << "------------------------------------" << endl
	   << "2 rule(s)." << endl;
  
  CHECK_EQUAL(expected.str(), out.str());
}
