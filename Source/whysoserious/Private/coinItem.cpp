#include "coinItem.h"
#include "Engine/Engine.h"
#include "ASHGameInstance.h"

ACoinItem::ACoinItem()
{
	PointValue = 100;
	ItemType = TEXT("DefaultCoin");
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	if (Activator && Activator->ActorHasTag(TEXT("Player")))
	{
		UASHGameInstance* GameInstance =
			Cast<UASHGameInstance>(GetGameInstance());

		if (GameInstance)
		{
			GameInstance->AddToScore(PointValue);
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Green,
				FString::Printf(TEXT("Player gained %d points!"), PointValue)
			);
		}

		DestroyItem();
	}
}