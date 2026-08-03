// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPartyManager.h"
#include "../Character/CharacterBase.h"

constexpr int PARTY_MEMBER_MIN = 1;
constexpr int PARTY_MEMBER_MAX = 4;

void UPlayerPartyManager::AddPlayerParty(ACharacterBase* character)
{
	if (!character) return;
	if (m_PlayerParty.Num() >= PARTY_MEMBER_MAX) return;

	m_PlayerParty.AddUnique(character);
}

void UPlayerPartyManager::RemovePlayerParty(ACharacterBase* character)
{
	if (!character) return;
	if (m_PlayerParty.Num() <= PARTY_MEMBER_MIN) return;

	m_PlayerParty.Remove(character);
}
