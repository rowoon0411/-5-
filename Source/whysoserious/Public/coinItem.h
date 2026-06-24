#pragma once

#include "CoreMinimal.h"
#include "Baseltem.h"
#include "coinItem.generated.h"

UCLASS()
class WHYSOSERIOUS_API ACoinItem : public ABaseItem
{
	GENERATED_BODY()

public:
	ACoinItem();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 PointValue;

	virtual void ActivateItem(AActor* Activator) override;
};
