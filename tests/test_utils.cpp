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

#include "utils.h"


using namespace xcsf;


class TestableRandomizer: public Randomizer
{
public:
  TestableRandomizer(double number)
    :Randomizer()
    ,_random_number(number)
  {};

  virtual ~TestableRandomizer()
  {};

  virtual double uniform(void) const
  {
    return _random_number;
  }
    
private:
  const double _random_number;

};



TEST_GROUP(TestRandomizer)
{
  double canned_value = 0.234;
  Randomizer *generate;

  void setup(void)
  {
    generate = new TestableRandomizer(canned_value);
  }

  void teardown(void)
  {
    delete generate;
  }

};


TEST(TestRandomizer, test_canned_response)
{
  double random_number = generate->uniform();
  
  CHECK(random_number == canned_value);
}
