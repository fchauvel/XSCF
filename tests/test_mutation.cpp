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
  Randomizer *randomizer;
  Chromosome *chromosome;

  void setup(void)
  {
    randomizer = new TestableRandomizer({ 0 });
    mutation = new RandomAlleleMutation(*randomizer);
    chromosome = new Chromosome({50, 50, 50});
  }


  void teardown(void)
  {
    delete chromosome;
    delete randomizer;
    delete mutation;
  }
  
};


TEST(TestAlleleMutation, test_simple_mutation)
{
  (*mutation)(*chromosome, 0);
  
  CHECK_EQUAL(0, (*chromosome)[0]);
}


TEST(TestAlleleMutation, test_invalid_locus)
{
  CHECK_THROWS(std::invalid_argument, { (*mutation)(*chromosome, 120); });
}  


