// Fill out your copyright notice in the Description page of Project Settings.


#include "GaugeUIBase.h"
#include "Components/Image.h"

void UGaugeUIBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (Orb)
	{
		m_OrbMaterial = Orb->GetDynamicMaterial();
	}
}

void UGaugeUIBase::NativeTick(const FGeometry& MyGeometry, float DeltaTime)
{
	Super::NativeTick(MyGeometry, DeltaTime);

	if (!m_OrbMaterial) return;

	// ゲージの上昇処理
	// 現在値と目標値が違ったら処理をする
	if (!FMath::IsNearlyEqual(
		m_CurrentPercentage,
		m_TargetPercentage))
	{
		// 線形補完（現在値→目標値）
		m_CurrentPercentage = FMath::FInterpTo(
			m_CurrentPercentage,
			m_TargetPercentage,
			DeltaTime,
			m_GaugeSpeed
		);

		// パーセンテージに設定
		m_OrbMaterial->SetScalarParameterValue(
			TEXT("Percentage"),
			m_CurrentPercentage);
	}
}

void UGaugeUIBase::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGaugeUIBase::SetGaugePercentage(float value)
{
	m_TargetPercentage = value;
}