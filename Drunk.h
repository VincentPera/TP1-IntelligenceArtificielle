#ifndef DRUNK_H
#define DRUNK_H
//------------------------------------------------------------------------
//
//  Name:   Drunk.h
//
//  Desc:   A class defining a druck man.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "DrunkOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

//the amount of alccol the drunk man has in blood.
const int DrukenFloor       = 3;



class Drunk : public BaseGameEntity
{
private:

  //an instance of the state machine class
  StateMachine<Drunk>*  m_pStateMachine;
 

  //how many alcool the drunk man has in blood
  int                   m_AlcoolInBlood;

public:

  Drunk(int id):m_AlcoolInBlood(0),
                          BaseGameEntity(id)
                               
  {
    //set up state machine
    m_pStateMachine = new StateMachine<Drunk>(this);
    
    m_pStateMachine->SetCurrentState(OrderADrinkAndBottomUp::Instance());

    /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
  }

  ~Drunk(){delete m_pStateMachine;}

  //this must be implemented
  void Update();

  //so must this
  virtual bool  HandleMessage(const Telegram& msg);

  
  StateMachine<Drunk>* GetFSM()const{return m_pStateMachine;}


  
  //-------------------------------------------------------------accessors
    
  int           GetAlcool()const{return m_AlcoolInBlood;}
  void          SetAlcool(int val){m_AlcoolInBlood = val;}
  void          AddADrink(){m_AlcoolInBlood += 1;};
  bool          IsDrunk()const{return m_AlcoolInBlood >= DrukenFloor;}

};



#endif
