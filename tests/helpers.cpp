

#include "helpers.h"



TestRuleFactory::TestRuleFactory()
  :RuleFactory(),
   _rules()
{}


TestRuleFactory::~TestRuleFactory()
{
  for (std::size_t index=0 ; index<_rules.size() ; ++index) {
    delete _rules[index];
  }
}


void
TestRuleFactory::initialise(RuleSet& rule_set) const
{
  for (vector<Rule*>::const_iterator each_rule = _rules.begin();
       each_rule != _rules.end();
       ++each_rule){
    rule_set.add(**each_rule);
  }
};


void
TestRuleFactory::define(Rule& rule)
{
  _rules.push_back(&rule);
}



RuleFactory&
Helper::one_rule(std::vector<int> predictions) {
  TestRuleFactory *factory = new TestRuleFactory();
  const double fitness = 0.75;
  const double payoff = 0.75;
  const double error = 1.0;
  Rule *single_rule = new Rule({Interval(0, 100)}, predictions, fitness, payoff, error);
  factory->define(*single_rule);
  return *factory;
}
