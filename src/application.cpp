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


#include "application.h"


using namespace xcsf;


Application::Application(istream&		input,
			 ostream&		output,
			 const Evolution&	evolution,
			 const RewardFunction&	reward)
  : _encoder(new Encoder(output))
  , _controller(new AgentController(*_encoder, evolution, reward))
  , _decoder(new Decoder(input, *_controller))
{
}


Application::~Application()
{
  delete _encoder;
  delete _decoder;
  delete _controller;
}


void
Application::run(void) const {
  _decoder->decode();
}
