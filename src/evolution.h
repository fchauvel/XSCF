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
#include "crossover.h"
#include "mutation.h"


using namespace std;


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


  class Selection
  {
  public:
    virtual ~Selection();
    
    virtual vector<Rule*> operator () (const RuleSet& rules) const = 0;
    
  };


  class DummySelection: public Selection
  {
  public:
    DummySelection();
    ~DummySelection();

    virtual vector<Rule*> operator () (const RuleSet& rules) const;
  };
  
  
  class Evolution
  {
  public:
    Evolution(const Decision& decisions, const Crossover& crossover, const Selection& selection, const AlleleMutation& mutation, unsigned int input=1, unsigned int output=1);
    ~Evolution(void);

    void evolve(RuleSet& rules) const;

    vector<Rule*> select_parents(const RuleSet& rules) const;
    vector<Rule*> breed(const Rule& father, const Rule& mother) const;
    Rule* decode(const Chromosome&) const;
    Chromosome encode(const Rule& rule) const;
    
  private:
    void mutate(Chromosome& child) const;
    
    const Decision& _decision;
    const Crossover& _crossover;
    const Selection& _select_parents;
    const AlleleMutation& _mutate;
    unsigned int _input_count;
    unsigned int _output_count;
    mutable vector<Rule*> _rules;
  };

  
}

#endif
