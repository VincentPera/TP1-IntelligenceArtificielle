#include "DrunkOwnedStates.h"
#include "fsm/State.h"
#include "Drunk.h"
#include "Locations.h"
#include "CrudeTimer.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include "MyForm1.h"

#include <iostream>
using std::cout;
using namespace System;
using namespace Projet1;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif


//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
OrderADrinkAndBottomUp* OrderADrinkAndBottomUp::Instance()
{
  static OrderADrinkAndBottomUp instance;

  return &instance;
}


void OrderADrinkAndBottomUp::Enter(Drunk* pDrunk)
{
	// Order a drink

	pDrunk->addSpeech(">> Bartender! A drink!\n");


}


void OrderADrinkAndBottomUp::Execute(Drunk* pDrunk)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pDrunk->AddADrink();

  pDrunk->addSpeech(">> *gloup gloup* Oh hell yeah !\n");


  //if enough gold mined, go and put it in the bank
  if (pDrunk->IsDrunk())
  {
    pDrunk->GetFSM()->ChangeState(DrunkenRage::Instance());
	//MyForm1::changeStateSelBastien("DrunkenRage");
  }
}


void OrderADrinkAndBottomUp::Exit(Drunk* pDrunk)
{
	pDrunk->addSpeech(">> Well. It was a good one.\n");

}


bool OrderADrinkAndBottomUp::OnMessage(Drunk* pDrunk, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

DanceOnTheTable* DanceOnTheTable::Instance()
{
  static DanceOnTheTable instance;

  return &instance;
}

void DanceOnTheTable::Enter(Drunk* pDrunk)
{  

	//on entry the miner makes sure he is located at the bank
	pDrunk->addSpeech(">> Happy dance on the table.\n");

}


void DanceOnTheTable::Execute(Drunk* pDrunk)
{
  //reset the alcool in blood
  pDrunk->SetAlcool(0);
  pDrunk->addSpeech(">> HEEEEEEEEEEE MACARENA !!!\n");


  pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());
  //MyForm1::changeStateSelBastien("OrderADrinkAndBottomUp");
}


void DanceOnTheTable::Exit(Drunk* pDrunk)
{
	pDrunk->addSpeech(">> This dance made my day! Let's get down.\n");

}


bool DanceOnTheTable::OnMessage(Drunk* pDrunk, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

DrunkenRage* DrunkenRage::Instance()
{
  static DrunkenRage instance;

  return &instance;
}

void DrunkenRage::Enter(Drunk* pDrunk)
{
	pDrunk->addSpeech(">> The next guys coming in better pay his drink!\n");
	
}

void DrunkenRage::Execute(Drunk* pDrunk)
{ 
  //if miner is not fatigued start to dig for nuggets again.

	     Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pDrunk->ID(),        //ID of sender
                              ent_Miner_Bob,            //ID of recipient
                              Msg_Tournee,   //the message
                              NO_ADDITIONAL_INFO);  
   
}

void DrunkenRage::Exit(Drunk* pDrunk)
{ 
}


bool DrunkenRage::OnMessage(Drunk* pDrunk, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();

   switch(msg.Msg)
   {
   case Msg_ImRich:

     cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	 pDrunk->addSpeech(">> YES ! FREE ROUND FOR EVERYONE !\n");


     pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());
      
     return true;

   case Msg_ImNotRich:

	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": ";

	   pDrunk->addSpeech(">> Kidding Me ?! Not enough rich ?!\n");
	  

	   return true;

   case Msg_LostEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	pDrunk->addSpeech(">> I win! Now pay what you own me!\n");
	   

	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());

	   return true;

   case Msg_LostNotEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	pDrunk->addSpeech(">> I win! Get out your poor jackass!\n");


	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());

	   return true;

   case Msg_Win:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	pDrunk->addSpeech(">> Lost this one.. Okay.\n");


	   pDrunk->SetAlcool(0);
	   pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());

	   return true;

   }//end switch

   return false; //send message to global message handler
}