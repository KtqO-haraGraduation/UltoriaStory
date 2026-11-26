// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleManager.h"
#include "BattleAreaManager.h"
#include "../Character/CharacterBase.h"
#include "../Character/Enemy/EnemyBase.h"
#include "../Character/Data/CharacterDataManager.h"
#include "PlayerPartyManager.h"
#include "Combat/Lifecycle/BattleLifecycle.h"
#include "Combat/Data/BattleDataManager.h"
#include "Encounter/EncounterManager.h"
#include "../UI/Battle/BattleUIManager.h"
#include "../System/Controller/PlayPlayerController.h"
#include "Combat/Components/CombatComponent.h"
#include "Combat/Components/Status/CombatStatusComponent.h"
#include "../System/Sound/SoundManager.h"
#include "../System/Selection/SelectionSystem.h"
#include "Combat/Data/BattleParameter.h"
#include "Combat/Turn/BattleTurnManager.h"
#include "Combat/Selection/BattleSelectioinManager.h"
#include "Combat/Camera/BattleCameraManager.h"

void UBattleManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_IsBattle = false;

	m_Lifecycle = NewObject<UBattleLifecycle>(this);
	m_TurnManager = NewObject<UBattleTurnManager>(this);

	m_SelectManager = NewObject<UBattleSelectioinManager>(this);
	m_SelectManager->Init(this);

	m_CameraManager = NewObject<UBattleCameraManager>(this);
	m_CameraManager->Init(this);
}

void UBattleManager::BattleStart(ACharacterBase* enemy, bool isPreemptive)
{
	if (m_IsBattle) return;
	m_IsBattle = true;

	USoundManager* soundMg = GetWorld()->GetGameInstance()->GetSubsystem<USoundManager>();
	if (soundMg) soundMg->PlayBGM("BGM_Battle");

	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	//if (encounter) encounter->SetEncounterFogEnabled(false);

	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();

	m_TurnManager->Start();

	AEnemyBase* enemyBase = Cast<AEnemyBase>(enemy);
	m_Lifecycle->Start(this, enemyBase);

	// 各キャラのバトル開始処理
	NotifyBattleStart();

	m_OnTurnChanged.Broadcast(m_TurnManager->GetCurrentCharacter(), m_TurnManager->GetCurrentCharacter());

	// 先制攻撃（仮実装）
	if (isPreemptive)
	{
		for (auto& e : m_EnemyParty)
		{
			if (!e) continue;

			UCombatComponent* combat = e->GetComponent<UCombatComponent>();
			if (combat) combat->TakeDamage(150);
		}
	}

	// 仮実装（最終的には消す）
	if (m_TurnManager->GetCurrentCharacter()->GetTeamType() == ETeamType::ENEMY)
	{
		m_SelectManager->SetSelectionType(SelectionType::NONE);
		StartAttack();
	}
	if (m_TurnManager->GetCurrentCharacter()->GetTeamType() == ETeamType::PLAYER)
	{
		// カメラを設定


		m_SelectManager->SetSelectionType(SelectionType::COMMAND);
	}

	m_CameraManager->SetActionCameraPos();
}

void UBattleManager::NextTurn()
{
	if (m_TurnManager->GetCharacters().IsEmpty()) return;
	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();

	// 状態異常の更新
	m_TurnManager->UpdateTurnEnd();

	if (BattleWin() || BattleLose())
	{
		// 決着がついたらバトル終了
		BattleEnd();
		return;
	}

	m_TurnManager->NextTurn();
	m_CameraManager->SetActionCameraPos();

	if (m_TurnManager->GetCurrentCharacter()->GetTeamType() == ETeamType::PLAYER)
	{
		m_SelectManager->SetSelectionType(SelectionType::COMMAND);
		
	}
	else if(m_TurnManager->GetCurrentCharacter()->GetTeamType() == ETeamType::ENEMY)
	{
		m_SelectManager->SetSelectionType(SelectionType::NONE);
		StartAttack();

	}

	m_OnTurnChanged.Broadcast(m_TurnManager->GetPrevCharacter(), m_TurnManager->GetCurrentCharacter());
}

void UBattleManager::BattleEnd()
{
	if (!m_IsBattle) return;

	UBattleUIManager* uiManager = GetWorld()->GetSubsystem<UBattleUIManager>();

	m_SelectManager->SetSelectionType(SelectionType::RESULT);

	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	if (encounter) encounter->ResetFieldCamera();

	// 勝利と負けで処理を分ける
	if (m_IsVictory)
	{
		// 勝った時の処理

		// リザルト画面を出す
		if(uiManager)uiManager->CreateResultUI();
	}
	else
	{
		// 負けた時の処理

		// ゲームオーバーUIを出す（一時的）

		FinishBattle();
	}
}

ACharacterBase* UBattleManager::GetCurrentCharacter()
{
	return m_TurnManager->GetCurrentCharacter();
}

void UBattleManager::ExecuteCommand(ECommandType type)
{
	
	UCombatComponent* combat = m_TurnManager->GetCurrentCharacter()->GetComponent<UCombatComponent>();
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!combat || !uiMg) return;

	switch (type)
	{
	case ECommandType::ATTACK:
		StartAttack();
		break;
	case ECommandType::GUARD:
		combat->Guard();
		break;
	case ECommandType::ARCANA:
		break;
	case ECommandType::SKILL:
	{
		m_SelectManager->SetSelectionType(SelectionType::SKILL);
		m_SelectManager->SetSkillTarget();

		// スキル用のボードを表示
		uiMg->SetUIVisible(EBattleUIType::COMMAND, false);
		uiMg->SetUIVisible(EBattleUIType::SKILL, true);
		break;
	}
	case ECommandType::ITEM:
	{
		m_SelectManager->SetSelectionType(SelectionType::ITEM);
		m_SelectManager->SetItemTarget();

		// アイテム用のボードを表示
		uiMg->SetUIVisible(EBattleUIType::COMMAND, false);
		uiMg->SetUIVisible(EBattleUIType::ITEM, true);
		break;
	}
	case ECommandType::ESCAPE:
	{
		bool isEscape = combat->Escape();
		if (isEscape)
		{
			m_IsVictory = false;
			BattleEnd();
		}
		break;
	}
	}
}

void UBattleManager::SetCharacterPos(ABattleAreaManager* area)
{
	if (!area) return;

	// 各座標を取得
	TArray<FBattleTransform> playersTransform = area->GetBattleTransform(m_PlayerParty.Num(), false);
	TArray<FBattleTransform> enemysTransform  = area->GetBattleTransform(m_EnemyParty.Num(),  true);

	UCharacterDataManager* dataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!dataMg) return;

	for (int i = 0; i < m_PlayerParty.Num(); i++)
	{
		m_PlayerParty[i]->SetActorLocationAndRotation(playersTransform[i].pos, playersTransform[i].rot);
	}

	for (int i = 0; i < m_EnemyParty.Num(); i++)
	{
		ACharacterBase* enemy = m_EnemyParty[i];
		FEnemyMasterData enemyData = dataMg->GetEnemyData(enemy->GetID());

		enemy->SetActorLocationAndRotation(enemysTransform[i].pos, enemysTransform[i].rot);
		//enemy->GetMesh()->SetRelativeScale3D(FVector(enemyData.BattleSize));
	}
}

void UBattleManager::NotifyBattleStart()
{
	for (ACharacterBase* character : m_TurnManager->GetCharacters())
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character)combat->BattleStart();
	}
}

void UBattleManager::NotifyBattleEnd()
{
	for (ACharacterBase* character : m_TurnManager->GetCharacters())
	{
		if (!character) return;
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (combat->GetCurrentState() == CombatState::DEAD) continue;

		combat->BattleEnd();
	}

	// プレイヤー敗北時は敵を消す
	if (!m_IsVictory)
	{
		for (ACharacterBase* enemy : m_EnemyParty)
		{
			if (enemy) enemy->Destroy();
		}
	}
}

void UBattleManager::ResetBattleState()
{
	for (auto character : m_TurnManager->GetCharacters())
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character)combat->ResetState();
	}
}

TArray<ACharacterBase*> UBattleManager::GetTargets(ETargetTeam team) const
{
	TArray<ACharacterBase*> result;

	ACharacterBase* currentCharacter = m_TurnManager->GetCurrentCharacter();
	if (!currentCharacter) return result;

	if (team == ETargetTeam::SELF)
	{
		// 時分自身を取得
		result.Add(currentCharacter);
		m_CameraManager->SetTargetCameraPos(team, result);
		return result;
	}
	else if (team == ETargetTeam::ALLY)
	{
		// プレイヤーパーティーを取得
		m_CameraManager->SetTargetCameraPos(team, m_PlayerParty);
		return m_PlayerParty;
	}
	else if (team == ETargetTeam::ENEMY)
	{
		// 敵パーティーを取得
		m_CameraManager->SetTargetCameraPos(team, m_EnemyParty);
		return m_EnemyParty;
	}
	return result;
}

TArray<ACharacterBase*> UBattleManager::GetActiveTargets(ETargetTeam team) const
{
	auto targets = GetTargets(team);
	// 生きてる敵を格納するリスト
	TArray<ACharacterBase*> activeTargets = {};

	for (auto target : targets)
	{
		if (!target) continue;
		UCombatComponent* combat = target->GetComponent<UCombatComponent>();

		if (combat->GetCurrentState() != CombatState::DEAD)
		{
			activeTargets.Add(target);
		}
	}
	return activeTargets;
}

void UBattleManager::StartAttack()
{
	ACharacterBase* currentCharacter = m_TurnManager->GetCurrentCharacter();
	if (!currentCharacter) return;

	ETargetTeam targetTeam = currentCharacter->GetTeamType() == ETeamType::PLAYER ? 
		ETargetTeam::ENEMY : ETargetTeam::ALLY;

	// 生きてる敵を格納
	TArray<ACharacterBase*> activeTargets = GetActiveTargets(targetTeam);

	if (activeTargets.Num() == 1)
	{
		ExecuteAttack(activeTargets[0]);
	}
	else if(activeTargets.Num() > 1)
	{
		// 仮実装、アクティブの先頭のキャラを攻撃する
		if (m_TurnManager->GetCurrentCharacter()->GetTeamType() == ETeamType::ENEMY)
		{
			ExecuteAttack(activeTargets[0]);
		}

		ShowTargetUI(activeTargets, ETargetType::SINGLE);
	}
}

void UBattleManager::ExecuteAttack(ACharacterBase* target)
{
	if (!target) return;
	UCombatComponent* combat = m_TurnManager->GetCurrentCharacter()->GetComponent<UCombatComponent>();

	combat->Attack(target);

	if (GetCurrentCharacter()->GetTeamType() == ETeamType::PLAYER)
	{
		combat->NextTurn();
	}
}

void UBattleManager::ShowAttackTargetUI(const TArray<ACharacterBase*>& targets)
{
	m_SelectManager->SetSelectionType(SelectionType::TARGET);
	m_SelectManager->SetCharacterTarget(targets);

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, false);
	uiMg->SetUIVisible(EBattleUIType::TARGET, true);
}

void UBattleManager::ShowTargetUI(const TArray<ACharacterBase*>& targets, ETargetType type)
{
	m_SelectManager->SetSelectionType(SelectionType::TARGET);
	m_SelectManager->SetCharacterTarget(targets);

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

    uiMg->SetUIVisible(EBattleUIType::SKILL, false);
    uiMg->SetUIVisible(EBattleUIType::ITEM, false);

	if (type == ETargetType::SINGLE)
	{
		uiMg->SetUIVisible(EBattleUIType::TARGET, true);
	}
	else if (type == ETargetType::ALL)
	{
		uiMg->SetUIVisible(EBattleUIType::ALLTARGET, true);
	}
}

bool UBattleManager::BattleWin()
{
	// 全ての敵を倒したら
	for (auto character : m_TurnManager->GetCharacters())
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character->GetTeamType() == ETeamType::ENEMY &&
			combat->GetCurrentState() != CombatState::DEAD)
		{
			return false;
		}
	}

	m_IsVictory = true;
	return true;
}

bool UBattleManager::BattleLose()
{
	// 全てのプレイヤーが倒れたら
	for (auto character : m_TurnManager->GetCharacters())
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character->GetTeamType() == ETeamType::PLAYER &&
			combat->GetCurrentState() != CombatState::DEAD)
		{
			return false;
		}
	}

	m_IsVictory = false;
	return true;
}

void UBattleManager::FinishBattle()
{
	UBattleUIManager* uiManager = GetWorld()->GetSubsystem<UBattleUIManager>();
	UEncounterManager* encounterMg = GetWorld()->GetSubsystem<UEncounterManager>();

	if (uiManager && m_IsVictory) uiManager->RemoveResultUI();

	m_Lifecycle->End();
	NotifyBattleEnd();

	m_TurnManager->End();

	// 全キャラの状態をリセット
	ResetBattleState();

	// プレイヤーパーティーを削除
	UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!playerPartyMg) return;
	playerPartyMg->DeletePlayerParty();
	for (ACharacterBase* enemy : m_EnemyParty)
	{
		if (enemy)
		{
			enemy->Destroy();
		}
	}

	// 初期化
	m_PlayerParty.Empty();
	m_EnemyParty.Empty();
	m_IsBattle = false;
	m_IsVictory = false;

	/*UStageManager* stageMg = GetWorld()->GetSubsystem<UStageManager>();
	if (stageMg) stageMg->ShowFieldLv();*/
	if (encounterMg) encounterMg->EndBattle();

	USoundManager* soundMg = GetWorld()->GetGameInstance()->GetSubsystem<USoundManager>();
	if (soundMg) soundMg->PlayBGM("BGM_Field_1");
}

void UBattleManager::CreateParty(AEnemyBase* enemyBase)
{
	// 各パーティーを List に追加
	UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!playerPartyMg) return;

	// プレイヤーのパーティーを作成
	playerPartyMg->CreatePlayerParty();

	m_PlayerParty = playerPartyMg->GetPlayerParty();
	m_TurnManager->AddParty(m_PlayerParty);

	m_EnemyParty = enemyBase->CreateEnemyParty();
	m_TurnManager->AddParty(m_EnemyParty);
}
