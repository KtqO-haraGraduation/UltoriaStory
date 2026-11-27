// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedWidget.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Character/Data/CharacterDataManager.h"

void USpeedWidget::SetCharacter(ACharacterBase* character)
{
	if (!character) return;
	m_Character = character;

	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!characterDataMg) return;

	UTexture2D* texture = nullptr;
	FName characterID = m_Character->GetID();

	// CharacterBase‚ÉGetID‚ðì‚é
	if (character->GetTeamType() == ETeamType::PLAYER)
	{
		FAllyMasterData allyData = characterDataMg->GetAllyMasterData(characterID);
		texture = allyData.Image;
	}
	else if (character->GetTeamType() == ETeamType::ENEMY)
	{
		FEnemyMasterData enemyData = characterDataMg->GetEnemyData(characterID);
		texture = enemyData.Image;
	}

	// ‰æ‘œ‚ðÝ’è
	if (m_Character && texture)
	{
		if (m_Character && texture && CharacterImage)
		{
			CharacterImage->SetBrushFromTexture(texture);
		}
	}
}
