// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUIManager.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIBase.h"
#include "BattleUIWidget.h"
#include "BattleResultWidget.h"
#include "CommandUIWidget.h"
#include "CharacterStatusWidget.h"
#include "DamageWidget.h"
#include "Skill/SkillUIWidget.h"
#include "Item/ItemUIWidget.h"
#include "Speed/SpeedUIWidget.h"
#include "BattleText/BattleTextWidget.h"
#include "../../Battle/BattleGaugeManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Battle/Combat/Selection/BattleSelectioinManager.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Enemy/EnemyBase.h"
#include "../../System/Selection/SelectionSystem.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "Data/BattleUIDataAsset.h"

void UBattleUIManager::CreateBattleUI()
{
	if (m_BattleUIWidget) return;

	// バトルUIを生成、表示
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleDataMg) return;

	TSubclassOf<UBattleUIWidget> battleWidgetClass = battleDataMg->GetBattleUIDataAsset()->BattleUIClass;

	m_BattleUIWidget = CreateWidget<UBattleUIWidget>(GetWorld(), battleWidgetClass);
	
	if (!m_BattleUIWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Battle Null"));
	}
	
	if (m_BattleUIWidget) { m_BattleUIWidget->AddToViewport();}

	// 切り替え時に呼ばれる処理を登録
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	if (battleManager)
	{
		battleManager->OnTurnChanged().AddUObject(
			this,
			&UBattleUIManager::OnTurnChanged);
	}

	// SkillとItemのコマンドUIを非表示にする
	SetUIVisible(EBattleUIType::SKILL, false);
	SetUIVisible(EBattleUIType::ITEM, false);
}

void UBattleUIManager::CreateResultUI()
{
	if (m_ResultUIWidget) return;

	// バトルUIを生成、表示
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleDataMg) return;

	TSubclassOf<UBattleResultWidget> resultClass = battleDataMg->GetBattleUIDataAsset()->ResultWidgetClass;

	m_ResultUIWidget = CreateWidget<UBattleResultWidget>(GetWorld(), resultClass);
	
	if (!m_ResultUIWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Result Null"));
	}
	
	
	
	if (m_ResultUIWidget) { m_ResultUIWidget->AddToViewport(); }
}

void UBattleUIManager::RemoveBattleUI()
{
	if (!m_BattleUIWidget) return;

	UBattleManager* battleManager =
		GetWorld()->GetSubsystem<UBattleManager>();

	if (battleManager)
	{
		battleManager->OnTurnChanged().RemoveAll(this);
	}

	m_BattleUIWidget->RemoveFromParent();
	m_BattleUIWidget = nullptr;
}

void UBattleUIManager::RemoveResultUI()
{
	if (!m_ResultUIWidget) return;

	m_ResultUIWidget->RemoveFromParent();
	m_ResultUIWidget = nullptr;
}

void UBattleUIManager::UpdateSelection(EBattleUIType type, int index)
{
	if (!m_BattleUIWidget) return;

	m_BattleUIWidget->GetSelectionUI(type)->UpdateSelection(index);
}

void UBattleUIManager::SetUIVisible(EBattleUIType type, bool flag)
{
	if (!m_BattleUIWidget) return;

	switch (type)
	{
	case EBattleUIType::STATUS:
	{
		m_BattleUIWidget->GetHBCharacterStatus()->SetVisibility(
			flag ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	}
	
	case EBattleUIType::COMMAND:
	case EBattleUIType::SKILL:
	case EBattleUIType::ITEM:
	{
		UBattleSelectionUIBase* selectionUI =
			m_BattleUIWidget->GetSelectionUI(type);

		if (selectionUI)
		{
			selectionUI->SetVisible(flag);
			selectionUI->SetButtonData();
		}
		break;
	}

	case EBattleUIType::SPEED:
	{
		m_BattleUIWidget->GetSpeedWidget()->SetVisible(flag);
		break;
	}
	case EBattleUIType::TEXT:
	{

		break;
	}
		
	case EBattleUIType::TARGET:
		SetTargetSelectionUI(flag);
		break;
	case EBattleUIType::ALLTARGET:
		SetAllTargetSelectionUI(flag);
		break;
	default:
		break;
	}
}

void UBattleUIManager::CreateDamageWidget(ACharacterBase* target, int damage)
{
	UBattleDataManager* battleDataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (!battleDataMg) return;

	TSubclassOf<UDamageWidget> damageClass = battleDataMg->GetBattleUIDataAsset()->DamageWidgetClass;

	
	UDamageWidget* widget = CreateWidget<UDamageWidget>(GetWorld(), damageClass);
	
	if (!widget)
	{
		UE_LOG(LogTemp, Warning, TEXT("Damage Null"));
	}
	
	if (!widget) return;

	widget->Init(target, damage);
	widget->AddToViewport();
}

void UBattleUIManager::OnTurnChanged(ACharacterBase* prevCharacter, ACharacterBase* currentCharacter)
{
	if (!m_BattleUIWidget || !prevCharacter || !currentCharacter) return;

	bool isPlayerTurn = currentCharacter->GetTeamType() == ETeamType::PLAYER;

	// コマンドUIの 表示 / 非表示 切り替え
	UBattleSelectionUIBase* commandWidget = m_BattleUIWidget->GetSelectionUI(EBattleUIType::COMMAND);
	isPlayerTurn ? commandWidget->ShowUI() : commandWidget->HideUI();

	// プレイヤーパーティーのターンのみ行う
	if (isPlayerTurn || prevCharacter->GetTeamType() == ETeamType::PLAYER)
	{
		UHorizontalBox* statusBox = m_BattleUIWidget->GetHBCharacterStatus();

		UPlayerPartyManager* playerParty = GetWorld()->GetSubsystem<UPlayerPartyManager>();
		if (!playerParty) return;

		for (int i = 0; i < playerParty->GetPlayerParty().Num(); i++)
		{
			UWidget* child = statusBox->GetChildAt(i);

			UCharacterStatusWidget* statusWidget =
				Cast<UCharacterStatusWidget>(child);

			if (!statusWidget || !statusWidget->GetCharacter()) continue;

			if (statusWidget->GetCharacter() == currentCharacter)
			{
				statusWidget->PlayTurnSelect();
			}
			else if(statusWidget->GetCharacter() == prevCharacter)
			{
				statusWidget->ReverseTurnSelect();
			}
		}
	}
}

void UBattleUIManager::SetTargetSelectionUI(bool flag)
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	ACharacterBase* target = battleMg->GetSelectManager()->GetTargetCurrentSelect();
	if (!target) return;

	target->SetSelectionUIVisibility(flag);
}

void UBattleUIManager::SetAllTargetSelectionUI(bool flag)
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	TArray<ACharacterBase*> targets= battleMg->GetSelectManager()->GetTargetSelection().GetTargets();
	for (ACharacterBase* target : targets)
	{
		if (!target) continue;
		target->SetSelectionUIVisibility(flag);
	}
}
