// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleGaugeManager.h"

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

	OnUltoriaGaugeChanged.Broadcast(GetUltoriaGaugePercent());
}

void UBattleGaugeManager::ResetUltoriaGauge()
{
	m_UltoriaGauge = 0;

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

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

void UBattleGaugeManager::ResetEnemyBreakGauge()
{
	m_EnemyBreakGauge = 0;

	OnEnemyBreakGaugeChanged.Broadcast(GetEnemyBreakGaugePercent());
}

float UBattleGaugeManager::GetEnemyBreakGaugePercent() const
{
	return FMath::Clamp(
		(float)m_EnemyBreakGauge / m_MaxGauge,
		0.0f,
		1.0f);
}
