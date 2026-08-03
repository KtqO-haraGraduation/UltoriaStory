// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "CharacterStatusWidget.generated.h"

class ACharacterBase;
class UTextBlock;
class UProgressBar;

enum class CharacterStatusState
{
	IDLE,		// 待機中
	MOVE_IN,	// 出現途中（通常再生）
	MOVE_OUT,	// 消える途中（逆再生）
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UCharacterStatusWidget : public UBattleUIBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetCharacter(ACharacterBase* character);

	void SetNameText();
	UFUNCTION()
	void UpdateHP();
	UFUNCTION()
	void UpdateMP();

	UFUNCTION(BlueprintCallable)
	void HideMPUI();

	ACharacterBase* GetCharacter() const { return m_Character; }

	// アニメーション再生
	void PlayTurnSelect();
	void ReverseTurnSelect();

protected:
	UPROPERTY()
	ACharacterBase* m_Character;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* HPText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MPText;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HPBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* MPBar;
	
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* TurnSelectAnim;
};
