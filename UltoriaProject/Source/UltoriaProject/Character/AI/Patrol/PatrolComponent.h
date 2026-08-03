// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../Battle/Combat/Components/ComponentBase.h"
#include "PatrolRoute.h"
#include "PatrolComponent.generated.h"

class APatrolPoint;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UPatrolComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPatrolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void Update() override;

	int FindNearestPointIndex();

private:
	void UpdateLoop();
	void UpdatePingPong();
	void UpdateOnce();
	void UpdateRandom();

	void ResetWaitTimer();
	void UpdateWait();

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	APatrolRoute* m_PatrolRoute;
	APatrolPoint* m_CurrentPoint;
	APatrolPoint* m_PrevPoint;

	int m_CurrentIndex;
	
	UPROPERTY(EditAnywhere, Category = "Patrol")
	float m_WaitTime;	// 待機時間（設定用：秒）
	float m_WaitTimer;	// 経過時間
	
	bool m_IsWaiting; // 待機中か

	void(UPatrolComponent::* m_NextPointFunc[static_cast<int>(EPatrolType::MAX)])();
};
