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

#ifndef XCSF_TEST_HELPERS_H
#define XCSF_TEST_HELPERS_H


#include <vector>
#include "utils.h"
#include "rule.h"
#include "covering.h"
#include "evolution.h"


using namespace xcsf;


class FakeCovering
  : public Covering
{
 public:

  virtual
    ~FakeCovering();

  virtual void
    operator () (RuleSet& rules, const Vector& context)
    const;

};


class TestRuleFactory
  : public Evolution
{
public:
  TestRuleFactory();
  virtual ~TestRuleFactory();

  virtual void initialise(RuleSet& rule_set) const;
  virtual void evolve(RuleSet& rules) const;
  virtual void create_rule_for(RuleSet& rules, const Vector& context) const;

  void define(MetaRule& rule);

private:
  mutable vector<MetaRule*> _rules;

};



class TestableRandomizer: public Randomizer
{
public:
  explicit TestableRandomizer(vector<double> sequence);
  virtual ~TestableRandomizer();

  virtual double uniform(void) const;

  void sequence(vector<double> sequence);

private:
  vector<double> _sequence;
  mutable unsigned int _next;

};



const bool NO_EVOLUTION = false;
const bool EVOLUTION = true;
const bool NO_MUTATION = false;
const bool MUTATION = true;


class FixedDecision: public Decision
{
public:
  FixedDecision(bool evolution, bool alelle);
  virtual ~FixedDecision();

  virtual bool shall_evolve(void) const;
  virtual bool shall_mutate(void) const;
  
private:
  bool _evolution;
  bool _allele;
  
};


#endif
