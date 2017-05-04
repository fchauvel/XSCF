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


TEST_GROUP(TestCrossover)
{
  Chromosome father = { 10, 20, 30 };
  Chromosome mother = { 70, 80, 90 };

  TestableRandomizer *randomizer;
  TwoPointCrossover *crossover;

  void setup(void)
  {
    randomizer = new TestableRandomizer({0.1, 0.2});
    crossover = new TwoPointCrossover(*randomizer);
  }

  void teardown(void) {
    delete randomizer;
    delete crossover;
  }
  
};


TEST(TestCrossover, cut_in_the_middle)
{
  randomizer->sequence({ 0.6, 0.5 });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 10, 80, 30 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 70, 20, 90 };
  CHECK(expected_daughter == children[1]);
  
}


TEST(TestCrossover, test_no_left_cut_point)
{
  randomizer->sequence({ 0, 0 });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 70, 20, 30 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 10, 80, 90 };
  CHECK(expected_daughter == children[1]);
  
}

TEST(TestCrossover, test_no_right_cut_point)
{
  randomizer->sequence({ 1., 1. });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 10, 20, 90 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 70, 80, 30 };
  CHECK(expected_daughter == children[1]);
  
}

TEST(TestCrossover, test_only_middle)
{
  randomizer->sequence({ 0., 1. });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 70, 80, 90 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 10, 20, 30 };
  CHECK(expected_daughter == children[1]);
  
}


TEST(TestCrossover, test_no_left_medium_right)
{
  randomizer->sequence({ 0., 0.6 });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 70, 80, 30 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 10, 20, 90 };
  CHECK(expected_daughter == children[1]);
  
}


TEST(TestCrossover, test_small_left_no_right)
{
  randomizer->sequence({ 0.6, 1. });
  
  vector<Chromosome> children;
  (*crossover)(father, mother, children);
  
  CHECK(2 == children.size());

  Chromosome expected_son = { 10, 80, 90 };
  CHECK(expected_son == children[0]);

  Chromosome expected_daughter = { 70, 20, 30 };
  CHECK(expected_daughter == children[1]);
  
}

TEST_GROUP(TestCrossoverWithInvalidRules)
{
  Chromosome father = { 10, 20, 30 };
  Chromosome mother = { 70, 80, 85, 87, 90 };
  
  void setup(void)
  {}
  
};


TEST(TestCrossoverWithInvalidRules, test)
{
  TestableRandomizer randomizer({1, 2});
  TwoPointCrossover crossover(randomizer);

  vector<Chromosome> children(2);
    
  CHECK_THROWS(invalid_argument,{ crossover(father, mother, children); });
}


TEST(TestCrossoverWithInvalidRules, test_empty_parent)
{
  TestableRandomizer randomizer({1, 2});
  TwoPointCrossover crossover(randomizer);

  Chromosome other;

  vector<Chromosome> children(2);
    
  CHECK_THROWS(invalid_argument,{ crossover(father, other, children); });
}
