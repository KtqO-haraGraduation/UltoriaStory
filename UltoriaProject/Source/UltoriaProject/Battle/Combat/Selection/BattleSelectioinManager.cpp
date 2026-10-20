// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleSelectioinManager.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../System/Controller/PlayPlayerController.h"
#include "../../../Character/CharacterBase.h"
#include "../../../Character/Enemy/EnemyBase.h"
#include "../../../Job/Skill/SkillBaseComponent.h"
#include "../../BattleManager.h"
#include "../Data/BattleDataManager.h"

void UBattleSelectioinManager::Init(UBattleManager* battleMg)
{
	if (!battleMg) return;
	m_BattleManager = battleMg;
	m_SelectionType = SelectionType::NONE;

	// 登録する前に前回のイベントを削除
	m_EnemySelection.m_OnSelectionChanged.Unbind();
	m_CommandSelection.m_OnSelectionChanged.Unbind();
	m_SkillSelection.m_OnSelectionChanged.Unbind();

	// 敵のカーソル移動時のイベント
	m_EnemySelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnEnemySelectionChanged
	);

	// コマンドのカーソル移動時のイベント
	m_CommandSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnCommandSelectionChanged
	);

	// スキルのカーソル移動時のイベント
	m_SkillSelection.m_OnSelectionChanged.BindUObject(
		this,
		&UBattleSelectioinManager::OnSkillSelectionChanged
	);

	// コマンドの選択データを入れる
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if(battleDataMg) m_CommandSelection.SetTargets(battleDataMg->GetCommandDatas());

	m_MoveFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::MoveCommand);
	m_MoveFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::MoveTarget);
	m_MoveFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::MoveSkill);
	m_MoveFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::MoveItem);
	m_MoveFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::MoveResult);

	m_ConfrmFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::ConfirmCommand);
	m_ConfrmFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::ConfirmTarget);
	m_ConfrmFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::ConfirmSkill);
	m_ConfrmFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::ConfirmItem);
	m_ConfrmFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::ConfirmResult);

	m_CancelFunc.Add(SelectionType::COMMAND, &UBattleSelectioinManager::CancelCommand);
	m_CancelFunc.Add(SelectionType::TARGET, &UBattleSelectioinManager::CancelTarget);
	m_CancelFunc.Add(SelectionType::SKILL, &UBattleSelectioinManager::CancelSkill);
	m_CancelFunc.Add(SelectionType::ITEM, &UBattleSelectioinManager::CancelItem);
	m_CancelFunc.Add(SelectionType::RESULT, &UBattleSelectioinManager::CancelResult);
}

void UBattleSelectioinManager::SelectMoveTarget(EUIInputType type)
{
	if (FSelectionMoveFunc* func = m_MoveFunc.Find(m_SelectionType))
	{
		(this->* * func)(type);
	}
}

void UBattleSelectioinManager::SelectConfirm()
{
	if (FSelectionConfrmFunc* func = m_ConfrmFunc.Find(m_SelectionType))
	{
		(this->* * func)();
	}
}

void UBattleSelectioinManager::SelectCancel()
{
	if (FSelectionCancelFunc* func = m_CancelFunc.Find(m_SelectionType))
	{
		(this->* * func)();
	}
}

AEnemyBase* UBattleSelectioinManager::GetEnemyCurrentSelect()
{
	return Cast<AEnemyBase>(m_EnemySelection.GetCurrent());
}

void UBattleSelectioinManager::SetEnemyTarget(const TArray<ACharacterBase*>& targets)
{
	m_EnemySelection.SetTargets(targets);
}

void UBattleSelectioinManager::SetSkillTarget()
{
	ACharacterBase* currentCharacter = m_BattleManager->GetCurrentCharacter();
	if (!currentCharacter) return;

	USkillBaseComponent* skillComponent = currentCharacter->GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	m_SkillSelection.SetTargets(skillComponent->GetSkills());
}

void UBattleSelectioinManager::SetCommandSelectIndex(int index)
{
	m_CommandSelection.SetCurrentIndex(index);
}

void UBattleSelectioinManager::SetSkillSelectIndex(int index)
{
	m_SkillSelection.SetCurrentIndex(index);
}

void UBattleSelectioinManager::OnEnemySelectionChanged(int oldIndex, int newIndex)
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

void UBattleSelectioinManager::OnCommandSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateSelection(EBattleUIType::COMMAND, newIndex);
}

void UBattleSelectioinManager::OnSkillSelectionChanged(int oldIndex, int newIndex)
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->UpdateSelection(EBattleUIType::SKILL, newIndex);
}

// --------- Move関数 --------- //
void UBattleSelectioinManager::MoveCommand(EUIInputType type)
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
}

void UBattleSelectioinManager::MoveTarget(EUIInputType type)
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
}

void UBattleSelectioinManager::MoveSkill(EUIInputType type)
{
	switch (type)
	{
	case EUIInputType::UP:
		m_SkillSelection.Move(-1);
		break;
	case EUIInputType::DOWN:
		m_SkillSelection.Move(1);
		break;
	}
}

void UBattleSelectioinManager::MoveItem(EUIInputType type)
{

}

void UBattleSelectioinManager::MoveResult(EUIInputType type)
{

}

// --------- Confirm関数 --------- //
void UBattleSelectioinManager::ConfirmCommand()
{
	const FBattleCommandData& command = m_CommandSelection.GetCurrent();
	m_BattleManager->ExecuteCommand(command.Type);
}

void UBattleSelectioinManager::ConfirmTarget()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	// 一旦、選択した敵に攻撃処理
	m_BattleManager->ExecuteAttack(m_EnemySelection.GetCurrent());
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
}

void UBattleSelectioinManager::ConfirmSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
}

void UBattleSelectioinManager::ConfirmItem()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	uiMg->SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleSelectioinManager::ConfirmResult()
{
	m_BattleManager->FinishBattle();
}

// --------- Cancel関数 --------- //
void UBattleSelectioinManager::CancelCommand()
{

}

void UBattleSelectioinManager::CancelTarget()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	// コマンド選択に戻る
	m_SelectionType = SelectionType::COMMAND;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::TARGET, false);
}

void UBattleSelectioinManager::CancelSkill()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::SKILL, false);
}

void UBattleSelectioinManager::CancelItem()
{
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (!uiMg) return;

	m_SelectionType = SelectionType::COMMAND;

	uiMg->SetUIVisible(EBattleUIType::COMMAND, true);
	uiMg->SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleSelectioinManager::CancelResult()
{

}
