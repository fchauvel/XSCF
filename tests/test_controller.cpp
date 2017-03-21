

#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include "controller.h"


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
  FakeAgent(Population& classifiers):Agent(classifiers){}

  const Action* const select_action(const Context& context) {
    mock().actualCall("select_action");
    return Agent::select_action(context);
  }
  
};


TEST_GROUP(TestController)
{
  Population population;
  Agent* agent;
  Actuator* actuator;
  Controller* controller;

  void setup(void)
  {
    actuator = new FakeActuator();
    agent = new FakeAgent(population);
    controller = new Controller(*actuator, *agent);
  }

  void teardown(void) {
    delete actuator;
    delete controller;
    delete agent;
    mock().clear();
  }
  
};


TEST(TestController, test_invoke_agent){
  mock().expectOneCall("set");
  mock().expectOneCall("select_action");
  Context context;
  controller->select_action(context);
  mock().checkExpectations();
}
