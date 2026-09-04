// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGaugeManager.h"
#include "Combat/Components/Status/CombatStatusComponent.h"
#include "PlayerPartyManager.h"
#include "../Character/CharacterBase.h"
#include "BattleManager.h"

void UBattleGaugeManager::UpdateGauge()
{
	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

void UBattleGaugeManager::AddUltoriaGauge(int value)
{
	m_UltoriaGauge = FMath::Clamp(
		m_UltoriaGauge + value,
		0,
		m_MaxGauge);

	// ゲージが満タンになったらプレイヤー・仲間に Ultoria ステータスを追加
	if (!m_IsStartUltorla)
	{
		if (m_UltoriaGauge >= m_MaxGauge)
		{
			UPlayerPartyManager* partyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
			if (!partyMg) return;

			for (ACharacterBase* character : partyMg->GetPlayerParty())
			{
				if (!character) continue;
				UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
				status->AddStatus(StatusType::ULTORIA, "Ultoria");
				m_IsStartUltorla = true;
			}
		}
	}

	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
}

void UBattleGaugeManager::ResetUltoriaGauge()
{
	m_UltoriaGauge = 0;
	m_IsStartUltorla = false;

	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
}

float UBattleGaugeManager::GetUltoriaGaugePercent() const
{
	return FMath::Clamp(
		(float)m_UltoriaGauge / m_MaxGauge,
		0.0f,
		1.0f);
}

void UBattleGaugeManager::AddEnemyBreakGauge(int value)
{
	m_EnemyBreakGauge = FMath::Clamp(
		m_EnemyBreakGauge + value,
		0,
		m_MaxGauge);

	// ゲージが満タンになったら敵に Break ステータスを追加
	if (!m_IsStartBreak)
	{
		if (m_EnemyBreakGauge >= m_MaxGauge)
		{
			UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
			if (!battleMg) return;

			for (ACharacterBase* character : battleMg->GetEnemyParty())
			{
				if (!character) continue;
				UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
				status->AddStatus(StatusType::BREAK, "Break");
				m_IsStartBreak = true;
			}
		}
	}

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

void UBattleGaugeManager::ResetEnemyBreakGauge()
{
	m_EnemyBreakGauge = 0;
	m_IsStartBreak = false;

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

float UBattleGaugeManager::GetEnemyBreakGaugePercent() const
{
	return FMath::Clamp(
		(float)m_EnemyBreakGauge / m_MaxGauge,
		0.0f,
		1.0f);
}
