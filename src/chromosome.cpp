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

#include <sstream>

#include "chromosome.h"


using namespace xcsf;
using namespace std;


ostream&
xcsf::operator << (ostream& out, const Chromosome& chromosome)
{
  out << "{ ";
  for (unsigned int locus=0 ; locus<chromosome.size() ; ++locus) {
    out << chromosome[locus];
    if (locus < chromosome.size()-1) out << ", ";
  }
  out << " }";
  return out;
}



Codec::Codec(MetaRulePool& rules)
  : _rules(rules)
{}


Chromosome
Codec::encode(const MetaRule& rule) const
{
  return Chromosome(rule.as_vector());
}


MetaRule*
Codec::decode(const Dimensions&	dimensions,
	      const Chromosome&	values,
	      const Performance& performance) const
{
  using namespace std;

  if (values.size() !=
      2 * dimensions.input_count() + dimensions.output_count()) {
    ostringstream error;
    error << "Input and output sizes "
	  << dimensions
	  << " do not match the given vector size ("
	  << values.size() << ")!";
    throw invalid_argument(error.str());
  }

  vector<Interval> constraints;
  for (unsigned int index=0 ; index < dimensions.input_count() ; ++index) {
    unsigned int lower_bound = values[index * 2];
    unsigned int upper_bound = values[index * 2 + 1];
    if (lower_bound > upper_bound) {
      std::swap(lower_bound, upper_bound);
    }
    constraints.push_back(Interval(lower_bound, upper_bound));
  }

  vector<unsigned int> prediction;
  for (unsigned int index=dimensions.input_count()*2
	 ; index<values.size()
	 ; ++index) {
    prediction.push_back(values[index]);
  }

  Vector p = Vector(prediction);
  MetaRule *result = _rules.acquire(Rule(constraints, p), performance);
  return result;
}
