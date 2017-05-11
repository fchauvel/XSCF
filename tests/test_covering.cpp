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


#include "covering.h"

#include "helpers.h"


using namespace xcsf;



TEST_GROUP(TestCovering)
{
  Randomizer *randomizer;
  const unsigned int strength = 6;
  MetaRulePool pool;
  RandomCovering* cover;
  RuleSet rules;

  void setup(void) {
    randomizer = new TestableRandomizer({ 0., 1., 0.5});
    cover = new RandomCovering(pool, strength, *randomizer);
  }

  void teardown(void) {
    delete randomizer;
    delete cover;
  }
  
};


TEST(TestCovering, test_covering)
{
  Vector context = { 6 };
  (*cover)(rules, context);

  CHECK_EQUAL(strength, rules.size());
}

