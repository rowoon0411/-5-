#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ARotatingCube.generated.h"

UCLASS()
class WHYSOSERIOUS_API ARotatingCube : public AActor
{
	GENERATED_BODY()

public:
	ARotatingCube();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComp;

	FVector StartLocation;

	bool bMovingUp;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Moving")
	float MoveDistance;
};