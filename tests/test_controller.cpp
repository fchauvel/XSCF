

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "controller.h"
#include "helpers.h"


class FakeActuator: public Actuator
{
public:
  FakeActuator():Actuator(){};
  ~FakeActuator(){};
  
  virtual void set(int value) {
    mock().actualCall("set");
  }; 

};


class FakeAgent: public Agent
{
public:
  FakeAgent(const RuleFactory& factory):
    Agent(factory)
  {}

  const Vector& select_action(const Vector& inputs) {
    mock().actualCall("select_action");
    return Agent::select_action(inputs);
  }
  
};


TEST_GROUP(TestController)
{
  Agent* agent;
  Actuator* actuator;
  Controller* controller;
  const RuleFactory& factory = Helper::one_rule({ 4 });
 
  void setup(void)
  {
    agent = new FakeAgent(factory);
    actuator = new FakeActuator();
    controller = new Controller(*actuator, *agent);
  }

  void teardown(void) {
    delete controller;
    delete agent;
    delete actuator;
    delete &factory;
    mock().clear();
  }
  
};


TEST(TestController, test_invoke_agent){
  mock().expectOneCall("set");
  mock().expectOneCall("select_action");
  Vector inputs(1);
  controller->select_action(inputs);
  mock().checkExpectations();
}
