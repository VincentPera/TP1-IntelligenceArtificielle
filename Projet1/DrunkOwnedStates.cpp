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
	// Change the label on the UI
	MyForm1::changeStateSelBastien("OrderADrinkAndBottomUp");
	// Order a drink
	MyForm1::getTextSelbatien()->Text += ": Bartender! A drink!";
	MyForm1::getTextSelbatien()->Text += "\n";
	MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	MyForm1::getTextSelbatien()->ScrollToCaret();

}


void OrderADrinkAndBottomUp::Execute(Drunk* pDrunk)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pDrunk->AddADrink();

  MyForm1::getTextSelbatien()->Text += ">> *gloup gloup* Oh hell yeah !";
  MyForm1::getTextSelbatien()->Text += "\n";
  MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
  MyForm1::getTextSelbatien()->ScrollToCaret();

  //if enough gold mined, go and put it in the bank
  if (pDrunk->IsDrunk())
  {
    pDrunk->GetFSM()->ChangeState(DrunkenRage::Instance());
	//MyForm1::changeStateSelBastien("DrunkenRage");
  }
}


void OrderADrinkAndBottomUp::Exit(Drunk* pDrunk)
{
	MyForm1::getTextSelbatien()->Text += ">> Well. It was a good one.";
	MyForm1::getTextSelbatien()->Text += "\n";
	MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	MyForm1::getTextSelbatien()->ScrollToCaret();
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
	// Change the label on the UI
	MyForm1::changeStateSelBastien("DanceOnTheTable");

	//on entry the miner makes sure he is located at the bank
	MyForm1::getTextSelbatien()->Text += ">> Happy dance on the table.";
	MyForm1::getTextSelbatien()->Text += "\n";
	MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	MyForm1::getTextSelbatien()->ScrollToCaret();
}


void DanceOnTheTable::Execute(Drunk* pDrunk)
{
  //reset the alcool in blood
  pDrunk->SetAlcool(0);

  MyForm1::getTextSelbatien()->Text += ">> HEEEEEEEEEEE MACARENA !!!";
  MyForm1::getTextSelbatien()->Text += "\n";
  MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
  MyForm1::getTextSelbatien()->ScrollToCaret();

  pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());
  //MyForm1::changeStateSelBastien("OrderADrinkAndBottomUp");
}


void DanceOnTheTable::Exit(Drunk* pDrunk)
{
	MyForm1::getTextSelbatien()->Text += ">> This dance made my day! Let's get down.";
	MyForm1::getTextSelbatien()->Text += "\n";
	MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	MyForm1::getTextSelbatien()->ScrollToCaret();
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
	// Change the label on the UI
	MyForm1::changeStateSelBastien("DrunkenRage");

	MyForm1::getTextSelbatien()->Text += ">> The next guys coming in better pay his drink!";
	MyForm1::getTextSelbatien()->Text += "\n";
	MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	MyForm1::getTextSelbatien()->ScrollToCaret();
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

	 MyForm1::getTextSelbatien()->Text += ">> YES ! FREE ROUND FOR EVERYONE !";
	 MyForm1::getTextSelbatien()->Text += "\n";
	 MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	 MyForm1::getTextSelbatien()->ScrollToCaret();

     pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());
	 MyForm1::changeStateSelBastien("DanceOnTheTable");
      
     return true;

   case Msg_ImNotRich:

	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": ";

	   MyForm1::getTextSelbatien()->Text += ">> Kidding Me ?! Not enough rich ?!";
	   MyForm1::getTextSelbatien()->Text += "\n";
	   MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	   MyForm1::getTextSelbatien()->ScrollToCaret();

	   return true;

   case Msg_LostEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   MyForm1::getTextSelbatien()->Text += ">> I win! Now pay what you own me!";
	   MyForm1::getTextSelbatien()->Text += "\n";
	   MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	   MyForm1::getTextSelbatien()->ScrollToCaret();

	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());
	   MyForm1::changeStateSelBastien("DanceOnTheTable");

	   return true;

   case Msg_LostNotEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   MyForm1::getTextSelbatien()->Text += ">> I win! Get out your poor jackass!";
	   MyForm1::getTextSelbatien()->Text += "\n";
	   MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	   MyForm1::getTextSelbatien()->ScrollToCaret();

	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());
	   MyForm1::changeStateSelBastien("DanceOnTheTable");

	   return true;

   case Msg_Win:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   MyForm1::getTextSelbatien()->Text += ">> Lost this one.. Okay.";
	   MyForm1::getTextSelbatien()->Text += "\n";
	   MyForm1::getTextSelbatien()->SelectionStart = MyForm1::getTextSelbatien()->Text->Length;
	   MyForm1::getTextSelbatien()->ScrollToCaret();

	   pDrunk->SetAlcool(0);
	   pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());
	   MyForm1::changeStateSelBastien("OrderADrinkAndBottomUp");

	   return true;

   }//end switch

   return false; //send message to global message handler
}