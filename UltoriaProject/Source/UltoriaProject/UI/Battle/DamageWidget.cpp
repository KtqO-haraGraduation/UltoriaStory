// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageWidget.h"
#include "../../Character/CharacterBase.h"
#include "Kismet/GameplayStatics.h"

constexpr float DESTROY_TIME = 1.0f;
const FVector DANAGE_TEXT_OFFSET = { 10.0f, 0.0f, 100.0f };

void UDamageWidget::Init(ACharacterBase* target, int damage)
{
	m_Target = target;

	SetDamage(damage);
	UpdatePos();

	GetWorld()->GetTimerManager().SetTimer(
		m_DestroyTimerHandle,
		this,
		&UDamageWidget::DestroySelf,
		DESTROY_TIME,
		false
		);
}

void UDamageWidget::UpdatePos()
{
	if (!m_Target) { return; }

	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	if (!pc) { return; }

	// 一旦、頭上に表示
	FVector worldPos = m_Target->GetActorLocation() + DANAGE_TEXT_OFFSET;

	FVector2D screenPos;
	if (UGameplayStatics::ProjectWorldToScreen(pc, worldPos, screenPos))
	{
		SetPositionInViewport(screenPos);
	}
}

void UDamageWidget::DestroySelf()
{
	RemoveFromParent();
}

void UDamageWidget::SetDamage(int damage)
{
	FString damageStr = FString::FromInt(damage);
	int textCount = HB_Damage->GetChildrenCount();
	int digit = damageStr.Len();

	// 全ての文字を空にする
	for (int i = 0; i < textCount; i++)
	{
		if (UTextBlock* text = Cast<UTextBlock>(HB_Damage->GetChildAt(i)))
		{
			text->SetText(FText::GetEmpty());
		}
	}

	// 右揃えでセット
	for (int i = 0; i < digit; i++)
	{
		int childIndex = textCount - digit + i;

		if (UTextBlock* text = Cast<UTextBlock>(HB_Damage->GetChildAt(childIndex)))
		{
			text->SetText(FText::FromString(damageStr.Mid(i, 1)));
		}
	}
}
