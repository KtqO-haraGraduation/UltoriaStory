// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BattleUIBase.h"
#include "Components/CanvasPanel.h"
#include "RaidSpeedUIWidget.generated.h"

class ACharacterBase;
class USpeedWidget;

UCLASS()
class ULTORIAPROJECT_API URaidSpeedUIWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	void RefreshSpeedWidgets(const TArray<TObjectPtr<ACharacterBase>>& turnOrder);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	TSubclassOf<USpeedWidget> SpeedWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	float StartX = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	float StartY = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	float ItemWidth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	float Spacing = 20.0f;

private:
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* SpeedPanel;

	UPROPERTY(Transient)
	TArray<TObjectPtr<USpeedWidget>> m_Widgets;

	TArray<TWeakObjectPtr<ACharacterBase>> m_Shown;
};
