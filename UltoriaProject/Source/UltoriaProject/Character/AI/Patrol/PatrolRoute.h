// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolRoute.generated.h"

class APatrolPoint;

// ルートのタイプ
UENUM()
enum class EPatrolType : uint8
{
	LOOP,		// ループ
	PING_PONG,	// 往復
	ONCE,		// 一度のみ
	RANDOM,		// ランダム
	MAX,
};

UCLASS()
class ULTORIAPROJECT_API APatrolRoute : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APatrolRoute();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	const TArray<APatrolPoint*>& GetPatrolPoints() const { return m_PatrolPoints; }
	APatrolPoint* GetPatrolPoint(int index) { return m_PatrolPoints[index]; }

	EPatrolType GetPatrolType() const { return m_PartolType; }

private:
	TArray<APatrolPoint*> m_PatrolPoints;

	UPROPERTY(EditAnywhere)
	EPatrolType m_PartolType;
};
