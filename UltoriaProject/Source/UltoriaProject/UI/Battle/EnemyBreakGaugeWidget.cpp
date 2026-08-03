// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBreakGaugeWidget.h"
#include "../../Battle/BattleGaugeManager.h"
#include "Components/Image.h"
#include "GaugeUIBase.h"

void UEnemyBreakGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ウルトリアゲージの変更時に呼び出される（イベント）
	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		SetGaugePercentage(gaugeManager->GetEnemyBreakGaugePercent());

		gaugeManager->OnEnemyBreakGauge().AddUObject(
			this,
			&UEnemyBreakGaugeWidget::SetGaugePercentage
		);
	}
}

void UEnemyBreakGaugeWidget::NativeDestruct()
{
	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		gaugeManager->OnEnemyBreakGauge().RemoveAll(this);
	}

	Super::NativeDestruct();
}

