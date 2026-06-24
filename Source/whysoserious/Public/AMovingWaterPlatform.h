#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AMovingWaterPlatform.generated.h"

UCLASS()
class WHYSOSERIOUS_API AMovingWaterPlatform : public AActor
{
	GENERATED_BODY()

public:
	AMovingWaterPlatform();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* StaticMeshComp;

	FVector StartLocation;

	bool bMovingForward;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float MoveDistance;
};