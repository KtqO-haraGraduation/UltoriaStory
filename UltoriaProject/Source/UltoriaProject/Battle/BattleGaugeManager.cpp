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


// --- Ultoria Gauge --- //

void UBattleGaugeManager::StartUltoria()
{
	UPlayerPartyManager* partyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!partyMg) return;

	m_IsStartUltorla = true;

	// プレイヤーパーティー全員のステータスに Ultoria を追加
	for (ACharacterBase* character : partyMg->GetPlayerParty())
	{
		if (!character) continue;
		UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
		if (!status) continue;

		status->AddStatus(StatusType::ULTORIA);
		status->SetUltoria(true);
	}
}

void UBattleGaugeManager::AddUltoriaGauge(int value)
{
	m_UltoriaGauge = FMath::Clamp(
		m_UltoriaGauge + value,
		0,
		m_MaxGauge);

	// ゲージが満タンになったらプレイヤー・仲間に Ultoria ステータスを追加
	if (m_UltoriaGauge >= m_MaxGauge && !m_IsStartUltorla)
	{
		StartUltoria();
	}

	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
}



void UBattleGaugeManager::ResetUltoriaGauge()
{
	m_UltoriaGauge = 0;
	m_IsStartUltorla = false;

	UPlayerPartyManager* partyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!partyMg) return;

	for (ACharacterBase* character : partyMg->GetPlayerParty())
	{
		if (!character) continue;
		UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
		if (!status) continue;
		status->SetUltoria(false);
	}

	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
}

float UBattleGaugeManager::GetUltoriaGaugePercent() const
{
	return FMath::Clamp(
		(float)m_UltoriaGauge / m_MaxGauge,
		0.0f,
		1.0f);
}

// --- Break Gauge --- //

void UBattleGaugeManager::StartBreakGauge()
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	m_IsStartBreak = true;

	// 敵パーティーのステータスに Break を追加
	for (ACharacterBase* character : battleMg->GetEnemyParty())
	{
		if (!character) continue;
		UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
		if (!status) continue;

		status->AddStatus(StatusType::BREAK);
		status->SetBreaking(true);
	}
}

void UBattleGaugeManager::AddEnemyBreakGauge(int value)
{
	m_EnemyBreakGauge = FMath::Clamp(
		m_EnemyBreakGauge + value,
		0,
		m_MaxGauge);

	// ゲージが満タンになったら敵に Break ステータスを追加
	if (m_EnemyBreakGauge >= m_MaxGauge && !m_IsStartBreak)
	{
		StartBreakGauge();
	}

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

void UBattleGaugeManager::ResetEnemyBreakGauge()
{
	m_EnemyBreakGauge = 0;
	m_IsStartBreak = false;

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	for (ACharacterBase* character : battleMg->GetEnemyParty())
	{
		if (!character) continue;
		UCombatStatusComponent* status = character->GetComponent<UCombatStatusComponent>();
		if (!status) continue;

		status->SetBreaking(false);
	}

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

float UBattleGaugeManager::GetEnemyBreakGaugePercent() const
{
	return FMath::Clamp(
		(float)m_EnemyBreakGauge / m_MaxGauge,
		0.0f,
		1.0f);
}
