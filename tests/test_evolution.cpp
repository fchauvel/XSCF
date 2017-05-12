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
  const double evolution = 0.5;
  const double mutation = 0.2;
  TestableRandomizer* randomizer;
  RandomDecision *decision;
  
  void setup(void)
  {
    randomizer = new TestableRandomizer({ 0 });
    decision = new RandomDecision(*randomizer, evolution, mutation);
  }

  void teardown(void)
  {
    delete randomizer;
    delete decision;
  }

};


TEST(TestRandomDecision, test_shall_evolve)
{
  randomizer->sequence({ evolution-0.1, evolution });
  CHECK(decision->shall_evolve());
  CHECK(decision->shall_evolve());
}


TEST(TestRandomDecision, test_shall_not_evolve)
{
  randomizer->sequence({ evolution+0.1 });
  CHECK(not decision->shall_evolve());
}


TEST(TestRandomDecision, test_shall_mutate)
{
  randomizer->sequence({ mutation-0.1, mutation });
  CHECK(decision->shall_mutate());
  CHECK(decision->shall_mutate());
}


TEST(TestRandomDecision, test_shall_not_mutate)
{
  randomizer->sequence({ mutation+0.1 });
  CHECK(not decision->shall_mutate());
}


class FakeCrossover: public Crossover
{
public:
  explicit FakeCrossover(const Chromosome& child)
    :_child(child)
  {};

  virtual ~FakeCrossover()
  {};

  virtual unsigned int
  children_count(void) const
  {
    return 1;
  };
  
  virtual void
  operator ()(const Chromosome& father, const Chromosome& mother, vector<Chromosome>& children) const
  {
    children.push_back(_child);
  };
  
private:
  const Chromosome& _child;

};


class FakeAlleleMutation: public AlleleMutation
{
public:
  FakeAlleleMutation(const Allele& mutated):_mutated(mutated){};
  virtual ~FakeAlleleMutation(){};
  
  virtual void operator () (Chromosome& subject, const Allele& target) const
  {
    subject[target] = _mutated;
  };

private:
  const Allele& _mutated;
  
};


class FakeListener: public EvolutionListener
{
public:
  FakeListener()
  {};

  virtual ~FakeListener()
  {};

  virtual void on_rule_added(const MetaRule& rule) const
  {
    mock().actualCall("on_rule_added");
  };

  virtual void on_rule_deleted(const MetaRule& rule) const
  {
    mock().actualCall("on_rule_deleted");
  };

  virtual void on_breeding(const MetaRule& father, const MetaRule& mother) const
  {
    mock().actualCall("on_breeding");
  };

  virtual void on_mutation(const Chromosome& subject, const Allele& locus) const
  {
    mock().actualCall("on_mutation");
  };    
  
  
};


TEST_GROUP(TestEvolution)
{
  Chromosome child = { 5, 10, 20 };
  MetaRule *rule_1, *rule_2;
  RuleSet *rules;
  AlleleMutation *mutations;
  Crossover *crossover;
  Selection *selection;
  EvolutionListener *listener;
  
  void setup(void)
  {
    crossover = new FakeCrossover(child);
    rule_1 = new MetaRule(Rule({Interval(0, 50)}, { 4 }), Performance(1.0, 1.0, 1.0));
    rule_2 = new MetaRule(Rule({Interval(50, 100)}, { 2 }), Performance(1.0, 1.0, 1.0));
    rules = new RuleSet(Dimensions(1, 1), 10);
    rules->add(*rule_1);
    rules->add(*rule_2);
    selection = new DummySelection();
    mutations = new FakeAlleleMutation(77);
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
  mock().expectNCalls(0, "on_rule_added");
 
  RuleSet before_evolution(*rules);
  FixedDecision decision(NO_EVOLUTION, NO_MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);
  
  evolution.evolve(*rules);
  
  CHECK(*rules == before_evolution);
  mock().checkExpectations();
}


TEST(TestEvolution, test_evolution_without_mutation)
{
  mock().expectOneCall("on_rule_added");
  mock().expectOneCall("on_breeding");
    
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, NO_MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  CHECK_EQUAL(before_evolution.size() + 1, rules->size());

  vector<unsigned int> expected_new_rule({ 5, 10, 20 });
  CHECK(expected_new_rule == (*rules)[2].as_vector());

  mock().checkExpectations();
}


TEST(TestEvolution, test_evolution_with_mutation)
{
  mock().expectOneCall("on_rule_added");
  mock().expectOneCall("on_breeding");
  mock().expectNCalls(3, "on_mutation");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  CHECK_EQUAL(before_evolution.size() + 1, rules->size());

  vector<unsigned int> expected_new_rule({ 77, 77, 77 });
  CHECK(expected_new_rule == (*rules)[2].as_vector());


  mock().checkExpectations();
}


TEST(TestEvolution, test_creating_rules_for_unknown_contexts)
{
  mock().expectOneCall("on_rule_added");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  Vector context = { 80 };
  evolution.create_rule_for(*rules, context);

  CHECK_EQUAL(before_evolution.size()+1, rules->size());
  mock().checkExpectations();
}


TEST(TestEvolution, test_listening)
{
  mock().expectOneCall("on_rule_added");
  mock().expectOneCall("on_breeding");
  mock().expectNCalls(3, "on_mutation");
   
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  mock().checkExpectations();
}


TEST_GROUP(TestEvolutionAtCapacity)
{
  unsigned int capacity = 3;
  MetaRulePool pool;
  Chromosome child = { 5, 10, 20 };
  MetaRule *rule_1, *rule_2, *rule_3;
  RuleSet *rules;
  AlleleMutation *mutations;
  Crossover *crossover;
  Selection *selection;
  EvolutionListener *listener;
  
  void setup(void)
  {
    rules = new RuleSet(Dimensions(1, 1), capacity);
    
    rule_1 = pool.acquire(Rule({Interval(0, 50)}, { 4 }));
    rules->add(*rule_1);

    rule_2 = pool.acquire(Rule({Interval(0, 50)}, { 5 }));
    rules->add(*rule_2);

    rule_3 = pool.acquire(Rule({Interval(0, 50)}, { 6 }));
    rules->add(*rule_3);
          
    crossover = new FakeCrossover(child);
    selection = new DummySelection();
    mutations = new FakeAlleleMutation(77);
    listener = new FakeListener();
  }

  void teardown(void)
  {
    delete rules;
    delete crossover;
    delete selection;
    delete mutations;
    delete listener;
    mock().clear();
  }
  
};


TEST(TestEvolutionAtCapacity, test_creating_rules_for_unknown_contexts)
{
  mock().expectOneCall("on_rule_added");
  mock().expectOneCall("on_rule_deleted");
  
  RuleSet before_evolution(*rules);
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  Vector context = { 80 };
  evolution.create_rule_for(*rules, context);

  CHECK_EQUAL(before_evolution.size(), rules->size());
  mock().checkExpectations();

  CHECK(&(*rules)[0] != rule_1 or &(*rules)[1] != rule_2);
}


TEST(TestEvolutionAtCapacity, test_evolution)
{
  mock().expectOneCall("on_rule_added");
  mock().expectOneCall("on_breeding");
  mock().expectOneCall("on_rule_deleted");
  mock().expectNCalls(3, "on_mutation");
 
  FixedDecision decision(EVOLUTION, MUTATION);
  Evolution evolution(decision, *crossover, *selection, *mutations, *listener);

  evolution.evolve(*rules);

  CHECK_EQUAL(rules->capacity(), rules->size());

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
  MetaRule rule(Rule({Interval(0, 100)}, { 50 }), Performance(1., 1., 1.));
  
  listener->on_rule_added(rule);

  stringstream expected;
  expected << "New rule '" << rule << "'" << endl;

  CHECK_EQUAL(expected.str(), out.str());
}

TEST(TestLogListener, test_on_rule_deleted)
{
  MetaRule rule(Rule({Interval(0, 100)}, { 50 }), Performance(1., 1., 1.));
  
  listener->on_rule_deleted(rule);

  stringstream expected;
  expected << "Deleted rule '" << rule << "'" << endl;

  CHECK_EQUAL(expected.str(), out.str());
}

TEST(TestLogListener, test_on_breeding)
{
  MetaRule father(Rule({Interval(0, 50)}, { 50 }), Performance(1., 1., 1.));
  MetaRule mother(Rule({Interval(50, 75)}, { 67 }), Performance(1., 1., 1.));
  
  listener->on_breeding(father, mother);

  stringstream expected;
  expected << "Breeding:" << endl
	   << " - Father: " << father << endl
	   << " - Mother: " << mother << endl;

  CHECK_EQUAL(expected.str(), out.str());
}


TEST(TestLogListener, test_on_mutation)
{
  Chromosome individual({ 10, 20, 30, 40 });
  Allele locus(2);
  
  listener->on_mutation(individual, locus); 

  stringstream expected;
  expected << "Mutation of " << individual << " at " << locus << endl;

  CHECK_EQUAL(expected.str(), out.str());
}
