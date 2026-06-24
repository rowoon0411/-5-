#include "ASHGameState.h"
#include "ASHGameInstance.h"
#include "ASHPlayerController.h"
#include "SpawnVolume.h"
#include "coinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AASHGameState::AASHGameState()
{
		Score = 0;
		SpawnedCoinCount = 0;
		CollectedCoinCount = 0;
		LevelDuration = 30.0f;
		CurrentLevelIndex = 0;
		MaxLevels = 3;
}

void AASHGameState::BeginPlay()
{
		Super::BeginPlay();
	
		StartLevel();
	
		GetWorldTimerManager().SetTimer(
				HUDUpdateTimerHandle,
				this,
				&AASHGameState::UpdateHUD,
				0.1f,
				true
		);
}

int32 AASHGameState::GetScore() const
{
		return Score;
}

void AASHGameState::AddScore(int32 Amount)
{
		if (UGameInstance* GameInstance = GetGameInstance())
		{
				UASHGameInstance* ASHGameInstance = Cast<UASHGameInstance>(GameInstance);
				if (ASHGameInstance)
				{
						ASHGameInstance->AddToScore(Amount);
				}
		}
}

void AASHGameState::StartLevel()
{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
				if (AASHPlayerController* ASHPlayerController = Cast<AASHPlayerController>(PlayerController))
				{
						ASHPlayerController->ShowGameHUD();
				}
		}
	
		if (UGameInstance* GameInstance = GetGameInstance())
		{
				UASHGameInstance* ASHGameInstance = Cast<UASHGameInstance>(GameInstance);
				if (ASHGameInstance)
				{
						CurrentLevelIndex = ASHGameInstance->CurrentLevelIndex;
				}
		}
	
		SpawnedCoinCount = 0;
		CollectedCoinCount = 0;
	
		TArray<AActor*> FoundVolumes;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	
		const int32 ItemToSpawn = 40;
	
		if (FoundVolumes.Num() > 0)
		{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
				if (SpawnVolume)
				{
						for (int32 i = 0; i < ItemToSpawn; i++)
						{
								AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
								if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
								{
										SpawnedCoinCount++;
								}
						}
				}
		}
	
		GetWorldTimerManager().SetTimer(
			LevelTimerHandle,
			this,
			&AASHGameState::OnLevelTimeUp,
			LevelDuration,
			false
		);
}

void AASHGameState::OnLevelTimeUp()
{
		EndLevel();
}

void AASHGameState::OnCoinCollected()
{
		CollectedCoinCount++;
		
		if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
				EndLevel();
		}
}

void AASHGameState::EndLevel()
{
		GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	
		if (UGameInstance* GameInstance = GetGameInstance())
		{
				UASHGameInstance* ASHGameInstance = Cast<UASHGameInstance>(GameInstance);
				if (ASHGameInstance)
				{
						AddScore(Score);
						CurrentLevelIndex++;
						ASHGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			
						if (CurrentLevelIndex >= MaxLevels)
						{
								OnGameOver();
								return;
						}
			
						if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
						{
								UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
						}
						else
						{
								OnGameOver();
						}
				}
		}
}

void AASHGameState::OnGameOver()
{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
				if (AASHPlayerController* ASHPlayerController = Cast<AASHPlayerController>(PlayerController))
				{
						ASHPlayerController->ShowGameOverMenu();
				}
		}
}

void AASHGameState::UpdateHUD()
{
		if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
		{
				AASHPlayerController* ASHPlayerController = Cast<AASHPlayerController>(PlayerController);
				{
						if (UUserWidget* HUDWidget = ASHPlayerController->GetHUDWidget())
						{
								if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
								{
										float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
										TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
								}
				
								if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
								{
										if (UGameInstance* GameInstance = GetGameInstance())
										{
												UASHGameInstance* ASHGameInstance = Cast<UASHGameInstance>(GameInstance);
												if (ASHGameInstance)
												{
														ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), ASHGameInstance->TotalScore)));
												}
										}
								}
				
								if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
								{
										LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex + 1)));
								}
						}
				}
		}
}
