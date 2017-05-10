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


#ifndef XCSF_CONTROLLER_H
#define XCSF_CONTROLLER_H


#include "agent.h"


namespace xcsf {

  
  class Controller
  {
  public:
    virtual ~Controller();
    
    virtual void reward(double value) = 0;
    virtual void predict(const Vector& context) = 0;
    virtual void show(void) const = 0;
    
  };

  
  class Decoder
  {
  public:
    Decoder(istream& source, Controller& target);
    
    void decode(void);
    
  private:
    istream& _source;
    Controller& _target;
    
  };


  class Encoder
  {
  public:
    Encoder(ostream& out);
    ~Encoder();
    
    void show_prediction(const Vector& prediction);
    void show(const Agent& agent);
    
  private:
    ostream& _out;
    
  };
  

  class AgentController: public Controller
  {
  public:
    AgentController(Encoder& encoder, const RuleFactory& factory);
    ~AgentController();
  
    void select_action(const Vector& input) const;

    virtual void reward(double value);
    virtual void predict(const Vector& context);
    virtual void show(void) const;
  
  private:
    Encoder& _encoder;
    const RuleFactory& _factory;
    vector<Agent*> _agents;
  };


  class BasicRuleFactory: public RuleFactory
  {
  public:
    BasicRuleFactory();
    ~BasicRuleFactory();

    virtual void initialise(RuleSet& rule_set) const;

  private:
    mutable vector<MetaRule*> _rules;

  };

}

#endif
