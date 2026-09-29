// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ComponentBase.h"
#include "CombatComponent.generated.h"

class ACharacterBase;

UENUM(BlueprintType)
// 戦闘中の状態
enum class CombatState : uint8
{
	IDLE,
	ATTACK,
	GUARD,
	SKILL,
	ITEM,
	ESCAPE,  // 逃げる
	DAMEGE,
	DEADING, // 死亡中
	DEAD,    // 死亡
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ULTORIAPROJECT_API UCombatComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 取得・設定関連
	// 現在の状態
	UFUNCTION(BlueprintCallable)
	CombatState GetCurrentState() { return m_CurrentState; }
	void SetCurrentState(CombatState state) { m_CurrentState = state; }

	void SetPreemptive(bool flag) { m_IsPreemptive = flag; }

	// バトル開始処理
	virtual void BattleStart();
	// バトル終了処理
	virtual void BattleEnd();

	// ターンの開始
	virtual void StartTurn();

	// 全キャラ共通で持ってる処理
	virtual void Attack(ACharacterBase* target);
	virtual void Guard();
	virtual void Skill(const TArray<ACharacterBase*>& targets); // 後でSkillBaseを取得できるようにする
	virtual void Item(ACharacterBase* target);
	virtual bool Escape();
	virtual void TakeDamage(int damage);
	virtual void Deading();
	virtual void Dead();

	virtual void ResetState(); // 状態をリセット

protected:
	UFUNCTION(BlueprintCallable)
	virtual void OnAttackHit();

	UFUNCTION(BlueprintCallable)
	void OnActionEnd();

	UFUNCTION(BlueprintCallable)
	void OnDead();

protected:
	UPROPERTY(EditAnywhere)		// 逃走の成功率（0 ～ 99）
	int m_EscapeRate;
	int m_GuardCount = 3; // ガードのターン数
	bool m_IsGuarding;			// ガード中か
	bool m_IsPreemptive;		// 先制攻撃か

	CombatState m_CurrentState;		// 現在の状態
	ACharacterBase* m_Target;
};
