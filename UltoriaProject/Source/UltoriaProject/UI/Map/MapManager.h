// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MapManager.generated.h"

class UMapWidgetTable;
class UMiniMapWidget;
class UMapWidget;

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

	
public:
	// マップ作成関数
	void CreateMap(UWorld*);
	// ミニマップ作成関数
	void CreateMiniMap(UWorld*);

	// マップ表示/非表示関数
	void SetMapVisible(bool isVisible);
	// ミニマップ表示/非表示関数
	void SetMiniMapVisible(bool isVisible);
private:
	UPROPERTY()
	TObjectPtr<UMapWidgetTable> m_MWT = nullptr;

	// マップ
	UPROPERTY()
	UMapWidget* m_Map;

	UPROPERTY()
	// ミニマップ
	UMiniMapWidget* m_MiniMap;

	// マップ表示フラグ
	bool m_IsMapVisible;

	// ミニマップ表示フラグ
	bool m_IsMiniMapVisible;
};
