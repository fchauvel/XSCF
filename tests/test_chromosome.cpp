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

#include <sstream>

#include "chromosome.h"



using namespace xcsf;



TEST_GROUP(TestChromosome)
{};


TEST(TestChromosome, test_debug_formatting)
{
  Chromosome chromosome = { 12, 20, 30 };
  stringstream out;
  out << chromosome;

  CHECK_EQUAL("{ 12, 20, 30 }", out.str());
}
