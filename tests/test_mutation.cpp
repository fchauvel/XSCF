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


using namespace xcsf;


TEST_GROUP(TestMutation)
{
  Chromosome *chromosome;

  void setup(void)
  {
    chromosome = new Chromosome({50, 50, 50});
  }


  void teardown(void)
  {
    delete chromosome;
  }
  
};


TEST(TestMutation, simple_mutation)
{
  ShiftMutation mutation(1, 10);

  mutation(*chromosome);

  Chromosome expected = { 50, 60, 50 }; 
  CHECK(*chromosome == expected);
}


TEST(TestMutation, test_excessive_positive_mutation)
{
  ShiftMutation mutation(1, 200);

  mutation(*chromosome);

  Chromosome expected = { 50, 100, 50 }; 
  CHECK(*chromosome == expected);
}


TEST(TestMutation, test_excessive_negative_mutation)
{
  ShiftMutation mutation(1, -200);

  mutation(*chromosome);

  Chromosome expected = { 50, 0, 50 }; 
  CHECK(*chromosome == expected);
}
