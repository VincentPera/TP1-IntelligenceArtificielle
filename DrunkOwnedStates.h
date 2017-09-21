#ifndef DRUNK_OWNED_STATES_H
#define DRUNK_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   DrunkOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Drunk class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class Drunk;
struct Telegram;




//------------------------------------------------------------------------
//
//  In this state the DRUNK will choose between fight or dance, depends on
//  the miner. 
//------------------------------------------------------------------------
class DrunkenRage : public State<Drunk>
{
private:
  
  DrunkenRage(){}

  //copy ctor and assignment should be private
  DrunkenRage(const DrunkenRage&);
  DrunkenRage& operator=(const DrunkenRage&);
 
public:

  //this is a singleton
  static DrunkenRage* Instance();

  virtual void Enter(Drunk* miner);

  virtual void Execute(Drunk* miner);

  virtual void Exit(Drunk* miner);

  virtual bool OnMessage(Drunk* agent, const Telegram& msg);

};

//------------------------------------------------------------------------
//
//  Entity will go to a bank and deposit any nuggets he is carrying. If the 
//  miner is subsequently wealthy enough he'll walk home, otherwise he'll
//  keep going to get more gold
//------------------------------------------------------------------------
class OrderADrinkAndBottomUp : public State<Drunk>
{
private:
  
  OrderADrinkAndBottomUp(){}

  //copy ctor and assignment should be private
  OrderADrinkAndBottomUp(const OrderADrinkAndBottomUp&);
  OrderADrinkAndBottomUp& operator=(const OrderADrinkAndBottomUp&);
 
public:

  //this is a singleton
  static OrderADrinkAndBottomUp* Instance();

  virtual void Enter(Drunk* miner);

  virtual void Execute(Drunk* miner);

  virtual void Exit(Drunk* miner);

  virtual bool OnMessage(Drunk* agent, const Telegram& msg);
};


//------------------------------------------------------------------------
//
//  miner will go home and sleep until his fatigue is decreased
//  sufficiently
//------------------------------------------------------------------------
class DanceOnTheTable : public State<Drunk>
{
private:
  
  DanceOnTheTable(){}

  //copy ctor and assignment should be private
  DanceOnTheTable(const DanceOnTheTable&);
  DanceOnTheTable& operator=(const DanceOnTheTable&);
 
public:

  //this is a singleton
  static DanceOnTheTable* Instance();

  virtual void Enter(Drunk* miner);

  virtual void Execute(Drunk* miner);

  virtual void Exit(Drunk* miner);

  virtual bool OnMessage(Drunk* agent, const Telegram& msg);
};







#endif