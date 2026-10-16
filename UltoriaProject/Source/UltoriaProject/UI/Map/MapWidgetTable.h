// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapWidgetTable.generated.h"

class UMapWidget;
class UMiniMapWidget;

UENUM(BlueprintType)
enum class EMapID :uint8
{
	CASRLE,
	ENTRANCE,
	TUTORIAL,
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UMapWidgetTable : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TMap<EMapID, TSubclassOf<UMapWidget>> m_MW;
	UPROPERTY(EditAnywhere)
	TMap<EMapID, TSubclassOf<UMiniMapWidget>> m_MMW;

	// ƒeƒNƒXƒ`ƒƒ‚Ì’¼Œa‚Ì’·‚³
	// c‚Æ‰¡‚Ì’¼Œa‚Ì’·‚³‚Í“¯‚¶‚È‚Ì‚ÅA•Ï”‚ÍfloatŒ^
	UPROPERTY(EditAnywhere)
	TMap<EMapID, float> m_MapTexDiameterLength;

	// —]”’‚Ì”¼Œa‚Ì’·‚³
	UPROPERTY(EditAnywhere)
	TMap<EMapID, FVector2D> m_MarginHalfLength;
};
