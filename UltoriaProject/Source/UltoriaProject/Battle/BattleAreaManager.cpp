// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleAreaManager.h"

// Sets default values
ABattleAreaManager::ABattleAreaManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABattleAreaManager::BeginPlay()
{
	Super::BeginPlay();

	// 自身の座標を中心座標に設定
	m_BattleCenterPos = GetActorLocation();
}

// Called every frame
void ABattleAreaManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<FBattleTransform> ABattleAreaManager::GetBattleTransform(int count, bool isEnemy)
{
	TArray<FBattleTransform> result;
	float offsetX = isEnemy ? m_Offset.X : -m_Offset.X;

	

	for (int i = 0; i < count; i++)
	{
		float offsetIndex = i - (count - 1) / 2.0f;

		FVector pos;

		pos.X = m_BattleCenterPos.X + offsetX;
		pos.Y = m_BattleCenterPos.Y + offsetIndex * m_Offset.Y;
		pos.Z = m_BattleCenterPos.Z;

		FRotator rot = isEnemy ? FRotator(0.0f, 180.0f, 0.0f) : FRotator(0.0f, 0.0f, 0.0f);

		FBattleTransform data;
		data.pos = pos;
		data.rot = rot;

		result.Add(data);
	}

	result.Sort([](const FBattleTransform& a, const FBattleTransform& b)
		{
			return a.pos.Y < b.pos.Y;
		});

	return result;
}

