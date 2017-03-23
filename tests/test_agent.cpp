
#include "CppUTest/TestHarness.h"


#include "agent.h"
#include "classifier.h"


TEST_GROUP(TestAgent)
{
};


TEST(TestAgent, test_action){
  Context context(0);
  Action expected(4);
  Population population = Population();
  
  population.add_classifier(expected);
  
  Agent agent(population);

  const Action* const actual = agent.select_action(context);

  CHECK(expected == *actual);
  
  delete actual;
}

