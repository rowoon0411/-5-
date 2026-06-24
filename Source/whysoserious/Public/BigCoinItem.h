#pragma once

#include "CoreMinimal.h"
#include "coinItem.h"
#include "BigCoinItem.generated.h"

UCLASS()
class WHYSOSERIOUS_API ABigCoinItem : public ACoinItem
{
	GENERATED_BODY()

public:
	ABigCoinItem();
	
	virtual void ActivateItem(AActor* Activator) override;
};
