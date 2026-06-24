#include "ASHCharacter.h"
#include "ASHPlayerController.h"
#include "ASHGameState.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"

AASHCharacter::AASHCharacter()
{
		PrimaryActorTick.bCanEverTick = false;
	
		SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
		SpringArmComp->SetupAttachment(RootComponent);
		SpringArmComp->TargetArmLength = 300.0f;
		SpringArmComp->bUsePawnControlRotation = true;
	
		CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
		CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
		CameraComp->bUsePawnControlRotation = false;
	
		OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
		OverheadWidget->SetupAttachment(GetMesh());
		OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);
	
		NormalSpeed = 600.0f;
		SprintSpeedMultiplier = 1.7f;
		SprintSpeed = NormalSpeed * SprintSpeedMultiplier;
	
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	
		MaxHealth = 100.0f;
		Health = MaxHealth;
}

void AASHCharacter::BeginPlay()
{
		Super::BeginPlay();
		UpdateOverheadHP();
}

void AASHCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
		Super::SetupPlayerInputComponent(PlayerInputComponent);
	
		if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
				if (AASHPlayerController* PlayerController = Cast<AASHPlayerController>(GetController()))
				{
						if (PlayerController->MoveAction)
						{
								EnhancedInput->BindAction(
										PlayerController->MoveAction,
										ETriggerEvent::Triggered,
										this,
										&AASHCharacter::Move
								);
						}
			
						if (PlayerController->JumpAction)
						{
								EnhancedInput->BindAction(
										PlayerController->JumpAction,
										ETriggerEvent::Triggered,
										this,
										&AASHCharacter::StartJump
								);
				
								EnhancedInput->BindAction(
										PlayerController->JumpAction,
										ETriggerEvent::Completed,
										this,
										&AASHCharacter::StopJump
								);
						}
			
						if (PlayerController->LookAction)
						{
								EnhancedInput->BindAction(
										PlayerController->LookAction,
										ETriggerEvent::Triggered,
										this,
										&AASHCharacter::Look
								);
						}
			
						if (PlayerController->SprintAction)
						{
								EnhancedInput->BindAction(
										PlayerController->SprintAction,
										ETriggerEvent::Triggered,
										this,
										&AASHCharacter::StartSprint
								);
				
								EnhancedInput->BindAction(
										PlayerController->SprintAction,
										ETriggerEvent::Completed,
										this,
										&AASHCharacter::StopSprint
								);
						}
				}
		}
}

void AASHCharacter::Move(const FInputActionValue& value)
{
		if (!Controller) return;
	
		const FVector2D MoveInput = value.Get<FVector2D>();
	
		if (!FMath::IsNearlyZero(MoveInput.X))
		{
				AddMovementInput(GetActorForwardVector(), MoveInput.X);
		}
	
		if (!FMath::IsNearlyZero(MoveInput.Y))
		{
				AddMovementInput(GetActorRightVector(), MoveInput.Y);
		}
}

void AASHCharacter::StartJump(const FInputActionValue& value)
{
		if (value.Get<bool>())
		{
			Jump();
		}
}

void AASHCharacter::StopJump(const FInputActionValue& value)
{
		if (!value.Get<bool>())
		{
			StopJumping();
		}
}

void AASHCharacter::Look(const FInputActionValue& value)
{
		FVector2D LookInput = value.Get<FVector2D>();
	
		AddControllerYawInput(LookInput.X);
		AddControllerPitchInput(LookInput.Y);
}

void AASHCharacter::StartSprint(const FInputActionValue& value)
{
		if (GetCharacterMovement())
		{
				GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
		}
}

void AASHCharacter::StopSprint(const FInputActionValue& value)
{
		if (GetCharacterMovement())
		{
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
		}
}

float AASHCharacter::GetHealth() const
{
		return Health;
}

void AASHCharacter::AddHealth(float Amount)
{
		Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
		UpdateOverheadHP();
}

float AASHCharacter::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
		float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
		Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
		UpdateOverheadHP();
	
		if (Health <= 0.0f)
		{
			OnDeath();
		}
	
		return ActualDamage;
}

void AASHCharacter::OnDeath()
{
		if (AASHPlayerController* ASHPlayerController = Cast<AASHPlayerController>(GetController()))
		{
				ASHPlayerController->ShowGameOverMenu();
				return;
		}

		AASHGameState* ASHGameState = GetWorld() ? GetWorld()->GetGameState<AASHGameState>() : nullptr;
		if (ASHGameState)
		{
				ASHGameState->OnGameOver();
		}
}

void AASHCharacter::UpdateOverheadHP()
{
		if (!OverheadWidget) return;
	
		UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
		if (!OverheadWidgetInstance) return;
	
		if (UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
		{
			HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
		}
}
