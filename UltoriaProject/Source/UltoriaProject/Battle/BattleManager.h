// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Combat/Data/BattleCommandData.h"
#include "../System/Selection/SelectionSystem.h"
#include "BattleManager.generated.h"

class ACharacterBase;
class AEnemyBase;
class ABattleAreaManager;
class UBattleLifecycle;
class UBattleTurnManager;
enum class EUIInputType;
//enum class ESelectionType;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTurnChanged, ACharacterBase*, ACharacterBase*);

// 選択肢の種類
UENUM(BlueprintType)
enum class SelectionType : uint8
{
	NONE,
	COMMAND,
	TARGET,
	SKILL,
	ITEM,
	RESULT,
};

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

	// カーソル移動
	void SelectMoveTarget(EUIInputType type);
	void SelectConfirn();
	void SelectCancel();

	void OnEnemySelectionChanged(int oldIndex, int newIndex);
	void OnCommandSelectionChanged(int oldIndex, int newIndex);

	// キャラクターを並べる
	void SetCharacterPos(ABattleAreaManager* area);
	// パーティー作成
	void CreateParty(AEnemyBase* enemyBase);

	// 敵を取得
	AEnemyBase* GetEnemyCurrentSelect();
	const TArray<ACharacterBase*>& GetEnemyParty() const { return m_EnemyParty; }

	UBattleTurnManager* GetTurnManager() { return m_TurnManager; }

	// コマンドセレクトのインデックスを設定
	void SetCommandSelectIndex(int index);

private:
	void InitializeSelection();
	void NotifyBattleStart();
	void NotifyBattleEnd();

	// 各キャラクターの状態をリセット
	void ResetBattleState();

	// ターゲットを取得
	const TArray<ACharacterBase*>& GetTargets() const;

	// Attackの処理
	void StartAttack();
	void StartSkill();
	void StartItem();

	void ExecuteAttack(ACharacterBase* target);
	void ShowTargetUI(const TArray<ACharacterBase*>& targets);

	bool BattleWin();
	bool BattleLose();

	void FinishBattle();

private:
	bool m_IsBattle;							 // バトル中か
	bool m_IsVictory;							 //	プレイヤー側が勝負に勝ったか
	SelectionType m_SelectionType;
	FOnTurnChanged m_OnTurnChanged;

	UPROPERTY()
	UBattleLifecycle* m_Lifecycle;
	UPROPERTY()
	UBattleTurnManager* m_TurnManager;

	TArray<ACharacterBase*> m_PlayerParty; // 戦闘時のプレイヤーリスト
	TArray<ACharacterBase*> m_EnemyParty;  // 戦闘時の敵リスト
	TSelectionSystem<ACharacterBase*> m_EnemySelection; // 敵の選択システム
	TSelectionSystem<FBattleCommandData> m_CommandSelection; // コマンドの選択システム
};
