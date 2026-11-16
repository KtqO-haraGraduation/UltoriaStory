// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFieldAIComponent.h"
#include "../../../Ally/MainPlayer.h"
#include "../../Patrol/PatrolComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/WidgetComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
//#include "AIController.h"

// Sets default values for this component's properties
UEnemyFieldAIComponent::UEnemyFieldAIComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UEnemyFieldAIComponent::Update()
{
	UpdateState();

	switch (m_EnemyAIState)
	{
	case EEnemyAIState::PATROL:
		UpdatePatrol();
		break;
	case EEnemyAIState::SEARCH:
		UpdateSearch();
		break;
	case EEnemyAIState::CHASE:
		UpdateChase();
		break;
	case EEnemyAIState::RETURN:
		UpdateReturn();
		break;
	}
}

// Called when the game starts
void UEnemyFieldAIComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

	m_Player = Cast<AMainPlayer>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	m_PatrolComponent = m_Owner->GetComponent<UPatrolComponent>();

	// UIを取得
	TArray<UWidgetComponent*> widgets;
	m_Owner->GetComponents<UWidgetComponent>(widgets);

	for (UWidgetComponent* widget : widgets)
	{
		if (widget->ComponentHasTag(TEXT("PreChase")))
		{
			m_PreChaseWidget = widget;
		}
		else if (widget->ComponentHasTag(TEXT("Return")))
		{
			m_ReturnWidget = widget;
		}
	}
}


// Called every frame
void UEnemyFieldAIComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemyFieldAIComponent::ChangeState(EEnemyAIState newState)
{
	if (m_EnemyAIState == newState)
	{
		return;
	}

	m_EnemyAIState = newState;
	m_PatrolComponent->SetActiveComponent(newState == EEnemyAIState::PATROL);

	if (newState == EEnemyAIState::CHASE)
	{
		// UIを表示
		ShowReturnUI(false);
		ShowPreChaseUI(true);

		ResetSearchWaitTimer();
	}

	if (newState == EEnemyAIState::RETURN)
	{
		// UIを表示
		ShowReturnUI(true);

		ResetWaitTimer();
	}
}

void UEnemyFieldAIComponent::UpdateState()
{
	// 視野角を描画
	DrawViewRange();

	// 見逃し処理
	if (m_EnemyAIState == EEnemyAIState::CHASE)
	{
		if (NeedReturn() && IsPreChaseWaitFinished())
		{
			ChangeState(EEnemyAIState::RETURN);
		}
		return;
	}

	// 視野角に入れば、チェイス
	if (CanSeePlayer())
	{
		ChangeState(EEnemyAIState::CHASE);
		return;
	}

	if (m_EnemyAIState == EEnemyAIState::RETURN)
	{
		return;
	}

	// 全ての条件に当てはまらなければ、パトロールへ
	ChangeState(EEnemyAIState::PATROL);
}

void UEnemyFieldAIComponent::UpdatePatrol()
{
	UPatrolComponent* patrol = m_Owner->GetComponent<UPatrolComponent>();
	if (patrol) patrol->UpdatePatrol();
}

void UEnemyFieldAIComponent::UpdateSearch()
{
	
}

void UEnemyFieldAIComponent::UpdateChase()
{
	if (!m_Player) return;

	if (!IsPreChaseWaitFinished())
	{
		m_PreChaseWaitTimer -= GetWorld()->GetDeltaSeconds();
		return;
	}

	ShowPreChaseUI(false);

	FVector start = m_Owner->GetActorLocation();
	FVector target = m_Player->GetActorLocation();

	UNavigationSystemV1* navSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!navSystem) return;

	// 座標指定で経路検索
	/*UNavigationPath* path =
		navSystem->FindPathToLocationSynchronously(
			GetWorld(),
			start,
			target,
			m_Owner
		);*/

	UNavigationPath* path =
		navSystem->FindPathToActorSynchronously(
			GetWorld(),
			start,
			m_Player,
			100.0f,
			m_Owner
		);

	FNavLocation startNav;
	FNavLocation targetNav;

	bool startResult = navSystem->ProjectPointToNavigation(
		start, startNav);

	bool targetResult = navSystem->ProjectPointToNavigation(
		target, targetNav);

	// NavMeshで計算された次の地点
	FVector nextPoint = path->PathPoints[1];

	// 次の地点へ向かう
	FVector dir =
		(nextPoint - start).GetSafeNormal();

	m_Owner->AddMovementInput(dir, 1.0f);

	/*else
	{
		ShowPreChaseUI(false);

		FVector targetPos = m_Player->GetActorLocation();
		FVector pos = m_Owner->GetActorLocation();

		FVector dir = targetPos - pos;
		dir.Normalize();

		m_Owner->AddMovementInput(dir, 1.0f);
	}*/
}

void UEnemyFieldAIComponent::UpdateReturn()
{
	// しばらくその場で待機
	m_WaitTimer -= GetWorld()->GetDeltaSeconds();

	if (m_WaitTimer <= 0.0f)
	{
		ChangeState(EEnemyAIState::PATROL);
		ShowReturnUI(false);
	}
}

bool UEnemyFieldAIComponent::CanSeePlayer()
{
	if (GetPlayerDistance() > m_SearchDistance)
	{
		return false;
	}

	FVector forward = m_Owner->GetActorForwardVector();

	FVector toPlayer = (m_Player->GetActorLocation() - m_Owner->GetActorLocation()).GetSafeNormal();

	float dot = FVector::DotProduct(forward, toPlayer);
	float dotLimit = FMath::Cos(FMath::DegreesToRadians(m_ViewAngle * 0.5f));

	return dot >= dotLimit;
}

void UEnemyFieldAIComponent::ResetSearchWaitTimer()
{
	m_PreChaseWaitTimer = m_PreChaseWaitTime;
}

bool UEnemyFieldAIComponent::IsPreChaseWaitFinished()
{
	return m_PreChaseWaitTimer <= 0.0f;
}

bool UEnemyFieldAIComponent::NeedReturn()
{
	if (m_EnemyAIState != EEnemyAIState::CHASE) return false;
	return GetPlayerDistance() >= m_ReturnDistance;
}

void UEnemyFieldAIComponent::ResetWaitTimer()
{
	m_WaitTimer = m_WaitTime;
}

bool UEnemyFieldAIComponent::IsReturnWaitFinished()
{
	return m_WaitTimer <= 0.0f;
}

float UEnemyFieldAIComponent::GetPlayerDistance() const
{
	if (!m_Player)
	{
		return TNumericLimits<float>::Max();
	}

	return FVector::Distance(
		m_Owner->GetActorLocation(),
		m_Player->GetActorLocation());
}

void UEnemyFieldAIComponent::DrawViewRange()
{
	FVector start = m_Owner->GetActorLocation();
	FVector forward = m_Owner->GetActorForwardVector();

	// 左右に半分の角度だけ回転
	FVector leftDir = forward.RotateAngleAxis(-m_ViewAngle * 0.5f, FVector::UpVector);
	FVector rightDir = forward.RotateAngleAxis(m_ViewAngle * 0.5f, FVector::UpVector);

	DrawDebugLine(
		GetWorld(),
		start,
		start + leftDir * m_SearchDistance,
		FColor::Green,
		false,
		0.0f,
		0,
		3.0f
	);

	DrawDebugLine(
		GetWorld(),
		start,
		start + rightDir * m_SearchDistance,
		FColor::Green,
		false,
		0.0f,
		0,
		3.0f
	);

	// 正面も描画
	DrawDebugLine(
		GetWorld(),
		start,
		start + forward * m_SearchDistance,
		FColor::Red,
		false,
		0.0f,
		0,
		2.0f
	);
}

void UEnemyFieldAIComponent::ShowPreChaseUI(bool flag)
{
	if (!m_PreChaseWidget) return;
	if (m_PreChaseWidget->IsVisible() == flag) return;

	m_PreChaseWidget->SetVisibility(flag);
}

void UEnemyFieldAIComponent::ShowReturnUI(bool flag)
{
	if (!m_ReturnWidget) return;
	if (m_ReturnWidget->IsVisible() == flag) return;

	m_ReturnWidget->SetVisibility(flag);
}
