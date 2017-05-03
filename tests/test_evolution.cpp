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
#include "CppUTestExt/MockSupport.h"

#include "helpers.h"
#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>



using namespace xcsf;



TEST_GROUP(TestRandomDecision)
{
  TestableRandomizer* randomizer;
  RandomDecision *decision;
  
  void setup(void)
  {
    randomizer = new TestableRandomizer({ 0 });
    decision = new RandomDecision(*randomizer, 0, 0);
  }

  void teardown(void)
  {
    delete randomizer;
    delete decision;
  }

};


TEST(TestRandomDecision, test_shall_evolve)
{
  randomizer->sequence({ 1.0 });
  CHECK(decision->shall_evolve());
}


TEST(TestRandomDecision, test_shall_not_evolve)
{
  randomizer->sequence({ 0.0 });
  CHECK(not decision->shall_evolve());
}


class FakeCrossover: public Crossover
{
public:
  explicit FakeCrossover(const Chromosome& child)
    :_child(child)
  {};

  virtual ~FakeCrossover()
  {};
  
  virtual void
  operator ()(const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const
  {
    children[0] = _child;
    children.erase(children.begin()+1);
  };
  
private:
  const Chromosome& _child;

};


class FakeAlleleMutation: public AlleleMutation
{
public:
  FakeAlleleMutation(){};
  virtual ~FakeAlleleMutation(){};
  
  virtual void operator () (Chromosome& subject, const Allele& target) const
  {
    subject[target] = 50;
  };
  
};


class FakeListener: public EvolutionListener
{
public:
  FakeListener()
  {};

  virtual ~FakeListener()
  {};

  virtual void on_rule_added(const Rule& rule) const
  {
    mock().actualCall("on_rule_added");
  };
  
};


TEST_GROUP(TestEvolution)
{
  Chromosome child = { 5, 10, 20 };
  Rule *rule_1, *rule_2;
  RuleSet *rules;
  AlleleMutation *mutations;
  Crossover *crossover;
  Selection *selection;
  EvolutionListener *listener;
  
  void setup(void)
  {
    crossover = new FakeCrossover(child);
    rule_1 = new Rule({Interval(0, 50)}, { 4 }, 1.0, 1.0, 1.0);
    rule_2 = new Rule({Interval(50, 100)}, { 2 }, 1.0, 1.0, 1.0);
    rules = new RuleSet();
    rules->add(*rule_1);
    rules->add(*rule_2);
    selection = new DummySelection();
    mutations = new FakeAlleleMutation();
    listener = new FakeListener();
  }

  void teardown(void)
  {
    delete rules;
    delete crossover;
    delete rule_1;
    delete rule_2;
    delete selection;
    delete mutations;
    delete listener;
    mock().clear();
  }
  
};


TEST(TestEvolution, test_no_evolution)
{
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(NO_EVOLUTION, NO_MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);
  
  evolution.evolve(*rules);
  
  CHECK(*rules == before_evolution);
}


TEST(TestEvolution, test_evolution_without_mutation)
{
  mock().expectOneCall("on_rule_added");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, NO_MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  CHECK_EQUAL(before_evolution.size() + 1, rules->size());

  Rule expected_new_rule({ Interval(5, 10) }, { 20 }, 1., 1., 1.);
  CHECK(expected_new_rule == (*rules)[2]);
}


TEST(TestEvolution, test_evolution_with_mutation)
{
  mock().expectOneCall("on_rule_added");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  CHECK_EQUAL(before_evolution.size() + 1, rules->size());

  Rule expected_new_rule({ Interval(50, 50) }, { 50 }, 1., 1., 1.);
  CHECK(expected_new_rule == (*rules)[2]);
}


TEST(TestEvolution, test_evolution_with_population_at_capacity)
{
  mock().expectOneCall("on_rule_added");

  const unsigned int CAPACITY = 2;
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener, 1, 1, CAPACITY);

  evolution.evolve(*rules);

  CHECK_EQUAL(CAPACITY, rules->size());
}


TEST(TestEvolution, test_creating_rules_for_unknown_contexts)
{
  mock().expectOneCall("on_rule_added");

  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener, 1, 1);

  Vector context = { 80 };
  evolution.create_rule_for(*rules, context);

  CHECK_EQUAL(before_evolution.size()+1, rules->size());
}


TEST(TestEvolution, test_creating_rules_for_unknown_contexts_at_capacity)
{
  mock().expectOneCall("on_rule_added");

  const unsigned int CAPACITY = 2;
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener, 1, 1, CAPACITY);

  Vector context = { 80 };
  evolution.create_rule_for(*rules, context);

  CHECK_EQUAL(before_evolution.size(), rules->size());
}


TEST(TestEvolution, test_listening)
{
  mock().expectOneCall("on_rule_added");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener, 1, 1, 100);

  evolution.evolve(*rules);

  mock().checkExpectations();
  
}


TEST_GROUP(TestLogListener)
{
  stringstream out;
  EvolutionListener *listener;
  
  void setup(void)
  {
    listener = new LogListener(out);
  }

  void teardown(void)
  {
    delete listener;
  }
  
};


TEST(TestLogListener, test_on_rule_added)
{
  Rule rule({Interval(0, 100)}, { 50 }, 1., 1., 1.);
  
  listener->on_rule_added(rule);

  stringstream expected;
  expected << "New rule '" << rule << "'" << endl;

  CHECK_EQUAL(expected.str(), out.str());
}
