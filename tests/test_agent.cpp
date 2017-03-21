
#include "CppUTest/TestHarness.h"

#include "agent.h"


TEST_GROUP(TestAgent)
{
};


TEST(TestAgent, test_action){
  Context context;
  Population population;
  Agent agent(population);

  Action expected = Action(4);
  const Action* const actual = agent.select_action(context);

  CHECK(expected == *actual);
  
  delete actual;
}

