

#include "helpers.h"



TestRuleFactory::TestRuleFactory()
  :RuleFactory(),
   _rules()
{}


TestRuleFactory::~TestRuleFactory()
{
  for (auto each_rule: _rules) {
    delete each_rule;
  }
}


void
TestRuleFactory::initialise(RuleSet& rule_set) const
{
  for (auto each_predefined_rule: _rules) {
    rule_set.add(*each_predefined_rule);
  }
};


void
TestRuleFactory::define(Rule& rule)
{
  _rules.push_back(&rule);
}

