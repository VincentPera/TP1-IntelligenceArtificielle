#include "DrunkOwnedStates.h"
#include "fsm/State.h"
#include "Drunk.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "Time/CrudeTimer.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;


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
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine

    cout << "\n" << GetNameOfEntity(pDrunk->ID()) << " : " << "Il est l'heure d'aller boire !";

}


void OrderADrinkAndBottomUp::Execute(Drunk* pDrunk)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pDrunk->AddADrink();

  cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " << "Un verre Barman !";

  //if enough gold mined, go and put it in the bank
  if (pDrunk->IsDrunk())
  {
    pDrunk->GetFSM()->ChangeState(DrunkenRage::Instance());
  }
}


void OrderADrinkAndBottomUp::Exit(Drunk* pDrunk)
{
	cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " << "C'est pas tout ca mais faut que j'y aille !";
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

    cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " << "Aller, je vais dancer sur cette table !!";

}


void DanceOnTheTable::Execute(Drunk* pDrunk)
{
  //deposit the gold
  pDrunk->SetAlcool(0);

  cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " 
       << "HEEEEEEEEEEE MACARENA";


  //otherwise get more gold

    pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());
  
}


void DanceOnTheTable::Exit(Drunk* pDrunk)
{
	  cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " 
       << "Jme sens fatigue et totalement en forme apres cette dance !!";
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
    cout << "\n" << GetNameOfEntity(pDrunk->ID()) << ": " << "Le prochain qui rentre dans ce bar il paye sa tournee ou je le defonce !";


    //let the wife know I'm home
  
  
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

     cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": TOURNEE GENERALLLLLLE";

     pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());
      
     return true;

   case Msg_ImNotRich:

	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": Are You Kidding Me ?!";

	   return true;

   case Msg_LostEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": WHO'S THE  BOSS";

	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());

	   return true;

   case Msg_LostNotEnought:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": J'ai gagne !!!! Dance de la joie !!";

	   pDrunk->GetFSM()->ChangeState(DanceOnTheTable::Instance());

	   return true;

   case Msg_Win:

	   	   cout << "\nMessage handled by " << GetNameOfEntity(pDrunk->ID()) 
     << " at time: " << Clock->GetCurrentTime();
	   
	SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	   cout << "\n" << GetNameOfEntity(pDrunk->ID()) 
          << ": MMmm oKAY j'ai perdu ...";

	   pDrunk->SetAlcool(0);
	   pDrunk->GetFSM()->ChangeState(OrderADrinkAndBottomUp::Instance());

	   return true;

   }//end switch

   return false; //send message to global message handler
}