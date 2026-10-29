// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTurnManager.h"
#include "../../../Character/CharacterBase.h"
#include "../Components/CombatComponent.h"
#include "../Components/Status/CombatStatusComponent.h"

#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../UI/Battle/Speed/SpeedUIWidget.h"

void UBattleTurnManager::NextTurn()
{
	if (m_Characters.IsEmpty()) return;
	
	// 前回のキャラクター
	m_PrevCharacter = m_Characters[m_CurrentIndex];

	// 次のキャラクターへ
	m_CurrentIndex++;

	if (m_CurrentIndex >= m_Characters.Num())
	{
		m_CurrentIndex = 0;
		AddTurn();
	}

	// 死亡しているキャラクターをスキップ
	UCombatComponent* combat = m_Characters[m_CurrentIndex]->GetComponent<UCombatComponent>();
	if (combat && 
		combat->GetCurrentState() == CombatState::DEAD)
	{
		NextTurn();
		return;
	}
}

void UBattleTurnManager::UpdateTurnEnd()
{
	UCombatStatusComponent* status =
		m_Characters[m_CurrentIndex]->GetComponent<UCombatStatusComponent>();
	if (!status) return;

	status->UpdateTurn();
}

void UBattleTurnManager::Start()
{
}

void UBattleTurnManager::End()
{
	ResetCharacters();
	ResetTurn();
}

void UBattleTurnManager::AddCharacter(ACharacterBase* character)
{
	if (!character) return;
	m_Characters.Add(character);
}

void UBattleTurnManager::AddParty(const TArray<ACharacterBase*>& characters)
{
	m_Characters.Append(characters);
}

void UBattleTurnManager::SortBySpeedDescending()
{
	m_Characters.Sort(
		[](const ACharacterBase& a,
			const ACharacterBase& b)
		{
			return a.GetBaseStatus().m_SPD > b.GetBaseStatus().m_SPD;
		});
}

void UBattleTurnManager::ResetCharacters()
{
	m_Characters.Empty();

	m_CurrentIndex = 0;
	m_PrevCharacter = nullptr;
}

void UBattleTurnManager::AddTurn()
{
	m_TurnNum++;
}

void UBattleTurnManager::ResetTurn()
{
	m_TurnNum = 1;
}

void UBattleTurnManager::CreateSpeedUI()
{


}

void UBattleTurnManager::RemoveSpeedUI()
{

}


