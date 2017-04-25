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


#include "CppUTest/TestHarness.h"
#include "CppUTestExt/MockSupport.h"

#include <iostream>
#include <sstream>
#include <stdexcept>

#include "context.h"
#include "decoder.h"


using namespace std;
using namespace xcsf;


class TestControllable: public Controllable
{
  virtual ~TestControllable()
  {};

  virtual void reward(double value)
  {
    mock()
      .actualCall("reward")
      .onObject(this)
      .withParameter("value", value);
  }

  virtual void predict(const Vector& context)
  {
    mock()
      .actualCall("predict").
      onObject(this)
      .withParameterOfType("Vector", "context", (void*) &context);
  }
  
};


class VectorComparator : public MockNamedValueComparator
{
public:
  virtual bool isEqual(void* object1, void* object2)
  {
    Vector left = *(Vector*) (object1);
    Vector right = *(Vector*) (object2);
    return left == right;
  }
  
  virtual SimpleString valueToString(void* object)
  {
    return StringFrom(object);
  }
};


TEST_GROUP(TestReader)
{
  VectorComparator comparator;
  stringstream input;
  Controllable *target;
  Decoder *reader;
  
  void setup(void) {
    mock().installComparator("Vector", comparator);
    
    target = new TestControllable();
    reader = new Decoder(input, *target);
  }

  void teardown(void) {
    delete target;
    delete reader;
    mock().removeAllComparators();
    mock().clear();
  }
  
};


TEST(TestReader, test_reading_invalid_command)
{
  mock().expectNCalls(0, "reward");
  mock().expectNCalls(0, "predict");
  
  input << "UNKNOWN_COMMAND:(13,34,234)" << endl;
  CHECK_THROWS(std::invalid_argument, {reader->decode();});

  mock().checkExpectations();
}


TEST(TestReader, test_reading_reward)
{
  mock().expectOneCall("reward").onObject(target).withParameter("value", 23.5);

  input << "R:23.5" << endl;
  reader->decode();
  
  mock().checkExpectations();
}


TEST(TestReader, test_reading_another_reward)
{
  mock().expectOneCall("reward").onObject(target).withParameter("value", 15.34);
  
  input << "R:15.34" << endl;
  reader->decode();

  mock().checkExpectations();
}


TEST(TestReader, test_reading_invalid_reward)
{
  mock().expectNCalls(0, "reward");
  
  input << "R=15.34" << endl;
  CHECK_THROWS(std::invalid_argument, {reader->decode();});

  mock().checkExpectations();
}


TEST(TestReader, test_reading_input)
{
  Vector expected = { 10, 20, 30 };
  mock().expectOneCall("predict").onObject(target).withParameterOfType("Vector", "context", &expected);
  
  input << "P:(10, 20, 30)" << endl;
  reader->decode();

  mock().checkExpectations();
}

