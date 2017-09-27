#include "Drunk.h"

bool Drunk::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}

void Drunk::Update()
{
  SetTextColor(FOREGROUND_RED| FOREGROUND_INTENSITY);
  
  m_pStateMachine->Update();
}

void Drunk::ChangeState(State<Drunk>* state)
{
	m_pStateMachine->SetCurrentState(state);
}
