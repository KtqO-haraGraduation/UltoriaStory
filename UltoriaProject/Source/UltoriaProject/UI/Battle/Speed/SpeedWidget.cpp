// Fill out your copyright notice in the Description page of Project Settings.


#include "SpeedWidget.h"
#include "../../../Character/CharacterBase.h"

void USpeedWidget::SetCharacter(ACharacterBase* character)
{
	if (!character) return;
	m_Character = character;
}