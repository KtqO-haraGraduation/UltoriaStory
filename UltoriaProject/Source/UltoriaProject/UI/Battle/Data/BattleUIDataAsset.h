// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "BattleUIDataAsset.generated.h"

class UCommandButtonWidget;
class USkillButtonWidget;
class UItemButtonWidget;
class UBattleUIWidget;
class UBattleResultWidget;
class UResultCharacterWidget;
class UDamageWidget;
class USpeedWidget;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBattleUIWidget> BattleUIClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UCommandButtonWidget> CommandButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USkillButtonWidget> SkillButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UItemButtonWidget> ItemButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UBattleResultWidget> ResultWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UResultCharacterWidget> ResultCharacterWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UDamageWidget> DamageWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USpeedWidget> SpeedWidgetClass;

};
