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

#ifndef XCSF_APPLICATION_H
#define XCSF_APPLICATION_H


#include <iostream>

#include "controller.h"


namespace xcsf {

  class Application
  {
  public:
    Application(istream&		input,
		ostream&		output,
		const Evolution&	evolution,
		const Covering&	convering,
		const RewardFunction&	reward);

    ~Application();

    void run(void) const;

  private:
    Encoder* const	_encoder;
    Controller* const	_controller;
    Decoder* const	_decoder;
  };


}


#endif
