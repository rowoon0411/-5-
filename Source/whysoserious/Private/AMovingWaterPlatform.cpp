#include "AMovingWaterPlatform.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "UObject/ConstructorHelpers.h"

AMovingWaterPlatform::AMovingWaterPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/Resources/Props/Floor_400x400.Floor_400x400")
	);

	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("/Game/Resources/Materials/M_Gem_D.M_Gem_D")
	);

	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}

	MoveSpeed = 200.f;
	MoveDistance = 800.f;

	bMovingForward = true;
}

void AMovingWaterPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void AMovingWaterPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector NewLocation = GetActorLocation();

	if (bMovingForward)
	{
		NewLocation.X += MoveSpeed * DeltaTime;

		if (NewLocation.X >= StartLocation.X + MoveDistance)
		{
			NewLocation.X = StartLocation.X + MoveDistance;
			bMovingForward = false;
		}
	}
	else
	{
		NewLocation.X -= MoveSpeed * DeltaTime;

		if (NewLocation.X <= StartLocation.X)
		{
			NewLocation.X = StartLocation.X;
			bMovingForward = true;
		}
	}

	// X축만 움직이고 나머지는 고정
	NewLocation.Y = StartLocation.Y;
	NewLocation.Z = StartLocation.Z;

	SetActorLocation(NewLocation);
}