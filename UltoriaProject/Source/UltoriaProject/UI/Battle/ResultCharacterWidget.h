// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "BattleUIBase.h"
#include "ResultCharacterWidget.generated.h"

class ACharacterBase;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UResultCharacterWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	void SetCharacter(ACharacterBase* character);


private:
	void SetImage();
	void SetNameText();
	void SetLvText();
	void SetNextLvText();
	void UpdateLvBar();

	bool IsUpdateBar();

private:
	UPROPERTY(meta = (BindWidget))
	UImage* CharacterImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LvText;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NextLvText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* LvBar;

	ACharacterBase* m_Character;
};
