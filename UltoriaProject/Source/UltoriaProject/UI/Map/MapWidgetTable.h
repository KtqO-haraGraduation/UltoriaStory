// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapWidgetTable.generated.h"

class UMapWidget;

UENUM(BlueprintType)
enum class EMapID :uint8
{
	CASRLE,
	ENTRANCE,
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

	// 余白ピクセル数
	UPROPERTY(EditAnywhere)
	TMap<EMapID, float> m_MarginPxX;	// X
	UPROPERTY(EditAnywhere)
	TMap<EMapID, float> m_MarginPxY;	// Y
};
