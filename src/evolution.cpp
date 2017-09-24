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


#include <sstream>
#include <cassert>


#include "evolution.h"



using namespace xcsf;



Decision::Decision()
{}


Decision::~Decision()
{}


RandomDecision::RandomDecision(const Randomizer& generator,
			       double evolution_probability,
			       double allele_mutation_probability)
  : Decision()
  , _generator(generator)
  , _evolution_probability(evolution_probability)
  , _allele_mutation_probability(allele_mutation_probability)
{}


RandomDecision::~RandomDecision()
{}


bool
RandomDecision::shall_evolve(void) const
{
  return _generator.uniform() <= _evolution_probability;
}


bool
RandomDecision::shall_mutate(void) const
{
  return _generator.uniform() <= _allele_mutation_probability;
}



EvolutionListener::~EvolutionListener(void)
{};



NoListener::~NoListener()
{}


void
NoListener::on_rule_added(const MetaRule& rule) const
{}


void
NoListener::on_rule_deleted(const MetaRule& rule) const
{}


void
NoListener::on_breeding(const MetaRule& father, const MetaRule& mother) const
{}


void
NoListener::on_mutation(const Chromosome& subject, const Allele& locus) const
{}


LogListener::LogListener(std::ostream& out)
  : _out(out)
{};


LogListener::~LogListener()
{}


void
LogListener::on_rule_added(const MetaRule& rule) const
{
  _out << "New rule '" << rule << "'" << endl;
}


void
LogListener::on_rule_deleted(const MetaRule& rule) const
{
  _out << "Deleted rule '" << rule << "'" << endl;
}


void
LogListener::on_breeding(const MetaRule& father, const MetaRule& mother) const
{
  _out << "Breeding:" << endl
       << " - Father: " << father << endl
       << " - Mother: " << mother << endl;
}


void
LogListener::on_mutation(const Chromosome& subject, const Allele& locus) const
{
  _out << "Mutation of " << subject << " at " << locus << endl;
}



Evolution::~Evolution()
{}



DefaultEvolution::DefaultEvolution(MetaRulePool&		rules,
				   const Decision&		decision,
				   const Crossover&		crossover,
				   const Selection&		selection,
				   const AlleleMutation&	mutation,
				   const EvolutionListener&	listener)
  : _decision(decision)
  , _crossover(crossover)
  , _select_parents(selection)
  , _mutate(mutation)
  , _listener(listener)
  , _rules(rules)
  , _codec(rules)
{}


DefaultEvolution::~DefaultEvolution()
{}


void
DefaultEvolution::evolve(RuleSet& rules) const
{
  if (not _decision.shall_evolve()) return;

  assert (not rules.is_empty() && "Impossible evolution, no rules");

  auto deleted_rules = rules.enforce_capacity(_crossover.children_count(),
					       Comparators::with_lower_weighted_payoff);
  for(auto each : deleted_rules) {
    _listener.on_rule_deleted(*each);
  }

  auto parents = _select_parents(rules);

  auto children = breed(*parents[0], *parents[1]);
  for (auto each_child: children) {
    rules.add(*each_child);
  }

}


void
DefaultEvolution::initialise(RuleSet& rules) const
{
  const Value delta(25);

  Vector rule_1 = { 25 };
  create_rule(rules, rule_1, delta, 25);

  Vector rule_2 = { 75 };
  create_rule(rules, rule_2, delta, 75);
}


void
DefaultEvolution::create_rule(RuleSet&		rules,
			      const Vector&	seed,
			      const Value&	tolerance,
			      const Value&	prediction) const
{
  vector<Interval> constraints;
  for (unsigned int index=0 ;
       index<rules.dimensions().input_count() ;
       ++index) {
    constraints.push_back(Interval(seed[index]-tolerance,
				   seed[index]+tolerance));
  }

  vector<unsigned int> predictions;
  for (unsigned int index=0 ;
       index<rules.dimensions().output_count() ;
       ++index) {
    predictions.push_back(static_cast<unsigned int>(prediction));
  }

  MetaRule *rule = make_rule(constraints, predictions);
  rules.add(*rule);
}


const Performance DEFAULT(0, 0, 0);

MetaRule*
DefaultEvolution::make_rule(std::vector<Interval>	constraints,
			    std::vector<unsigned int>	predictions) const
{
  MetaRule* rule = _rules.acquire(Rule(constraints, predictions), DEFAULT);
  _listener.on_rule_added(*rule);
  return rule;
}


vector<MetaRule*>
DefaultEvolution::breed(const MetaRule& father, const MetaRule& mother) const
{
  vector<Chromosome> children;
  _crossover(_codec.encode(father),
	     _codec.encode(mother),
	     children);

  _listener.on_breeding(father, mother);

  Performance performance
    = Performance((father.fitness() + mother.fitness()) / 2,
		  (father.payoff() + mother.payoff()) / 2,
		  (father.error() + mother.error()) / 2);

  vector<MetaRule*> children_rules;
  for (auto each_child: children) {
      mutate(each_child);
      MetaRule *rule = _codec.decode(father.dimensions(), each_child, performance);
      children_rules.push_back(rule);
      _listener.on_rule_added(*rule);
  }

  return children_rules;
}


void
DefaultEvolution::mutate(Chromosome& child) const
{
  for(Allele each_locus=0 ; each_locus<child.size() ; ++each_locus) {
    if (_decision.shall_mutate()) {
      _mutate(child, each_locus);
      _listener.on_mutation(child, each_locus);
    }
  }
}
