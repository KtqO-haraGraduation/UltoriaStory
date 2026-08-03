// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../../Battle/Combat/Components/ComponentBase.h"
#include "EnemyFieldAIComponent.generated.h"

class UPatrolComponent;
class ATestPlayerMC;
class UWidgetComponent;

UENUM(BlueprintType)
enum class EEnemyAIState : uint8
{
	PATROL, // 巡回
	SEARCH, // 周囲を探す
	CHASE,  // 追跡
	RETURN, // 元の位置へ戻る
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API UEnemyFieldAIComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFieldAIComponent();

	void Update() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	bool IsState(EEnemyAIState state) const { return m_EnemyAIState == state; }

	UFUNCTION(BlueprintCallable)
	EEnemyAIState GetState() const { return m_EnemyAIState; }

	UFUNCTION(BlueprintCallable)
	bool IsPreChaseWaitFinished();

private:
	void ChangeState(EEnemyAIState newState);

	void UpdateState();

	void UpdatePatrol();
	void UpdateSearch();
	void UpdateChase();
	void UpdateReturn();

	// プレイヤーを発見したか
	bool CanSeePlayer();
	void ResetSearchWaitTimer();
	

	// プレイヤーを見逃したか
	bool NeedReturn();
	void ResetWaitTimer();
	bool IsReturnWaitFinished();

	float GetPlayerDistance() const;

	// デバック用（視野角を描画）
	void DrawViewRange();

	void ShowPreChaseUI(bool flag);
	void ShowReturnUI(bool flag);

private:
	EEnemyAIState m_EnemyAIState;

	ATestPlayerMC* m_Player;
	UPatrolComponent* m_PatrolComponent;

	UPROPERTY(EditAnywhere, Category = "AI")
	float m_ViewAngle;

	UPROPERTY(EditAnywhere, Category = "AI")
	float m_SearchDistance;

	UPROPERTY(EditAnywhere, Category = "AI")
	float m_ReturnDistance;

	// プレイヤーを発見した時の待機時間
	UPROPERTY(EditAnywhere, Category = "AI")
	float m_PreChaseWaitTime;
	float m_PreChaseWaitTimer;

	// プレイヤーを見失った時の待機時間
	UPROPERTY(EditAnywhere, Category = "AI")
	float m_WaitTime;
	float m_WaitTimer;

	UPROPERTY()
	UWidgetComponent* m_PreChaseWidget;

	UPROPERTY()
	UWidgetComponent* m_ReturnWidget;
};
