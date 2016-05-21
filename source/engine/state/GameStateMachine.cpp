#include "GameStateMachine.h"

GameStateMachine::GameStateMachine(): m_Stack(kGameStateMax) {

}

GameStateMachine::~GameStateMachine() {
	while (!m_Stack.IsEmpty()) {
		m_Stack.Pop();
	}
}

void GameStateMachine::Update() {
	if (!m_Stack.IsEmpty()) {
		SharedPtr<BaseGameState> state = m_Stack.GetFront();

		state->Update();
	}
}

void GameStateMachine::ChangeState(SharedPtr<BaseGameState> state) {
	ASSERT(!m_Stack.IsEmpty());

	SharedPtr<BaseGameState> prev = m_Stack.GetFront();

	prev->PostState();

	state->PreState();

	m_Stack.Pop();

	m_Stack.Push(state);
}

void GameStateMachine::PushState(SharedPtr<BaseGameState> state) {
	ASSERT(!m_Stack.IsFull());

	if (!m_Stack.IsEmpty()) {
		SharedPtr<BaseGameState> prev = m_Stack.GetFront();

		prev->Pause();
	}

	state->PreState();

	m_Stack.Push(state);
}

void GameStateMachine::PopState() {
	ASSERT(!m_Stack.IsEmpty());

	SharedPtr<BaseGameState> state = m_Stack.GetFront();

	state->PostState();

	m_Stack.Pop();

	if (!m_Stack.IsEmpty()) {
		SharedPtr<BaseGameState> prev = m_Stack.GetFront();

		prev->Resume();
	}
}

BaseGameState* GameStateMachine::GetTopState() {
	if (m_Stack.IsEmpty()) {
		return nullptr;
	}

	return m_Stack.GetFront().get();
}