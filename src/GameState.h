#ifndef _GAMESTATE_
#define _GAMESTATE_

#include <iostream>
#include <string.h>
#include "SpriteFactory.h"
#include <memory>

/// forward declaration of GameStateManager
class GamestateManager;

///
/// This is a virtual class, for all states to inherit from. The gamestate manager will have a current gameState to call the functions from
///
class GameState {

public:
	GameState();

	///need this here to ensure inheriting class destructors will call properly
	virtual ~GameState() {}

	///any inheriting classes must provide these
	virtual bool HandleSDLEvents() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;


protected:

};

#endif