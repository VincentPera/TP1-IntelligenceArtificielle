#ifndef MESSAGE_TYPES
#define MESSAGE_TYPES

#include <string>

enum message_type
{
  Msg_HiHoneyImHome,
  Msg_StewReady,
  Msg_Tournee,
  Msg_ImRich,
  Msg_ImNotRich,
  Msg_LostEnought,
  Msg_LostNotEnought,
  Msg_Win
};


inline std::string MsgToStr(int msg)
{
  switch (msg)
  {
  case Msg_HiHoneyImHome:
    
    return "HiHoneyImHome"; 

  case Msg_StewReady:
    
    return "StewReady";

  case Msg_Tournee:

	  return "Tournee";

  case Msg_ImRich:

	  return "ImRich";

  case Msg_ImNotRich:

	  return "ImNotRich";

  case Msg_LostEnought:

	  return "LostEnought";

  case Msg_LostNotEnought:

	  return "LostNotEnought";

  case Msg_Win:

	  return "Win";

  default:

    return "Not recognized!";
  }
}

#endif