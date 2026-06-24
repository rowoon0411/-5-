#include "ASHGameInstance.h"

UASHGameInstance::UASHGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
}

void UASHGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;

	UE_LOG(LogTemp, Warning,
		TEXT("Total Score Updated: %d"),
		TotalScore);
}