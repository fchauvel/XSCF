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
#include "actions.h"


namespace xcsf {

  using namespace std;
  
  class Interval
  {
  public:
    Interval(const Value& lower, const Value& upper);
    Interval(const Interval& prototype);
    ~Interval();
    
    Interval& operator = (const Interval& prototype);
    bool operator == (const Interval& other_interval) const;
    bool operator != (const Interval& other_interval) const;

    const Value& lower() const;
    const Value& upper() const;
    
    bool contains(const Value value) const;
    
  private:
    friend ostream& operator << (ostream& out, const Interval& interval);
    
    Value _lower_bound;
    Value _upper_bound;
    
  };
  
  
  class Rule
  {
  public:
    Rule(const vector<Interval>& constraints, const Vector& prediction, double fitness, double payoff, double error);
    Rule(const Rule& prototype);
    ~Rule();
    
    Rule& operator = (const Rule& prototype);
    bool operator == (const Rule& other_rule) const;
    bool operator != (const Rule& other_rule) const;

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
    friend ostream& operator << (ostream& out, const Rule& rule);

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
    
  private:
    friend ostream& operator << (ostream& out, const Performance performance);
    
    double _fitness;
    double _payoff;
    double _error;
    
  };
  
  
  class RuleSet
  {
  public:
    RuleSet(void);
    RuleSet(const RuleSet& prototype);
    virtual ~RuleSet(void);
    
    RuleSet& operator = (const RuleSet& population);
    bool operator == (const RuleSet& rules) const;
    Rule& operator [] (unsigned int index) const;
    
    RuleSet& add(Rule& rule);
    Rule& remove(unsigned int index);

    unsigned int worst(void) const;
    
    size_t size(void) const;
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
    //PredictionGroup(const PredictionGroup& other);
    ~PredictionGroup();

    //PredictionGroup& operator = (const PredictionGroup& other);
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
  
    virtual void initialise(RuleSet& rule_set) const = 0;
  
  };

}

#endif
