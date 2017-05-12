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


#include "rule.h"
#include "removal.h"



using namespace xcsf;



TEST_GROUP(TestRemover)
{
  static const unsigned int rule_count = 4;

  MetaRulePool pool;
  MetaRule* rules[rule_count];
  RuleSet rule_set;
  Remover* remove;

  void setup(void) {
    remove = new Remover(pool, &RuleSet::worst);
    
    for(unsigned int i=0 ; i<rule_count ;++i) {
      rules[i] = pool.acquire(Rule({ Interval(1, 10) }, { 23 }),
			      Performance(i * 0.2, i * 0.2, i * 0.2));
      rule_set.add(*rules[i]);
    }
    
  }

  void teardown(void) {
    delete remove;
  }
  
  
};


TEST(TestRemover, test_removing_one)
{
  (*remove)(rule_set, 1);

  CHECK_EQUAL(rule_count - 1, rule_set.size());
  CHECK(pool.is_free(rules[0]));
}

TEST(TestRemover, test_removing_too_many)
{
  CHECK_THROWS(std::invalid_argument,
	       {
		 (*remove)(rule_set, rule_count + 1);
	       });

  CHECK_EQUAL(0, pool.free_rule_count());
}


