#pragma once

#include "CoreMinimal.h"
#include "Baseltem.h"
#include "HealingItem.generated.h"

UCLASS()
class WHYSOSERIOUS_API AHealingItem : public ABaseItem
{
	GENERATED_BODY()

public:
	AHealingItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Healing")
	int32 HealAmount;

	virtual void ActivateItem(AActor* Activator) override;
};
