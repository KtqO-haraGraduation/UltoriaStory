// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GaugeUIBase.h"
#include "EnemyBreakGaugeWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEnemyBreakGaugeWidget : public UGaugeUIBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
