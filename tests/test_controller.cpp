

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
  Action* action;
  Population* population;
  Agent* agent;
  Actuator* actuator;
  Controller* controller;

  void setup(void)
  {
    population = new Population();
    action = new Action(4);
    population->add_classifier(*action);
    actuator = new FakeActuator();
    agent = new FakeAgent(*population);
    controller = new Controller(*actuator, *agent);
  }

  void teardown(void) {
    delete action;
    delete controller;
    delete agent;
    delete actuator;
    delete population;
    mock().clear();
  }
  
};


TEST(TestController, test_invoke_agent){
  mock().expectOneCall("set");
  mock().expectOneCall("select_action");
  Context context(0);
  controller->select_action(context);
  mock().checkExpectations();
}
