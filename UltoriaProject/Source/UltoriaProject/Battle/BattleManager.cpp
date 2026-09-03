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

void UBattleManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_Lifecycle = NewObject<UBattleLifecycle>(this);
}

void UBattleManager::BattleStart(ACharacterBase* player, ACharacterBase* enemy, bool isPreemptive)
{
	if (m_IsBattle) return;
	m_IsBattle = true;
	m_Characters.Empty();
	m_CurrentIndex = 0;

	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	//if (encounter) encounter->SetEncounterFogEnabled(false);

	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();

	AEnemyBase* enemyBase = Cast<AEnemyBase>(enemy);
	m_Lifecycle->Start(this, enemyBase);

	// カーソル移動時のイベント設定
	InitializeSelection();

	// 各キャラのバトル開始処理
	NotifyBattleStart();

	m_OnTurnChanged.Broadcast(m_PrevCharacter, m_CurrentCharacter);

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
	if (m_CurrentCharacter->GetTeamType() == ETeamType::ENEMY)
	{
		m_SelectionType = SelectionType::NONE;
		StartAttack();
	}
	if (m_CurrentCharacter->GetTeamType() == ETeamType::PLAYER)
	{
		m_SelectionType = SelectionType::COMMAND;
		m_CommandSelection.SetTargets(dataManager->GetCommandDatas());
	}
}

void UBattleManager::NextTurn()
{
	if (m_Characters.IsEmpty()) return;
	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();

	if (BattleWin())
	{
		// 勝利処理
		BattleEnd();
		return;
	}

	if (BattleLose())
	{
		// 負け処理
		BattleEnd();
		return;
	}

	// 前回のキャラの状態をリセット（死んでたら無視）
	/*UCombatComponent* oldCombat = m_CurrentCharacter->GetComponent<UCombatComponent>();
	oldCombat->ResetState();*/

	// 状態の更新
	UCombatStatusComponent* statusComponent = m_CurrentCharacter->GetComponent<UCombatStatusComponent>();
	if (statusComponent) statusComponent->UpdateTurn();

	m_CurrentIndex++;
	if (m_CurrentIndex >= m_Characters.Num())
	{
		m_CurrentIndex = 0;
	}

	// 現在行動中のキャラを設定
	m_CurrentCharacter = m_Characters[m_CurrentIndex];

	UCombatComponent* combat = m_CurrentCharacter->GetComponent<UCombatComponent>();
	// 死んでたら強制で次のキャラのターンへ
	if (combat->GetCurrentState() == CombatState::DEAD)
	{
		NextTurn();
		return;
	}

	//combat->SetCurrentState(CombatState::IDLE);

	UE_LOG(LogTemp, Warning,
		TEXT("Current : %s"),
		*m_CurrentCharacter->GetName());

	if (m_CurrentCharacter->GetTeamType() == ETeamType::PLAYER)
	{
		m_SelectionType = SelectionType::COMMAND;
		m_CommandSelection.SetTargets(dataManager->GetCommandDatas());
	}
	else if(m_CurrentCharacter->GetTeamType() == ETeamType::ENEMY)
	{
		m_SelectionType = SelectionType::NONE;
		StartAttack();
	}

	m_OnTurnChanged.Broadcast(m_PrevCharacter, m_CurrentCharacter);

	// 前回のキャラを記録
	m_PrevCharacter = m_CurrentCharacter;

	UE_LOG(LogTemp, Warning, TEXT("UI Turn Changed"));
}

void UBattleManager::BattleEnd()
{
	if (!m_IsBattle) return;

	UBattleUIManager* uiManager = GetWorld()->GetSubsystem<UBattleUIManager>();

	m_SelectionType = SelectionType::RESULT;

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

void UBattleManager::AddCharacter(ACharacterBase* character)
{
	if (!character) return;
	m_Characters.Add(character);
}

void UBattleManager::AddParty(const TArray<ACharacterBase*>& characters)
{
	m_Characters.Append(characters);
}

void UBattleManager::ExecuteCommand(ECommandType type)
{
	if (!m_CurrentCharacter) return;
	UCombatComponent* combat = m_Characters[m_CurrentIndex]->GetComponent<UCombatComponent>();

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
		m_SelectionType = SelectionType::SKILL;
		combat->Skill();
		break;
	}
	case ECommandType::ITEM:
	{
		m_SelectionType = SelectionType::ITEM;
		combat->Item();
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

void UBattleManager::SelectMoveTarget(EUIInputType type)
{
	switch (m_SelectionType)
	{
	case SelectionType::COMMAND:
	{
		switch (type)
		{
		case EUIInputType::UP:
			m_CommandSelection.Move(-1);
			break;
		case EUIInputType::DOWN:
			m_CommandSelection.Move(1);
			break;
		}
		break;
	}
	case SelectionType::TARGET:
	{
		switch (type)
		{
		case EUIInputType::LEFT:
			m_EnemySelection.Move(-1);
			break;
		case EUIInputType::RIGHT:
			m_EnemySelection.Move(1);
			break;
		}
		break;
	}
	case SelectionType::SKILL:
	{
		break;
	}
	case SelectionType::ITEM:
	{
		break;
	}
	case SelectionType::RESULT:
	{
		break;
	}
	case SelectionType::NONE:
	{
		break;
	}
	}
}

void UBattleManager::SelectConfirn()
{
	switch (m_SelectionType)
	{
	case SelectionType::COMMAND:
	{
		const FBattleCommandData& command = m_CommandSelection.GetCurrent();
		ExecuteCommand(command.Type);
		break;
	}
	case SelectionType::TARGET:
	{
		// 一旦、選択した敵に攻撃処理
		ExecuteAttack(m_EnemySelection.GetCurrent());
		UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
		if (!uiMg) break;
		uiMg->SetUIVisible(EBattleUIType::TARGET, false);
		break;
	}
	case SelectionType::SKILL:
	{
		break;
	}
	case SelectionType::ITEM:
	{
		break;
	}
	case SelectionType::RESULT:
	{
		FinishBattle();
		break;
	}
	case SelectionType::NONE:
	{
		break;
	}
	}
}

void UBattleManager::SelectCancel()
{
	switch (m_SelectionType)
	{
	case SelectionType::COMMAND:
	{
		break;
	}
	case SelectionType::TARGET:
	{
		// コマンド選択に戻る
		m_SelectionType = SelectionType::COMMAND;

		UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
		if (!uiMg) break;
		uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
		uiMg->SetUIVisible(EBattleUIType::TARGET, false);
		break;
	}
	case SelectionType::SKILL:
	{
		break;
	}
	case SelectionType::ITEM:
	{
		break;
	}
	case SelectionType::RESULT:
	{
		break;
	}
	case SelectionType::NONE:
	{
		break;
	}
	}
}

void UBattleManager::OnEnemySelectionChanged(int oldIndex, int newIndex)
{
	const auto& targets = m_EnemySelection.GetTargets();

	if (!targets.IsValidIndex(oldIndex) ||
		!targets.IsValidIndex(newIndex))
	{
		return;
	}

	AEnemyBase* oldEnemy = Cast<AEnemyBase>(targets[oldIndex]);
	AEnemyBase* newEnemy = Cast<AEnemyBase>(targets[newIndex]);

	if (oldEnemy)
	{
		oldEnemy->SetSelectionUIVisibility(false);
	}
	if (newEnemy)
	{
		newEnemy->SetSelectionUIVisibility(true);
	}
}

void UBattleManager::OnCommandSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateCommandSelection(newIndex);
}

void UBattleManager::SetCharacterPos(ABattleAreaManager* area)
{
	if (!area) return;

	// 各座標を取得
	TArray<FBattleTransform> player = area->GetBattleTransform(m_PlayerParty.Num(), false);
	TArray<FBattleTransform> enemy  = area->GetBattleTransform(m_EnemyParty.Num(),  true);

	for (int i = 0; i < m_PlayerParty.Num(); i++)
	{
		m_PlayerParty[i]->SetActorLocationAndRotation(player[i].pos, player[i].rot);
	}

	for (int i = 0; i < m_EnemyParty.Num(); i++)
	{
		m_EnemyParty[i]->SetActorLocationAndRotation(enemy[i].pos, enemy[i].rot);
	}
}

void UBattleManager::SortBySpeedDescending()
{
	m_Characters.Sort(
		[](const ACharacterBase& a,
			const ACharacterBase& b)
		{
			return a.GetBaseStatus().m_SPD > b.GetBaseStatus().m_SPD;
		});

	// 一番最初のキャラクターを保存
	if (!m_Characters.IsEmpty())
	{
		m_CurrentCharacter = m_Characters[0];
	}
}

AEnemyBase* UBattleManager::GetEnemyCurrentSelect()
{
	return Cast<AEnemyBase>(m_EnemySelection.GetCurrent());
}

void UBattleManager::SetCommandSelectIndex(int index)
{
	m_CommandSelection.SetCurrentIndex(index);
}

void UBattleManager::InitializeSelection()
{
	// 登録する前に前回のイベントを削除
	m_EnemySelection.m_OnSelectionChanged.Unbind();
	m_CommandSelection.m_OnSelectionChanged.Unbind();

	// 敵のカーソル移動時のイベント
	m_EnemySelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleManager::OnEnemySelectionChanged
	);

	// コマンドのカーソル移動時のイベント
	m_CommandSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleManager::OnCommandSelectionChanged
	);
}

void UBattleManager::NotifyBattleStart()
{
	for (ACharacterBase* character : m_Characters)
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character)combat->BattleStart();
	}
}

void UBattleManager::NotifyBattleEnd()
{
	for (ACharacterBase* character : m_Characters)
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
	for (auto character : m_Characters)
	{
		UCombatComponent* combat = character->GetComponent<UCombatComponent>();
		if (character)combat->ResetState();
	}
}

const TArray<ACharacterBase*>& UBattleManager::GetTargets() const
{
	static TArray<ACharacterBase*> empty;

	if (!m_CurrentCharacter) return empty;

	bool isPlayerTurn = m_CurrentCharacter->GetTeamType() == ETeamType::PLAYER;

	return isPlayerTurn ? m_EnemyParty : m_PlayerParty;
}

void UBattleManager::StartAttack()
{
	auto targets = GetTargets();
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

	if (activeTargets.Num() == 1)
	{
		ExecuteAttack(activeTargets[0]);
	}
	else
	{
		// 仮実装、アクティブの先頭のキャラを攻撃する
		if (m_CurrentCharacter->GetTeamType() == ETeamType::ENEMY)
		{
			ExecuteAttack(activeTargets[0]);
		}

		ShowTargetUI(activeTargets);
	}
}

void UBattleManager::ExecuteAttack(ACharacterBase* target)
{
	if (!target) return;
	UCombatComponent* combat = m_CurrentCharacter->GetComponent<UCombatComponent>();

	combat->Attack(target);
}

void UBattleManager::ShowTargetUI(const TArray<ACharacterBase*>& targets)
{
	m_SelectionType = SelectionType::TARGET;
	m_EnemySelection.SetTargets(targets);

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;
    uiMg->SetUIVisible(EBattleUIType::COMMAND, false);
	uiMg->SetUIVisible(EBattleUIType::TARGET, true);
}

bool UBattleManager::BattleWin()
{
	// 全ての敵を倒したら
	for (auto character : m_Characters)
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
	for (auto character : m_Characters)
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

	if (uiManager && m_IsVictory) uiManager->RemoveResultUI();

	m_Lifecycle->End();
	NotifyBattleEnd();

	// 全キャラの状態をリセット
	ResetBattleState();

	// 初期化
	m_CurrentCharacter = nullptr;
	m_PlayerParty.Empty();
	m_EnemyParty.Empty();
	m_Characters.Empty();
	m_IsBattle = false;
	m_IsVictory = false;

	UEncounterManager* encounterMg = GetWorld()->GetSubsystem<UEncounterManager>();
	if (encounterMg)
	{
		// バトルLVを非表示
		encounterMg->SetBattleLevelVisible(false);
		// フィールドLVを表示
		encounterMg->SetFieldLevelVisible(true);
	}

	// プレイヤーパーティーを削除
	UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!playerPartyMg) return;

	playerPartyMg->DeletePlayerParty();
}

void UBattleManager::CreateParty(AEnemyBase* enemyBase)
{
	// 各パーティーを List に追加
	UPlayerPartyManager* playerPartyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (!playerPartyMg) return;

	// プレイヤーのパーティーを作成
	playerPartyMg->CreatePlayerParty();

	m_PlayerParty = playerPartyMg->GetPlayerParty();
	AddParty(m_PlayerParty);

	m_EnemyParty = enemyBase->CreateEnemyParty();
	AddParty(m_EnemyParty);
}
