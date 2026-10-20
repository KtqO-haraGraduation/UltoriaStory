// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../../Stage/StageParameter.h"
#include "CharacterDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> AllyTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> AllyLevelTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> EnemyTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EStageType, TObjectPtr<UDataTable>> EnemyPartyTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> SkillTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> JobTable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UDataTable> JobLvTable;
};
