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


#include <vector>
#include <map>


#include "context.h"
#include "utils.h"
#include "actions.h"


namespace xcsf {


  class Formatter;
  
  
  class Rule
  {
  public:
    Rule(const vector<Interval>& constraints, const Vector& prediction, double fitness, double payoff, double error);
    Rule(const Rule& prototype);
    ~Rule();
    
    Rule& operator = (const Rule& prototype);
    bool operator == (const Rule& other_rule) const;
    bool operator != (const Rule& other_rule) const;

    void accept(Formatter& visitor) const;
    
    // Conversions
    vector<unsigned int> as_vector(void) const;
    
    double fitness(void) const;
    double error(void) const;
    double payoff(void) const;
    double weighted_payoff(void) const;
    
    void update(double payoff, double error, double fitness);
    
    bool match(const Vector& input) const;
    const Vector& outputs(void) const;
    
  private:
    friend std::ostream& operator << (std::ostream& out, const Rule& rule);

    vector<Interval> _intervals;
    Vector _outputs;
    double _fitness;
    double _payoff;
    double _error;
    
  };


  class Performance
  {
  public:
    explicit Performance(double fitness=0.0, double payoff=0.0, double error=0.0);
    Performance(const Performance& other);
    ~Performance();

    Performance& operator = (const Performance& other);
    bool operator == (const Performance& other) const;
    bool operator != (const Performance& other) const;

    double fitness(void) const;
    double payoff(void) const;
    double error(void) const;
    
  private:
    friend std::ostream& operator << (std::ostream& out, const Performance performance);
    
    double _fitness;
    double _payoff;
    double _error;
    
  };



  class RewardFunction
  {
  public:
    virtual ~RewardFunction();

    virtual void operator () (double reward, vector<Rule*>& rules) const = 0;
  };


  class NaiveReward
    : public RewardFunction
  {
  public:
    NaiveReward(double learning_rate);
    virtual ~NaiveReward();

    virtual void operator () (double reward, vector<Rule*>& rules) const;
    
  private:
    double _learning_rate;
    
  };

  
  class WilsonReward
    : public RewardFunction
  {
  public:
    WilsonReward(double learning_rate, double error, double v);
    virtual ~WilsonReward();

    virtual void operator () (double reward, vector<Rule*>& rules) const;

  private:
    double _learning_rate;
    double _error;
    double _v;

  };

  
  class RuleSet
  {
  public:
    RuleSet();
    virtual ~RuleSet(void);
    
    bool operator == (const RuleSet& rules) const;
    Rule& operator [] (unsigned int index) const;

    void accept(Formatter& visitor) const;
    
    bool empty(void) const;
    RuleSet& add(Rule& rule);
    Rule& remove(unsigned int index);

    unsigned int worst(void) const;
    double total_fitness(void) const; // TODO delete!
    double total_weighted_payoff(void) const;
    
    std::size_t size(void) const;
    bool rewards_more_than(const RuleSet& other) const;
    double average_payoff(void) const;

    void reward(double reward);
    
  private:
    void validate(unsigned int index) const;

    vector<Rule*> _rules;
  };


  class ActivationGroup: public RuleSet
  {
  public:
    ActivationGroup(RuleSet& rules, const Vector& context);
    ~ActivationGroup();

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

    void format(const vector<Rule*>& rules);
    void format(const vector<Interval>& antecedant, const Vector& conclusion, const Performance& performance);
    
  private:
    ostream& _out;
    
  };


}

#endif
