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

#include "decoder.h"


using namespace std;
using namespace xcsf;

class TestControllable: public Controllable
{
  virtual ~TestControllable(){};

  virtual void reward(double value) {
    mock()
      .actualCall("reward")
      .onObject(this)
      .withParameter("value", value);
  }
};


TEST_GROUP(TestReader)
{
  stringstream input;
  Controllable *target;

  void setup(void) {
    target = new TestControllable();
  }

  void teardown(void) {
    delete target;
    mock().clear();
  }
  
};


TEST(TestReader, test_reading_reward)
{
  input << "R:23.5" << endl;
  Decoder reader(input, *target);
 
  mock().expectOneCall("reward").onObject(target).withParameter("value", 23.5);
 
  reader.decode();
  mock().checkExpectations();
}

/*
TEST(TestReader, test_reading_another_reward)
{
  input << "R:15.34" << endl;
  Decoder reader(input, *target);
 
  mock().expectOneCall("reward").onObject(target).withParameter("value", 15.34);
 
  reader.decode();
  mock().checkExpectations();
}
*/
