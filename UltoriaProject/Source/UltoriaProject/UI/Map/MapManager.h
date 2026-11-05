// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapManager.generated.h"

class UMapWidgetTable;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMapManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() {};

	void OnPostLoadMapWithWorld(UWorld* world);

	void OnWorldInitializedActors(const UWorld::FActorsInitializedParams& Params);

	void DebugInit(UWorld* world);
private:
	UPROPERTY()
	TObjectPtr<UMapWidgetTable> m_MWT = nullptr;
};
