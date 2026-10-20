// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleUIDataAsset.generated.h"

class UCommandButtonWidget;
class USkillButtonWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommandButtonWidget> CommandButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USkillButtonWidget> SkillButtonClass;

};
