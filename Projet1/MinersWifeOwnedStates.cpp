#include "MinersWifeOwnedStates.h"
#include "MinerOwnedStates.h"
#include "MinersWife.h"
#include "Locations.h"
#include "CrudeTimer.h"
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

//-----------------------------------------------------------------------Global state

WifesGlobalState* WifesGlobalState::Instance()
{
  static WifesGlobalState instance;

  return &instance;
}


void WifesGlobalState::Execute(MinersWife* wife)
{
  //1 in 10 chance of needing the bathroom (provided she is not already
  //in the bathroom)
  if ( (RandFloat() < 0.1) && 
       !wife->GetFSM()->isInState(*VisitBathroom::Instance()) )
  {
    wife->GetFSM()->ChangeState(VisitBathroom::Instance());
  }
}

bool WifesGlobalState::OnMessage(MinersWife* wife, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
  case Msg_HiHoneyImHome:
   {
     cout << "\nMessage handled by " << GetNameOfEntity(wife->ID()) << " at time: " << Clock->GetCurrentTime();

     SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

	 /*MyForm1::getTextElsa()->Text += ">> Hi honey. Let me make you some of mah fine country stew";
	 MyForm1::getTextElsa()->Text += "\n";
	 MyForm1::getTextElsa()->SelectionStart = MyForm1::getTextElsa()->Text->Length;
	 MyForm1::getTextElsa()->ScrollToCaret();*/

     wife->GetFSM()->ChangeState(CookStew::Instance());
   }

   return true;

  }//end switch

  return false;
}

//-------------------------------------------------------------------------DoHouseWork

DoHouseWork* DoHouseWork::Instance()
{
  static DoHouseWork instance;

  return &instance;
}


void DoHouseWork::Enter(MinersWife* wife)
{
  wife->addSpeech(">> Time to do some more housework!\n");
}


void DoHouseWork::Execute(MinersWife* wife)
{
  switch(RandInt(0,2))
  {
  case 0:

	wife->addSpeech(">> Moppin' the floor\n");

    break;

  case 1:

	wife->addSpeech(">> Washin' the dishes\n");

    break;

  case 2:

	wife->addSpeech(">> Makin' the bed\n");

    break;
  }
}

void DoHouseWork::Exit(MinersWife* wife)
{
}

bool DoHouseWork::OnMessage(MinersWife* wife, const Telegram& msg)
{
  return false;
}

//------------------------------------------------------------------------VisitBathroom

VisitBathroom* VisitBathroom::Instance()
{
  static VisitBathroom instance;

  return &instance;
}


void VisitBathroom::Enter(MinersWife* wife)
{
  wife->addSpeech(">> Walkin' to the can. Need to powda mah pretty li'lle nose\n");
}


void VisitBathroom::Execute(MinersWife* wife)
{
  wife->addSpeech(">> Ahhhhhh! Sweet relief!\n");

  wife->GetFSM()->RevertToPreviousState();
}

void VisitBathroom::Exit(MinersWife* wife)
{
  wife->addSpeech(">> Leavin' the Jon\n");
}


bool VisitBathroom::OnMessage(MinersWife* wife, const Telegram& msg)
{
  return false;
}


//------------------------------------------------------------------------CookStew

CookStew* CookStew::Instance()
{
  static CookStew instance;

  return &instance;
}


void CookStew::Enter(MinersWife* wife)
{
  //if not already cooking put the stew in the oven
  if (!wife->Cooking())
  {
	wife->addSpeech(">> Putting the stew in the oven\n");
  
    //send a delayed message myself so that I know when to take the stew
    //out of the oven
    Dispatch->DispatchMessage(1.5,                  //time delay
                              wife->ID(),           //sender ID
                              wife->ID(),           //receiver ID
                              Msg_StewReady,        //msg
                              NO_ADDITIONAL_INFO); 

    wife->SetCooking(true);
  }
}


void CookStew::Execute(MinersWife* wife)
{
  wife->addSpeech(">> Fussin' over food\n");
}

void CookStew::Exit(MinersWife* wife)
{
  SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);
  
  wife->addSpeech(">> Puttin' the stew on the table\n");
}


bool CookStew::OnMessage(MinersWife* wife, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  switch(msg.Msg)
  {
    case Msg_StewReady:
    {
      cout << "\nMessage received by " << GetNameOfEntity(wife->ID()) <<
           " at time: " << Clock->GetCurrentTime();

      SetTextColor(FOREGROUND_GREEN|FOREGROUND_INTENSITY);

	  wife->addSpeech(">> StewReady! Lets eat\n");

      //let hubby know the stew is ready
      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
                                wife->ID(),
                                ent_Miner_Bob,
                                Msg_StewReady,
                                NO_ADDITIONAL_INFO);

      wife->SetCooking(false);

      wife->GetFSM()->ChangeState(DoHouseWork::Instance()); 
    }

    return true;

  }//end switch

  return false;
}