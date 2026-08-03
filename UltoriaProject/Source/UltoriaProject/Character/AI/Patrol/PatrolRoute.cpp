// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolRoute.h"
#include "PatrolPoint.h"
#include "Components/ChildActorComponent.h"

// Sets default values
APatrolRoute::APatrolRoute()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APatrolRoute::BeginPlay()
{
	Super::BeginPlay();
	
	// パトロールポイントを取得・追加
	m_PatrolPoints.Empty();

	TArray<UChildActorComponent*> childComponents;
	GetComponents<UChildActorComponent>(childComponents);

	for (UChildActorComponent* child : childComponents)
	{
		if (!child) continue;

		APatrolPoint* point = Cast<APatrolPoint>(child->GetChildActor());
		if (point)
		{
			m_PatrolPoints.Add(point);
		}
	}
}

// Called every frame
void APatrolRoute::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

