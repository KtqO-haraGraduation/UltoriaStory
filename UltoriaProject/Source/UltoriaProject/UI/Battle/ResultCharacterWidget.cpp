// Fill out your copyright notice in the Description page of Project Settings.

#include "ResultCharacterWidget.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Data/CharacterDataManager.h"
#include "../../Character/Data/AllyMasterData.h"

void UResultCharacterWidget::SetCharacter(ACharacterBase* character)
{
	m_Character = character;

	if (!m_Character)
	{
		HideUI();
		return;
	}

	ShowUI();

	SetImage();
	SetNameText();
	SetLvText();
	SetNextLvText();
}

void UResultCharacterWidget::SetImage()
{
	if (!CharacterImage) return;
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!characterDataMg) return;

	UTexture2D* texture = nullptr;
	FName characterID = m_Character->GetID();

	FAllyMasterData allyData = characterDataMg->GetAllyMasterData(characterID);
	texture = allyData.Image;

	CharacterImage->SetBrushFromTexture(texture);
}

void UResultCharacterWidget::SetNameText()
{
	NameText->SetText(
		FText::FromString(m_Character->GetCharacterName()));
}

void UResultCharacterWidget::SetLvText()
{
	if (!LvText) return;

	LvText->SetText(
		FText::FromString(
			FString::Printf(
				TEXT("Lv %d"),
				m_Character->GetLevel()
			)
		)
	);
}

void UResultCharacterWidget::SetNextLvText()
{
	if (!NextLvText) return;
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!characterDataMg) return;

	// 現在のキャラクターの経験値を取得する
	int nowExp = m_Character->GetExp();

	FName id = m_Character->GetID();
	int lv = m_Character->GetLevel();
	lv++;
	FAllyLevelData lvData = characterDataMg->GetAllyLevelData(id, lv);

	// 次のLvの経験値を取得する
	int nextExp = lvData.Exp;

	int remainingExp = nextExp - nowExp;

	NextLvText->SetText(
		FText::FromString(
			FString::Printf(
				TEXT("%d"),
				remainingExp
			)
		)
	);
}

void UResultCharacterWidget::UpdateLvBar()
{
	int oldExp = 0;
	int nowExp = 0;
}
