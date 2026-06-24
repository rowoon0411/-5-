#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ASHGameInstance.generated.h"

UCLASS()
class WHYSOSERIOUS_API UASHGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UASHGameInstance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Score")
	int32 TotalScore;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 CurrentLevelIndex;

	UFUNCTION(BlueprintCallable)
	void AddToScore(int32 Amount);
};