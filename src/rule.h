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

#ifndef XCSF_RULE_H
#define XCSF_RULE_H


#include <list>
#include <vector>
#include <map>


#include "context.h"
#include "utils.h"
#include "actions.h"


namespace xcsf {


  class Formatter;


  class Dimensions
  {
  public:
    Dimensions(unsigned int input_count, unsigned int output_count);

    unsigned int input_count(void) const;
    unsigned int output_count(void) const;

    void validate_inputs(const Vector& input) const;

    bool operator == (const Dimensions& other) const;
    bool operator != (const Dimensions& other) const;

    friend std::ostream& operator << (std::ostream& out, const Dimensions& dimensions);
    
  private:
    unsigned int _input_count;
    unsigned int _output_count;

  };
  
    
  class Rule
  {
  public:
    Rule(const vector<Interval>& premises, const Vector& conclusion);

    const Dimensions& dimensions(void) const;
    const Vector& conclusion() const;
    
    bool is_triggered_by(const Vector& context) const;
    bool subsumes(const Rule& rule) const;
      
    const Vector& operator () (const Vector& context) const;
    
    bool operator == (const Rule& other) const;
    bool operator != (const Rule& other) const;
    
    friend std::ostream& operator << (std::ostream& out, const Rule& rule);

    explicit operator vector<unsigned int> () const;

    
  private:
    vector<Interval> _premises;
    Vector _conclusion;
    Dimensions _dimensions;
    
  };


  class Performance
  {
  public:
    Performance(double fitness=0.0, double payoff=0.0, double error=0.0);
    
    bool operator == (const Performance& other) const;
    bool operator != (const Performance& other) const;

    double fitness(void) const;
    double payoff(void) const;
    double error(void) const;

    friend std::ostream& operator << (std::ostream& out, const Performance performance);
    
  private:
    double _fitness;
    double _payoff;
    double _error;
    
  };
    
  
  class MetaRule
  {
  public:
    MetaRule(const Rule& rule, const Performance& performance);
    
    bool operator == (const MetaRule& other_rule) const;
    bool operator != (const MetaRule& other_rule) const;

    void accept(Formatter& visitor) const;
    
    // Conversions
    vector<unsigned int> as_vector(void) const;

    const Dimensions& dimensions(void) const;
    double fitness(void) const;
    double error(void) const;
    double payoff(void) const;
    double weighted_payoff(void) const;
    
    void update(double payoff, double error, double fitness);
    
    bool match(const Vector& input) const;
    const Vector& outputs(void) const;
    
  private:
    friend std::ostream& operator << (std::ostream& out, const MetaRule& rule);

    Rule _rule;
    Performance _performance;
    
  };

  
  class RuleSet
  {
  public:
    RuleSet(const Dimensions& dimensions=Dimensions(1, 1), unsigned int capacity=100);
    virtual ~RuleSet(void);

    const Dimensions& dimensions(void) const;

    unsigned int capacity(void) const;
    unsigned int remaining_capacity(void) const;
    
    bool is_empty(void) const;
    bool is_full(void) const;
    
    bool operator == (const RuleSet& rules) const;
    MetaRule& operator [] (unsigned int index) const;

    void accept(Formatter& visitor) const;
    

    RuleSet& add(MetaRule& rule);
    MetaRule& remove(unsigned int index);

    unsigned int worst(void) const;
    double total_fitness(void) const; // TODO delete!
    double total_weighted_payoff(void) const;
    
    std::size_t size(void) const;
    bool rewards_more_than(const RuleSet& other) const;
    double average_payoff(void) const;

  private:
    void validate(unsigned int index) const;

    Dimensions _dimensions;
    unsigned int _capacity;
    vector<MetaRule*> _rules;
  };


  class ActivationGroup: public RuleSet
  {
  public:
    ActivationGroup(RuleSet& rules, const Vector& context);
    virtual ~ActivationGroup();
    
  };


  class PredictionGroup
  {
  public:
    PredictionGroup(const RuleSet& rules);
    ~PredictionGroup();

    const Vector& most_rewarding(void) const;
    RuleSet& rules_to_reward(void) const;

  private:
    void _group_rules_by_prediction(const RuleSet& rules);
    RuleSet& _find_most_rewarding_rules(void) const;

    map<Vector, RuleSet*> _predictions;
    mutable RuleSet _most_rewarding;
  };


  // TODO: Remove, it should directly the evolution class.
  class RuleFactory
  {
  public:
    virtual ~RuleFactory();
    
    virtual void initialise(RuleSet& rules) const = 0;
    virtual void evolve(RuleSet& rules) const = 0;
    virtual void create_rule_for(RuleSet& rules, const Vector& context) const = 0;
    
  };


  class Formatter
  {
  public:
    Formatter(std::ostream& out);

    void format(const vector<MetaRule*>& rules);
    void format(const Rule& rule, const Performance& performance);
    
  private:
    ostream& _out;
    
  };


  // TODO: Rename MetaRuleAllocator
  class MetaRulePool
  {
  public:
    MetaRulePool();
    ~MetaRulePool();

    MetaRule* acquire(const Rule& rule, const Performance& performance=Performance(0,0,0));
    void release(MetaRule *rule);

    bool is_active(MetaRule *rule) const;
    bool is_free(MetaRule *rule) const;
    
    unsigned int active_rule_count(void) const;
    unsigned int free_rule_count(void) const;
    
  private:
    MetaRulePool(const MetaRulePool&);
    MetaRulePool& operator = (const MetaRulePool&);
    
    std::list<MetaRule*> _active_rules;
    std::list<MetaRule*> _free_rules;

  };

}

#endif
