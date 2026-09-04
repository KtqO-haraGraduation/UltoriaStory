// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "BattleGaugeManager.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnUltoriaGaugeChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEnemyBreakGaugeChanged, float);

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleGaugeManager : public UWorldSubsystem
{
	GENERATED_BODY()
public:
	// ゲージの更新
	void UpdateGauge();

	void AddUltoriaGauge(int value);
	void ResetUltoriaGauge();
	float GetUltoriaGaugePercent() const;

	void AddEnemyBreakGauge(int value);
	void ResetEnemyBreakGauge();
	float GetEnemyBreakGaugePercent() const;

	FOnUltoriaGaugeChanged& OnUltoriaGauge() { return OnUltoriaGaugeChanged; }
	FOnEnemyBreakGaugeChanged& OnEnemyBreakGauge() { return OnEnemyBreakGaugeChanged; }

protected:
	FOnUltoriaGaugeChanged OnUltoriaGaugeChanged;
	FOnEnemyBreakGaugeChanged OnEnemyBreakGaugeChanged;

private:
	bool m_IsStartUltorla;
	bool m_IsStartBreak;
	int m_UltoriaGauge;
	int m_EnemyBreakGauge;
	float m_MaxGauge = 100.0f;
};
