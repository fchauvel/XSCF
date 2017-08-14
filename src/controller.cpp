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

#include "controller.h"


using namespace xcsf;


Controller::~Controller()
{}



const string SEPARATOR = ":";


enum Command {
  Reward,
  Predict,
  Show
};


Command
parse(const string& text)
{
  if (text == "R") return Reward;
  if (text == "P") return Predict;
  if (text == "S") return Show;
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


Decoder::Decoder(istream& source, Controller& target)
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
    case Show:
      _target.show();
      break;
    }
  }

}


Encoder::Encoder(ostream& out)
  :_out(out)
{}


Encoder::~Encoder()
{}


void
Encoder::show_prediction(const Vector& prediction)
{
  _out << prediction << endl;
}

void
Encoder::show(const Agent& agent)
{
  agent.display_on(_out);
}


AgentController::AgentController(Encoder&		encoder,
				 const Evolution&	evolution,
				 const RewardFunction&	reward)
  : Controller()
  , _encoder(encoder)
  , _evolution(evolution)
  , _reward(reward)
  , _agents()
{
  _agents.push_back(new Agent(_evolution, _reward));
}


AgentController::~AgentController()
{
  for(auto each_agent: _agents) {
    delete each_agent;
  }
}


void
AgentController::select_action(const Vector& inputs) const
{
  Vector prediction = { 1, 2, 3 };
  _encoder.show_prediction(prediction);
};


void
AgentController::reward(double prize)
{
  _agents[0]->reward(prize);
}


void
AgentController::predict(const Vector& context)
{
  const Vector prediction = _agents[0]->predict(context);
  _encoder.show_prediction(prediction);
}


void
AgentController::show(void) const
{
  _encoder.show(*_agents[0]);
}
