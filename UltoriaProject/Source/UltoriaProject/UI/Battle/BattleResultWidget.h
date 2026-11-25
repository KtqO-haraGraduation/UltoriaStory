// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/VerticalBox.h"
#include "BattleResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleResultWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// ê∂ê¨éûÇÃèâä˙âªèàóù
	virtual void NativeConstruct() override;

	void SetGoldText(int gold);

protected:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_Character;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* GoldText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemText;

	UPROPERTY(meta = (BindWidget))
	UImage* JobImage;

	// ä‘ÇÃä‘äu
	static constexpr float ResultPadding = 20.0f;
};
