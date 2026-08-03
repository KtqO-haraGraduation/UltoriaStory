// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolComponent.h"
#include "PatrolRoute.h"
#include "PatrolPoint.h"
#include "../../CharacterBase.h"

// 目的地の到達判定距離(約70cm)の二乗値
constexpr float ARRIVAL_DISTANCE_SQ = 5000.0f;

// Sets default values for this component's properties
UPatrolComponent::UPatrolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

	m_NextPointFunc[static_cast<int>(EPatrolType::LOOP)]		= &UPatrolComponent::UpdateLoop;
	m_NextPointFunc[static_cast<int>(EPatrolType::PING_PONG)]	= &UPatrolComponent::UpdatePingPong;
	m_NextPointFunc[static_cast<int>(EPatrolType::ONCE)]		= &UPatrolComponent::UpdateOnce;
	m_NextPointFunc[static_cast<int>(EPatrolType::RANDOM)]		= &UPatrolComponent::UpdateRandom;
}

// Called when the game starts
void UPatrolComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	m_CurrentIndex = 0;
	if (m_PatrolRoute)
	{
		m_CurrentPoint = m_PatrolRoute->GetPatrolPoint(m_CurrentIndex);
	}
}


// Called every frame
void UPatrolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPatrolComponent::Update()
{
	if (!m_CurrentPoint || !m_PatrolRoute) return;

	if (m_IsWaiting)
	{
		UpdateWait();
	}
	else
	{
		EPatrolType type = m_PatrolRoute->GetPatrolType();

		FVector currentPos = m_Owner->GetActorLocation();
		FVector targetPos = m_CurrentPoint->GetActorLocation();

		// 目的地までの方向
		FVector delta = targetPos - currentPos;

		// 到着判定
		if (delta.SizeSquared() < ARRIVAL_DISTANCE_SQ)
		{
			ResetWaitTimer();
			return;
		}

		FVector dir = delta.GetSafeNormal();
		m_Owner->AddMovementInput(dir, 1.0f);
	}
	
}

int UPatrolComponent::FindNearestPointIndex()
{
	return 0;
}

void UPatrolComponent::UpdateLoop()
{
	const TArray<APatrolPoint*>& patrolPoints = m_PatrolRoute->GetPatrolPoints();
	if (patrolPoints.IsEmpty()) return;

	m_CurrentIndex++;
	if (m_CurrentIndex >= patrolPoints.Num())
	{
		m_CurrentIndex = 0;
	}
	m_PrevPoint = m_CurrentPoint;
	m_CurrentPoint = patrolPoints[m_CurrentIndex];
}

void UPatrolComponent::UpdatePingPong()
{
}

void UPatrolComponent::UpdateOnce()
{
}

void UPatrolComponent::UpdateRandom()
{
	const TArray<APatrolPoint*>& patrolPoints = m_PatrolRoute->GetPatrolPoints();
	if (patrolPoints.IsEmpty()) return;

	int newIndex;

	do
	{
		newIndex = FMath::RandRange(0, patrolPoints.Num() - 1);
	} while (newIndex == m_CurrentIndex);

	m_CurrentIndex = newIndex;

	m_PrevPoint = m_CurrentPoint;
	m_CurrentPoint = patrolPoints[m_CurrentIndex];
}

void UPatrolComponent::ResetWaitTimer()
{
	m_WaitTimer = m_WaitTime;
	m_IsWaiting = true;
}

void UPatrolComponent::UpdateWait()
{
	if (!m_IsWaiting) return;

	m_WaitTimer -= GetWorld()->GetDeltaSeconds();

	if (m_WaitTimer <= 0.0f)
	{
		m_IsWaiting = false;

		// 次のPointを選択
		EPatrolType type = m_PatrolRoute->GetPatrolType();
		(this->*m_NextPointFunc[static_cast<int>(type)])();
	}
}
