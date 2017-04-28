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


#include "utils.h"

#include <ctime>
#include <cstdlib>


using namespace xcsf;



Randomizer::Randomizer()
{
  std::srand(std::time(0));
}


Randomizer::~Randomizer()
{}


unsigned int
Randomizer::unsigned_int(unsigned int lower, unsigned int upper) const
{
  double value = lower + (upper - lower - 1) * uniform();
  return static_cast<unsigned int>(value);
}


double
Randomizer::uniform(void) const
{
  return std::rand() / RAND_MAX;
}
  



