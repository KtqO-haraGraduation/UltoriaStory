// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.generated.h"

enum class EBattleUIType
{
	NONE,

	STATUS,
	COMMAND,
	SKILL,
	ITEM,
	SPEED,
	TARGET,
	ALLTARGET,
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleUIBase : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	virtual void BattleStart();
	UFUNCTION(BlueprintCallable)
	virtual void BattleEnd();

	// •\Ž¦ / ”ñ•\Ž¦
	virtual void SetVisible(bool flag);

	UFUNCTION(BlueprintCallable)
	virtual void ShowUI();
	UFUNCTION(BlueprintCallable)
	virtual void HideUI();
};
