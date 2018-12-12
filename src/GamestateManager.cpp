#include "GamestateManager.h"

//delete any states that are left in the list
GamestateManager::~GamestateManager() {
	//for (unsigned int i = 0; i < gameStates.size(); i++)
	//	delete gameStates.at(i);
	gameStates.clear();
}

//add a new state
void GamestateManager::AddState(GameState* gamestate) {
	gameStates.push_back(gamestate);
}


/* ChangeState - this will get called we want to add a
* state and know that it will be the only one in use */
void GamestateManager::ChangeState(GameState* gamestate) {

	std::cout << "GameStateManager: Removing all states from states list" << std::endl;

	//delete any states that are in the list
	//for (unsigned int i = 0; i < gameStates.size(); i++)
	//	delete gameStates.at(i);

	//clear list of states and add just the state we were passed
	gameStates.clear();
	AddState(gamestate);
}

/* RemoveLastState - this will get rid of just the state
* that is last in the list, useful for say removing the
* pause state from over the game 'action' state */
void GamestateManager::RemoveLastState() {

	//delete the last state object and then pop the now dead pointer out of the vector
	delete gameStates.back();
	gameStates.back() = nullptr;
	gameStates.pop_back();
}

/* HandleSDLEvents - only the top state should do this */
bool GamestateManager::HandleSDLEvents() {
	return gameStates.back()->HandleSDLEvents();
}

/* Update - only the top state should be updated */
void GamestateManager::Update() {
	gameStates.back()->Update();
}

/* Draw - every state should be drawn */
void GamestateManager::Draw() {
	for (unsigned int i = 0; i < gameStates.size(); i++)
		gameStates.at(i)->Draw();
}