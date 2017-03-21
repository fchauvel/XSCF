#include "CppUTest/TestHarness.h"

#include <sstream>
#include "actions.h"


TEST_GROUP(TestPrinter)
{
};


TEST(TestPrinter, test_set)
{
  ostringstream output;
  Actuator* actuator = new Printer(output);
  actuator->set(4);
  delete actuator;
  CHECK_EQUAL("SET 4\n", output.str());
}


