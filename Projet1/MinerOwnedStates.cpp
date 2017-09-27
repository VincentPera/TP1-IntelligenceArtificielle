#include <msclr\marshal_cppstd.h>
#include "MinerOwnedStates.h"
#include "fsm/State.h"
#include "Miner.h"
#include "Locations.h"
#include "messaging/Telegram.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "CrudeTimer.h"
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
EnterMineAndDigForNugget* EnterMineAndDigForNugget::Instance()
{
  static EnterMineAndDigForNugget instance;

  return &instance;
}


void EnterMineAndDigForNugget::Enter(Miner* pMiner)
{
  //if the miner is not already located at the goldmine, he must
  //change location to the gold mine
  if (pMiner->Location() != goldmine)
  {
	// display text in the proper richTextBox
	pMiner->addSpeech(">> Walkin' to the goldmine\n");

    pMiner->ChangeLocation(goldmine);
  }
}


void EnterMineAndDigForNugget::Execute(Miner* pMiner)
{  
  //Now the miner is at the goldmine he digs for gold until he
  //is carrying in excess of MaxNuggets. If he gets thirsty during
  //his digging he packs up work for a while and changes state to
  //gp to the saloon for a whiskey.
  pMiner->AddToGoldCarried(1);

  pMiner->IncreaseFatigue();

  // display text in the proper richTextBox
  pMiner->addSpeech(">> Pickin' up a nugget\n");

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	// display text in the proper richTextBox
	pMiner->addSpeech(">> ah'm leavin' the goldmine with mah pockets full o' sweet gold\n");
}


bool EnterMineAndDigForNugget::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------methods for VisitBankAndDepositGold

VisitBankAndDepositGold* VisitBankAndDepositGold::Instance()
{
  static VisitBankAndDepositGold instance;

  return &instance;
}

void VisitBankAndDepositGold::Enter(Miner* pMiner)
{  
  //on entry the miner makes sure he is located at the bank
  if (pMiner->Location() != bank)
  {
	// display text in the proper richTextBox
	pMiner->addSpeech(">> Goin' to the bank. Yes siree\n");

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  pMiner->addSpeech(">> Depositing gold. Total savings now: ");
  pMiner->addSpeech(std::to_string(pMiner->Wealth()));
  pMiner->addSpeech("\n");

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
	pMiner->addSpeech(">> WooHoo! Rich enough for now. Back home to mah li'lle lady\n");
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());  
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  pMiner->addSpeech(">> Leavin' the bank\n");
}


bool VisitBankAndDepositGold::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}
//------------------------------------------------------------------------methods for GoHomeAndSleepTilRested

GoHomeAndSleepTilRested* GoHomeAndSleepTilRested::Instance()
{
  static GoHomeAndSleepTilRested instance;

  return &instance;
}

void GoHomeAndSleepTilRested::Enter(Miner* pMiner)
{
  if (pMiner->Location() != shack)
  {
	pMiner->addSpeech(">> Walkin' home\n");

    pMiner->ChangeLocation(shack);


    //let the wife know I'm home
    Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_Elsa,            //ID of recipient
                              Msg_HiHoneyImHome,   //the message
                              NO_ADDITIONAL_INFO); 
  }
}

void GoHomeAndSleepTilRested::Execute(Miner* pMiner)
{ 
  //if miner is not fatigued start to dig for nuggets again.
  if (!pMiner->Fatigued())
  {
	 pMiner->addSpeech(">> All mah fatigue has drained away. Time to find more gold!\n");

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

	pMiner->addSpeech(">> ZZZZ...\n");
  } 
}

void GoHomeAndSleepTilRested::Exit(Miner* pMiner)
{ 
}


bool GoHomeAndSleepTilRested::OnMessage(Miner* pMiner, const Telegram& msg)
{
   SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_StewReady:

     cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	 pMiner->addSpeech(">> Okay Hun, ahm a comin'!\n");

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
      
     return true;

   }//end switch

   return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
  static QuenchThirst instance;

  return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
  if (pMiner->Location() != saloon)
  {    
    pMiner->ChangeLocation(saloon);

	pMiner->addSpeech(">> Boy, ah sure is thusty! Walking to the saloon\n");
  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();
 
  pMiner->addSpeech(">> Un bon whiskey bien frais\n");

  if (!pMiner->Thirsty())
  {
	  pMiner->addSpeech(">> Go back to business !\n");

	  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
  }
}


void QuenchThirst::Exit(Miner* pMiner)
{
	
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler

	 SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

   switch(msg.Msg)
   {
   case Msg_Tournee:

	   if(pMiner->GoldCarried()<=1)
	   {

		   cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();
		
		   Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_ImNotRich,   //the message
                              NO_ADDITIONAL_INFO); 

		   SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

		   pMiner->addSpeech(">> I can't afford a round for everyone..\n");

		   pMiner->GetFSM()->ChangeState(FightForPride::Instance());

	   }
	   else
	   {

		   cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			   << " at time: " << Clock->GetCurrentTime();

		   Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_ImRich,   //the message
                              NO_ADDITIONAL_INFO); 

		   SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

		   pMiner->addSpeech(">> Well, let's have fun !\n");

		   pMiner->GetFSM()->ChangeState(RoundForEveryone::Instance());
	   }

	   return true;



   }

  return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
  static EatStew instance;

  return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
  pMiner->addSpeech(">> Smells Reaaal goood Elsa!\n");
}

void EatStew::Execute(Miner* pMiner)
{
  pMiner->addSpeech(">> Tastes real good too!\n");

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
	pMiner->addSpeech(">> Thankya li'lle lady. Ah better get back to whatever ah wuz doin'\n");
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------FightForPride

FightForPride* FightForPride::Instance()
{
  static FightForPride instance;

  return &instance;
}


void FightForPride::Enter(Miner* pMiner)
{
	// display text in the proper richTextBox
	pMiner->addSpeech(">> Let's Fight SelBastien !\n");
}

void FightForPride::Execute(Miner* pMiner)
{
  if(pMiner->FatiguedForFight()){
	  if(pMiner->GoldCarried()>=1)
	  {
		  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_LostEnought,   //the message
                              NO_ADDITIONAL_INFO);

		  // display text in the proper richTextBox
		  pMiner->addSpeech(">> I lost.. Well, I guess I have to pay this time !\n");

		  pMiner->GetFSM()->ChangeState(RoundForEveryone::Instance());
	  }
	  else
	  {
		  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_LostNotEnought,   //the message
                              NO_ADDITIONAL_INFO);

		  // display text in the proper richTextBox
		  pMiner->addSpeech(">> I lost.. And i need dollars.. Next time !\n");

		  pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
	  }
  }
  else
  {

	  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_Win,   //the message
                              NO_ADDITIONAL_INFO); 

	  // display text in the proper richTextBox
	  pMiner->addSpeech(">> I win!\n");

	  pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  }
}

void FightForPride::Exit(Miner* pMiner)
{ 
}


bool FightForPride::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}

//------------------------------------------------------------------------FightForPride

RoundForEveryone* RoundForEveryone::Instance()
{
  static RoundForEveryone instance;

  return &instance;
}


void RoundForEveryone::Enter(Miner* pMiner)
{
	// display text in the proper richTextBox
	pMiner->addSpeech(">> Round for everyone! Enjoy fellows!\n");
}

void RoundForEveryone::Execute(Miner* pMiner)
{
  pMiner->addSpeech(">> That round taste really good.\n");

  pMiner->BuyARound();

  pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
}

void RoundForEveryone::Exit(Miner* pMiner)
{
  pMiner->addSpeech(">> Back to work.\n");
}

bool RoundForEveryone::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}


