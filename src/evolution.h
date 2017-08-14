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

#ifndef XCSF_EVOLUTION_H
#define XCSF_EVOLUTION_H


#include <vector>

#include "utils.h"
#include "rule.h"
#include "selection.h"
#include "crossover.h"
#include "mutation.h"


namespace xcsf {


  class Decision
  {
  public:
    Decision();
    virtual ~Decision();

    virtual bool shall_evolve(void) const = 0;
    virtual bool shall_mutate(void) const = 0;

  };


  class RandomDecision: public Decision
  {
  public:
    RandomDecision(const Randomizer& generator,
		   double evolution_probability,
		   double allele_mutation_probability);
    virtual ~RandomDecision(void);

    virtual bool shall_evolve(void) const;
    virtual bool shall_mutate(void) const;

  private:
    const Randomizer& _generator;
    const double _evolution_probability;
    const double _allele_mutation_probability;

  };


  class  EvolutionListener
  {
  public:
    virtual ~EvolutionListener(void);

    virtual void on_rule_added(const MetaRule& rule) const = 0;
    virtual void on_rule_deleted(const MetaRule& rule) const = 0;
    virtual void on_breeding(const MetaRule& father, const MetaRule& mother) const = 0;
    virtual void on_mutation(const Chromosome& chromosome, const Allele& locus) const = 0;
  };


  class NoListener
    : public EvolutionListener
  {
  public:
    virtual ~NoListener();

    virtual void on_rule_added(const MetaRule& rule) const;
    virtual void on_breeding(const MetaRule& father, const MetaRule& mother) const;
    virtual void on_rule_deleted(const MetaRule& rule) const;
    virtual void on_mutation(const Chromosome& chromosome, const Allele& locus) const;
  };


  class LogListener
    : public EvolutionListener
  {
  public:
    explicit LogListener(std::ostream& out);
    virtual ~LogListener();

    virtual void on_rule_added(const MetaRule& rule) const;
    virtual void on_breeding(const MetaRule& father, const MetaRule& mother) const;
    virtual void on_rule_deleted(const MetaRule& rule) const;
    virtual void on_mutation(const Chromosome& chromosome, const Allele& locus) const;

  private:
    std::ostream& _out;

  };



  /**
   * Standard behaviour of Evolution object, that is, to support rule
   * set initialisation and evolution
   */
  class Evolution
  {
  public:
    virtual ~Evolution();

    virtual void
      initialise(RuleSet& rule_set) const = 0;

    virtual void
      evolve(RuleSet& rules) const = 0;

    // FIXME: Should be removed, as it is now the responsibility of
    // the Covering objects
    virtual void
      create_rule_for(RuleSet& rules, const Vector& context) const = 0;

  };



  class DefaultEvolution
    : public Evolution
  {
  public:
    DefaultEvolution(const Decision&		decisions,
		     const Crossover&		crossover,
		     const Selection&		selection,
		     const AlleleMutation&	mutation,
		     const EvolutionListener&	listener);

    virtual ~DefaultEvolution();

    virtual void
      initialise(RuleSet& rule_set) const;

    virtual void
      evolve(RuleSet& rules) const;

    virtual void
      create_rule_for(RuleSet& rules, const Vector& context) const;

    std::vector<MetaRule*>
      breed(const MetaRule& father, const MetaRule& mother) const;

    MetaRule*
      decode(const Dimensions& dimensions,
	     const Chromosome&,
	     const Performance&) const;

    Chromosome
      encode(const MetaRule& rule) const;

  private:
    void mutate(Chromosome& child) const;
    void remove(RuleSet& rules, unsigned int excess) const;
    void enforce_capacity(RuleSet& rules, unsigned int count) const;
    void create_rule(RuleSet& rules, const Vector& seed, const Value& tolerance, const Value& prediction) const;
    MetaRule* make_rule(std::vector<Interval>, std::vector<unsigned int>) const;

    const Decision& _decision;
    const Crossover& _crossover;
    const Selection& _select_parents;
    const AlleleMutation& _mutate;
    const EvolutionListener& _listener;
    mutable MetaRulePool _rules;
  };


}

#endif
