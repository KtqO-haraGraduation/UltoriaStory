// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "GaugeUIBase.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UGaugeUIBase : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float DeltaTime) override;
	virtual void NativeDestruct() override;

	void SetGaugePercentage(float value);

private:
	UPROPERTY(meta = (BindWidget))
	UImage* Orb;

	UPROPERTY()
	UMaterialInstanceDynamic* m_OrbMaterial;

	UPROPERTY(EditAnywhere)
	float m_GaugeSpeed;

	float m_CurrentPercentage = 0.0f;
	float m_TargetPercentage = 0.0f;
};
