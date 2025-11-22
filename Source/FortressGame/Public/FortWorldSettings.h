#pragma once

#include "CoreMinimal.h"
#include "GameFramework/WorldSettings.h"
#include "FortTimeOfDayManager.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/UnrealEd/Public/EditorActorFolders.h"
#endif

#include "FortWorldSettings.generated.h"

/**
 *
 */
UCLASS()
class FORTRESSGAME_API AFortWorldSettings : public AWorldSettings
{
	GENERATED_BODY()

public:
	// Show Time Of Day Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day")
	bool bShowTimeOfDayManager = false;

	// Editable Time Of Day Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day")
	bool bEditableTimeOfDayManager = false;

	// Spawn Time Of Day Manager
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day")
	bool bSpawnTimeOfDayManager = true;

	// Disable All Time Of Day Managers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day")
	bool bDisableAllTimeOfDayManagers = false;

	// World Time Of Day Manager (Blueprint Class Selector)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day", meta = (EditCondition = "bEditableTimeOfDayManager", EditConditionHides))
	TSubclassOf<AFortTimeOfDayManager> WorldTimeOfDayManagerBlueprint;

	// World Time Of Day Manager Rotation Z
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day")
	float WorldTimeOfDayManagerRotationZ = 0.0f;

	// Time Of Day Editor Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day Editor Settings")
	FString TimeOfDayEditorSettings;

	// Time Of Day Editor Weather Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Time Of Day Editor Weather Settings")
	FString TimeOfDayEditorWeatherSettings;

	// Function to spawn the World Time Of Day Manager
	UFUNCTION(BlueprintCallable, Category = "Time Of Day")
	void SpawnWorldTimeOfDayManager()
	{
		if (WorldTimeOfDayManagerBlueprint && bSpawnTimeOfDayManager)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FRotator SpawnRotation(0.0f, WorldTimeOfDayManagerRotationZ, 0.0f);
			FVector SpawnLocation(0.0f, 0.0f, 0.0f);

			UWorld* World = GetWorld();
			if (World)
			{
#if WITH_EDITOR
				if (World->WorldType == EWorldType::Editor)
				{
					if (GEditor)
					{
						FEditorScriptExecutionGuard ScriptGuard;
						AActor* SpawnedActor = GEditor->AddActor(
							World->GetCurrentLevel(),
							WorldTimeOfDayManagerBlueprint,
							FTransform(SpawnRotation, SpawnLocation)
						);
						if (SpawnedActor)
						{
							UE_LOG(LogTemp, Log, TEXT("Successfully spawned AFortTimeOfDayManager in the editor."));
						}
						else
						{
							UE_LOG(LogTemp, Error, TEXT("Failed to spawn AFortTimeOfDayManager in the editor."));
						}
					}
				}
				else
#endif
				{
					AFortTimeOfDayManager* SpawnedActor = World->SpawnActor<AFortTimeOfDayManager>(
						WorldTimeOfDayManagerBlueprint,
						SpawnLocation,
						SpawnRotation,
						SpawnParams
					);
					if (SpawnedActor)
					{
						UE_LOG(LogTemp, Log, TEXT("Successfully spawned AFortTimeOfDayManager during gameplay."));
					}
					else
					{
						UE_LOG(LogTemp, Error, TEXT("Failed to spawn AFortTimeOfDayManager during gameplay."));
					}
				}
			}
		}
	}

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
	{
		Super::PostEditChangeProperty(PropertyChangedEvent);
		if (PropertyChangedEvent.Property &&
			(PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFortWorldSettings, WorldTimeOfDayManagerBlueprint) ||
				PropertyChangedEvent.Property->GetFName() == GET_MEMBER_NAME_CHECKED(AFortWorldSettings, bSpawnTimeOfDayManager)))
		{
			UE_LOG(LogTemp, Log, TEXT("Property changed: Spawning AFortTimeOfDayManager."));
			SpawnWorldTimeOfDayManager();
		}
	}
#endif

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override
	{
		Super::BeginPlay();
		SpawnWorldTimeOfDayManager();
	}
};
