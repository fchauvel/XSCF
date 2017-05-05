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
#include <sstream>

using namespace xcsf;
using namespace std;



Interval::Interval(const Value& lower, const Value& upper)
  :_lower_bound(lower),
   _upper_bound(upper)
{
  if (_lower_bound > _upper_bound) {
    stringstream error;
    error << "Invalid 'empty' interval [" << lower << ", " << upper << "]";
    throw std::invalid_argument(error.str());
  }
}


Interval::Interval(const Interval& prototype)
  :_lower_bound(prototype._lower_bound),
   _upper_bound(prototype._upper_bound)
{}


Interval::~Interval()
{}


Interval&
Interval::operator = (const Interval& other)
{
  _lower_bound = other._lower_bound;
  _upper_bound = other._upper_bound;
  return *this;
}


bool
Interval::operator == (const Interval& other) const
{
  return _lower_bound == other._lower_bound
    and _upper_bound == other._upper_bound;
}


bool
Interval::operator != (const Interval& other) const
{
  return not (*this == other);
}


const Value&
Interval::lower(void) const
{
  return _lower_bound;
}


const Value&
Interval::upper(void) const
{
  return _upper_bound;
}


bool
Interval::contains(const Value value) const
{
  return _lower_bound <= value and _upper_bound >= value;
}


ostream&
xcsf::operator << (ostream& out, const Interval& interval)
{
  out << "[" << interval._lower_bound
      << "," << interval._upper_bound
      << "]";
  return out;
}



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
  return static_cast<double>(std::rand()) / RAND_MAX;
}
  



