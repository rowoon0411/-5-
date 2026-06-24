#include "ARotatingCube.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Materials/MaterialInterface.h"

ARotatingCube::ARotatingCube()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(
		TEXT("/Game/Resources/Props/Floor_400x400.Floor_400x400"));

	if (MeshAsset.Succeeded())
	{
		StaticMeshComp->SetStaticMesh(MeshAsset.Object);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialAsset(
		TEXT("/Game/Resources/Materials/M_Potion_Elixir.M_Potion_Elixir"));

	if (MaterialAsset.Succeeded())
	{
		StaticMeshComp->SetMaterial(0, MaterialAsset.Object);
	}

	MoveSpeed = 200.f;
	MoveDistance = 300.f;

	bMovingUp = true;
}

void ARotatingCube::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

void ARotatingCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CurrentLocation = GetActorLocation();

	if (bMovingUp)
	{
		CurrentLocation.Z += MoveSpeed * DeltaTime;

		if (CurrentLocation.Z >= StartLocation.Z + MoveDistance)
		{
			CurrentLocation.Z = StartLocation.Z + MoveDistance;
			bMovingUp = false;
		}
	}
	else
	{
		CurrentLocation.Z -= MoveSpeed * DeltaTime;

		if (CurrentLocation.Z <= StartLocation.Z)
		{
			CurrentLocation.Z = StartLocation.Z;
			bMovingUp = true;
		}
	}

	SetActorLocation(CurrentLocation);
}