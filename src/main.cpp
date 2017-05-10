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

#include <fstream>

#include "application.h"
#include "evolution.h"


using namespace xcsf;


int
main(int argc, char** argv)
{
  cout << APPLICATION << " v" << VERSION << endl;

  const std::string LOG_FILE("evolution.log");
  const unsigned int INPUT_COUNT(1);
  const unsigned int OUTPUT_COUNT(1);
  const unsigned int CAPACITY(100);
  const double EVOLUTION_PROBABILITY(0.25);
  const double MUTATION_PROBABILITY(0.1);

  Randomizer randomizer;
  RandomDecision decisions(randomizer, EVOLUTION_PROBABILITY, MUTATION_PROBABILITY);
  RouletteWheel selection(randomizer);
  TwoPointCrossover crossover(randomizer);
  RandomAlleleMutation mutation(randomizer);

  std::ofstream log;
  log.open(LOG_FILE, std::ofstream::out);
  LogListener listener(log);
  
  Evolution evolution(decisions,
		      crossover,
		      selection,
		      mutation,
		      listener,
		      INPUT_COUNT,
		      OUTPUT_COUNT,
		      CAPACITY);

  NaiveReward reward(0.25);
  
  Application application(cin, cout, evolution, reward);
  application.run();

  log.close();
  return 0;
}
