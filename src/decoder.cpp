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
#include <stdexcept>

#include "decoder.h"


using namespace std;
using namespace xcsf;


const string SEPARATOR = ":";


enum Command {
  Reward,
  Predict
};


Command
parse(const string& text)
{
  if (text == "R") return Reward;
  if (text == "P") return Predict;
  throw invalid_argument("Unknown command!");
}
  

void
validate(const string& line, size_t position)
{
    if (position == string::npos) {
      stringstream error;
      error << "Invalid line '" << line << "'."
	    << "Check it does use '" << SEPARATOR << "' as a separator.";
      throw invalid_argument(error.str());
    }
}


Controllable::~Controllable()
{}



Decoder::Decoder(istream& source, Controllable& target)
  :_source(source),
   _target(target)
{}


void
Decoder::decode(void)
{
  string line;
  while (getline(_source, line)) {
    size_t position = line.find(SEPARATOR, 0);
    validate(line, position);
    Command command = parse(line.substr(0, position));
    string value = line.substr(position+1);
    switch(command) {
    case Reward:
      _target.reward(stod(value));
      break;
    case Predict:
      _target.predict(Vector::parse(value));
      break;
    }
  }
  
}

    
