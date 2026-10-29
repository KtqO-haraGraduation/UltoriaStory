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

	// CharacterBase‚ÉGetID‚ðì‚é
	FAllyMasterData data = characterDataMg->GetAllyMasterData("Ally001");
	UTexture2D* texture = data.Image;

	if (m_Character && texture)
	{
		CharacterImage->SetBrushFromTexture(texture);
	}
}