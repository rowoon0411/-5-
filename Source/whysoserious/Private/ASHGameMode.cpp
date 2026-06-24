#include "ASHGameMode.h"
#include "ASHCharacter.h"
#include "ASHGameState.h"
#include "ASHPlayerController.h"

AASHGameMode::AASHGameMode()
{
	DefaultPawnClass = AASHCharacter::StaticClass();
	GameStateClass = AASHGameState::StaticClass();
	PlayerControllerClass = AASHPlayerController::StaticClass();
}
