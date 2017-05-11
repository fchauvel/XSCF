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

#include "helpers.h"
#include "rule.h"
#include "evolution.h"

#include <iostream>
#include <sstream>
#include <stdexcept>


using namespace xcsf;


TEST_GROUP(TestAlleleMutation)
{
  AlleleMutation* mutation;
  TestableRandomizer *randomizer;

  unsigned int initial;
  unsigned int maximum;

  void setup(void)
  {
    initial = 50;
    maximum = 30;
    randomizer = new TestableRandomizer({ 0, 0.51, 1.0 });
    mutation = new RandomAlleleMutation(*randomizer, maximum);
   
  }

  void teardown(void)
  {
    delete randomizer;
    delete mutation;
  }
  
};


TEST(TestAlleleMutation, test_simple_mutation)
{
  const unsigned int initial(50);
  Chromosome chromosome({initial, initial, initial});
  
  (*mutation)(chromosome, 0);
  CHECK_EQUAL(initial - maximum, chromosome[0]);

  (*mutation)(chromosome, 1);
  CHECK_EQUAL(initial, chromosome[1]);
  
  (*mutation)(chromosome, 2);
  CHECK_EQUAL(initial + maximum -1, chromosome[2]);
}

TEST(TestAlleleMutation, test_underflow)
{
  const unsigned int initial(0);
  Chromosome chromosome({initial, initial, initial});
  
  (*mutation)(chromosome, 0);
  CHECK_EQUAL(0, chromosome[0]);
}

TEST(TestAlleleMutation, test_overflow)
{
  randomizer->sequence({ 1.0 });
  
  const unsigned int initial(Value::MAXIMUM);
  Chromosome chromosome({initial, initial, initial});
  
  (*mutation)(chromosome, 0);
  CHECK_EQUAL(Value::MAXIMUM, chromosome[0]);
}


TEST(TestAlleleMutation, test_invalid_locus)
{
  Chromosome chromosome({ 50, 50, 50});
  CHECK_THROWS(std::invalid_argument, { (*mutation)(chromosome, 120); });
}  


