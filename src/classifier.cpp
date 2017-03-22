

#include "classifier.h"


Classifier::Classifier(Action& action):
  _action(action)
{}


Classifier::~Classifier()
{}


Action&
Classifier::action(void) const
{
  return _action;
}
