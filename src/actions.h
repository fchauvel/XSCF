

#ifndef XCSF_ACTIONS_H
#define XCSF_ACTIONS_H

#include <iostream>


using namespace std;


class Actuator
{
public:
  Actuator();
  virtual ~Actuator() = 0;
  
  virtual void set(int value) = 0;
  
};


class Printer: public Actuator {
 public:
  Printer(ostream& out);
  ~Printer();

  virtual void set(int value);
  
 private:
  ostream& _out;

};


class Action
{
public:
  Action(int value);
  ~Action();

  bool operator ==(const Action& other) const;
  void execute_on(Actuator& destination) const;

 private:
  const int _value;
  
};


#endif
