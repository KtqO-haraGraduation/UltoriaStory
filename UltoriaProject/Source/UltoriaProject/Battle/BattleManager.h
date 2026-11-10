// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Combat/Data/BattleCommandData.h"
#include "../Job/Skill/SkillDataRow.h"
#include "BattleManager.generated.h"

class ACharacterBase;
class AEnemyBase;
class ABattleAreaManager;
class UBattleLifecycle;
class UBattleTurnManager;
class UBattleSelectioinManager;
class UBattleCameraManager;
enum class EUIInputType;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTurnChanged, ACharacterBase*, ACharacterBase*);

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UBattleManager : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	void BattleStart(ACharacterBase* enemy, bool isPreemptive);	// バトル開始
	void NextTurn();	// 次のターン
	void BattleEnd();	// バトル終了

	// 現在行動してるキャラクターを取得
	ACharacterBase* GetCurrentCharacter();

	// バトル中かのフラグ
	UFUNCTION(BlueprintCallable)
	bool IsBattle() const { return m_IsBattle; }
	FOnTurnChanged& OnTurnChanged() { return m_OnTurnChanged; }

	// コマンドごとの実行処理
	void ExecuteCommand(ECommandType type);
	void ExecuteAttack(ACharacterBase* target);

	// キャラクターを並べる
	void SetCharacterPos(ABattleAreaManager* area);
	// パーティー作成
	void CreateParty(AEnemyBase* enemyBase);

	// 敵を取得
	const TArray<ACharacterBase*>& GetEnemyParty() const { return m_EnemyParty; }

	// ObjectのGet関数
	UBattleLifecycle* GetLifecycle() { return m_Lifecycle; }
	UBattleTurnManager* GetTurnManager() { return m_TurnManager; }
	UBattleSelectioinManager* GetSelectManager() { return m_SelectManager; }
	UBattleCameraManager* GetCameraManager() { return m_CameraManager; }

	void FinishBattle();

	// ターゲットを取得
	TArray<ACharacterBase*> GetTargets(ETargetTeam team) const;
	TArray<ACharacterBase*> GetActiveTargets(ETargetTeam team) const; // 生存してるターゲット

	void ShowAttackTargetUI(const TArray<ACharacterBase*>& targets);
	void ShowTargetUI(const TArray<ACharacterBase*>& targets);
	void ShowAllTargetUI(const TArray<ACharacterBase*>& targets);

private:
	//void InitializeSelection();
	void NotifyBattleStart();
	void NotifyBattleEnd();

	// 各キャラクターの状態をリセット
	void ResetBattleState();

	// Attackの処理
	void StartAttack();
	void StartSkill();
	void StartItem();

	bool BattleWin();
	bool BattleLose();

private:
	bool m_IsBattle;							 // バトル中か
	bool m_IsVictory;							 //	プレイヤー側が勝負に勝ったか
	FOnTurnChanged m_OnTurnChanged;

	// Object関連
	UPROPERTY()
	UBattleLifecycle* m_Lifecycle;
	UPROPERTY()
	UBattleTurnManager* m_TurnManager;
	UPROPERTY()
	UBattleSelectioinManager* m_SelectManager;
	UPROPERTY()
	UBattleCameraManager* m_CameraManager;

	TArray<ACharacterBase*> m_PlayerParty; // 戦闘時のプレイヤーリスト
	TArray<ACharacterBase*> m_EnemyParty;  // 戦闘時の敵リスト
};
