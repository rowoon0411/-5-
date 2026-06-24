#pragma once

#include "CoreMinimal.h"
#include "coinItem.h"
#include "smallCoinItem.generated.h"

UCLASS()
class WHYSOSERIOUS_API ASmallCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	ASmallCoinItem();

	virtual void ActivateItem(AActor* Activator) override;
};
