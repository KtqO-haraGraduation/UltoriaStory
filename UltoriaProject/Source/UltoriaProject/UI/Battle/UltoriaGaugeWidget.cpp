// Fill out your copyright notice in the Description page of Project Settings.


#include "UltoriaGaugeWidget.h"
#include "../../Battle/BattleGaugeManager.h"
#include "Components/Image.h"
#include "GaugeUIBase.h"

void UUltoriaGaugeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ウルトリアゲージの変更時に呼び出される（イベント）
	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		SetGaugePercentage(gaugeManager->GetUltoriaGaugePercent());

		gaugeManager->OnUltoriaGauge().AddUObject(
			this,
			&UUltoriaGaugeWidget::SetGaugePercentage
		);
	}
}

void UUltoriaGaugeWidget::NativeDestruct()
{
	UBattleGaugeManager* gaugeManager = GetWorld()->GetSubsystem<UBattleGaugeManager>();
	if (gaugeManager)
	{
		gaugeManager->OnUltoriaGauge().RemoveAll(this);
	}

	Super::NativeDestruct();
}
