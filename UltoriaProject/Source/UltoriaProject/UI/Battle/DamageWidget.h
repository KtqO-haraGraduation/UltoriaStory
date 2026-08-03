// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleUIBase.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "DamageWidget.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UDamageWidget : public UBattleUIBase
{
	GENERATED_BODY()
	

public:
	void Init(ACharacterBase* target, int damage);

private:
	void UpdatePos();
	void DestroySelf();


	void SetDamage(int damage);


private:
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HB_Damage;

	UPROPERTY()
	ACharacterBase* m_Target;

	FTimerHandle m_DestroyTimerHandle;
};
