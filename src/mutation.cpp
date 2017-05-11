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


#include <iostream>
#include <sstream>
#include <memory>


#include "mutation.h"
#include "context.h"


using namespace std;
using namespace xcsf;


AlleleMutation::~AlleleMutation()
{}



RandomAlleleMutation::RandomAlleleMutation(const Randomizer& generator, Value maximum)
  : AlleleMutation()
  , _generate(generator)
  , _maximum(static_cast<unsigned int>(maximum))
{};


RandomAlleleMutation::~RandomAlleleMutation()
{}


void
RandomAlleleMutation::operator () (Chromosome& subject, const Allele& locus) const
{
  if (locus >= subject.size()) {
    stringstream err;
    err << "Invalid locus '" << locus
	<< "'! The chromosome's length is only " << subject.size() << ".";
    throw std::invalid_argument(err.str());
  }

  unsigned int draw = _generate.unsigned_int(0, 2 * _maximum);
  if (draw >= _maximum) {
    unsigned int update = std::min(draw - _maximum, Value::MAXIMUM - subject[locus]);
    subject[locus] += update;
  } else {
    unsigned int update = std::min(_maximum - draw, subject[locus]);
    subject[locus] -= update;
  }
  
}
