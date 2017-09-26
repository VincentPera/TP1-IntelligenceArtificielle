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
	MyForm1::getTextBob()->Text += ">> Walkin' to the goldmine";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();

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
  MyForm1::getTextBob()->Text += ">> Pickin' up a nugget";
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();

  //if enough gold mined, go and put it in the bank
  if (pMiner->PocketsFull())
  {
    pMiner->GetFSM()->ChangeState(VisitBankAndDepositGold::Instance());
	MyForm1::changeStateBob("VisitBankAndDepositGold");
  }

  if (pMiner->Thirsty())
  {
    pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	MyForm1::changeStateBob("QuenchThirst");
  }
}


void EnterMineAndDigForNugget::Exit(Miner* pMiner)
{
	// display text in the proper richTextBox
	MyForm1::getTextBob()->Text += ">> Ah'm leavin' the goldmine with mah pockets full o' sweet gold";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
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
	MyForm1::getTextBob()->Text += ">> Goin' to the bank. Yes siree";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();

    pMiner->ChangeLocation(bank);
  }
}


void VisitBankAndDepositGold::Execute(Miner* pMiner)
{
  //deposit the gold
  pMiner->AddToWealth(pMiner->GoldCarried());
    
  pMiner->SetGoldCarried(0);

  MyForm1::getTextBob()->Text += ">> Depositing gold. Total savings now:";
  MyForm1::getTextBob()->Text += System::Convert::ToString(pMiner->GoldCarried());
  MyForm1::getTextBob()->Text += " ";
  MyForm1::getTextBob()->Text += System::Convert::ToString(pMiner->Wealth());
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();

  //wealthy enough to have a well earned rest?
  if (pMiner->Wealth() >= ComfortLevel)
  {
	MyForm1::getTextBob()->Text += ">> WooHoo! Rich enough for now. Back home to mah li'lle lady";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
      
    pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());  
	MyForm1::changeStateBob("GoHomeAndSleepTilRested");
  }

  //otherwise get more gold
  else 
  {
    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	MyForm1::changeStateBob("EnterMineAndDigForNugget");
  }
}


void VisitBankAndDepositGold::Exit(Miner* pMiner)
{
  MyForm1::getTextBob()->Text += ">> Leavin' the bank";
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();
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
	MyForm1::getTextBob()->Text += ">> Walkin' home";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();

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
	 MyForm1::getTextBob()->Text += ">> All mah fatigue has drained away. Time to find more gold!";
	 MyForm1::getTextBob()->Text += "\n";
	 MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	 MyForm1::getTextBob()->ScrollToCaret();

     pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	 MyForm1::changeStateBob("EnterMineAndDigForNugget");
  }

  else 
  {
    //sleep
    pMiner->DecreaseFatigue();

	MyForm1::getTextBob()->Text += ">> ZZZZ...";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
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

	 MyForm1::getTextBob()->Text += ">> Okay Hun, ahm a comin'!";
	 MyForm1::getTextBob()->Text += "\n";
	 MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	 MyForm1::getTextBob()->ScrollToCaret();

     pMiner->GetFSM()->ChangeState(EatStew::Instance());
	 MyForm1::changeStateBob("EatStew");
      
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

	MyForm1::getTextBob()->Text += ">> Boy, ah sure is thusty! Walking to the saloon";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();

  }
}

void QuenchThirst::Execute(Miner* pMiner)
{
  pMiner->BuyAndDrinkAWhiskey();
 
  MyForm1::getTextBob()->Text += ">> Un bon whiskey bien frais ";
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();


  if (!pMiner->Thirsty())
  {
	  MyForm1::getTextBob()->Text += ">> Go back to work !";
	  MyForm1::getTextBob()->Text += "\n";
	  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	  MyForm1::getTextBob()->ScrollToCaret();

	  pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
	  MyForm1::changeStateBob("EnterMineAndDigForNugget");
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

	   if(pMiner->GoldCarried()<=1){

		   cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID()) 
     << " at time: " << Clock->GetCurrentTime();
		
		   Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_ImNotRich,   //the message
                              NO_ADDITIONAL_INFO); 

		   SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	 MyForm1::getTextBob()->Text += ">> I can't afford a round for everyone.. ";
	 MyForm1::getTextBob()->Text += "\n";
	 MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	 MyForm1::getTextBob()->ScrollToCaret();

	 pMiner->GetFSM()->ChangeState(FightForPride::Instance());
	 MyForm1::changeStateBob("FightForPride");

	   }else{

		   cout << "\nMessage handled by " << GetNameOfEntity(pMiner->ID())
			   << " at time: " << Clock->GetCurrentTime();

		   Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_ImRich,   //the message
                              NO_ADDITIONAL_INFO); 

		   SetTextColor(FOREGROUND_RED|FOREGROUND_INTENSITY);

	 MyForm1::getTextBob()->Text += ">> Well, let's have fun !";
	 MyForm1::getTextBob()->Text += "\n";
	 MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	 MyForm1::getTextBob()->ScrollToCaret();

	 pMiner->GetFSM()->ChangeState(RoundForEveryone::Instance());
	 MyForm1::changeStateBob("RoundForEveryone");
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
  MyForm1::getTextBob()->Text += ">> Smells Reaaal goood Elsa!";
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();
}

void EatStew::Execute(Miner* pMiner)
{
  MyForm1::getTextBob()->Text += ">> Tastes real good too!";
  MyForm1::getTextBob()->Text += "\n";
  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
  MyForm1::getTextBob()->ScrollToCaret();

  pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{ 
	MyForm1::getTextBob()->Text += ">> Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
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
	MyForm1::getTextBob()->Text += ">> Let's Fight SelBastien !";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
}

void FightForPride::Execute(Miner* pMiner)
{
  if(pMiner->FatiguedForFight()){
	  if(pMiner->GoldCarried()>1){
		
		  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_LostEnought,   //the message
                              NO_ADDITIONAL_INFO);

		  // display text in the proper richTextBox
		  MyForm1::getTextBob()->Text += ">> I lost.. Well, I guess I have to pay this time !";
		  MyForm1::getTextBob()->Text += "\n";
		  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
		  MyForm1::getTextBob()->ScrollToCaret();

		  pMiner->GetFSM()->ChangeState(RoundForEveryone::Instance());
		  MyForm1::changeStateBob("RoundForEveryone");

	  }else{

		  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_LostNotEnought,   //the message
                              NO_ADDITIONAL_INFO);

		  // display text in the proper richTextBox
		  MyForm1::getTextBob()->Text += ">> I lost.. And i need dollars.. Next time !";
		  MyForm1::getTextBob()->Text += "\n";
		  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
		  MyForm1::getTextBob()->ScrollToCaret();

		  pMiner->GetFSM()->ChangeState(GoHomeAndSleepTilRested::Instance());
		  MyForm1::changeStateBob("GoHomeAndSleepTilRested");
	  }
  }else{

	  Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY, //time delay
                              pMiner->ID(),        //ID of sender
                              ent_SeLbastien,            //ID of recipient
                              Msg_Win,   //the message
                              NO_ADDITIONAL_INFO); 

	  // display text in the proper richTextBox
	  MyForm1::getTextBob()->Text += ">> I win!";
	  MyForm1::getTextBob()->Text += "\n";
	  MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	  MyForm1::getTextBob()->ScrollToCaret();

	  pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
	  MyForm1::changeStateBob("QuenchThirst");
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
	MyForm1::getTextBob()->Text += ">> Round for everyone! Enjoy fellows!";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
}

void RoundForEveryone::Execute(Miner* pMiner)
{
	// display text in the proper richTextBox
	MyForm1::getTextBob()->Text += ">> That round taste really good.";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();

  pMiner->BuyARound();

  pMiner->GetFSM()->ChangeState(QuenchThirst::Instance());
  MyForm1::changeStateBob("QuenchThirst");
}

void RoundForEveryone::Exit(Miner* pMiner)
{
	// display text in the proper richTextBox
	MyForm1::getTextBob()->Text += ">> Back to work";
	MyForm1::getTextBob()->Text += "\n";
	MyForm1::getTextBob()->SelectionStart = MyForm1::getTextBob()->Text->Length;
	MyForm1::getTextBob()->ScrollToCaret();
}

bool RoundForEveryone::OnMessage(Miner* pMiner, const Telegram& msg)
{
  //send msg to global message handler
  return false;
}


