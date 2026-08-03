// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleUIManager.h"
#include "Blueprint/UserWidget.h"
#include "BattleUIWidget.h"
#include "BattleResultWidget.h"
#include "CommandUIWidget.h"
#include "CharacterStatusWidget.h"
#include "DamageWidget.h"
#include "../../Battle/BattleGaugeManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Character/CharacterBase.h"
#include "../../Character/Enemy/EnemyBase.h"

void UBattleUIManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// 一旦初期化時に全てのデータをロード
	LoadBattle();
}

void UBattleUIManager::CreateBattleUI()
{
	if (m_BattleUIWidget) return;
	if (!m_BattleWidgetClass) return;

	// バトルUIを生成、表示
	m_BattleUIWidget = CreateWidget<UBattleUIWidget>(GetWorld(), m_BattleWidgetClass);
	if (m_BattleUIWidget) { m_BattleUIWidget->AddToViewport();}

	// 切り替え時に呼ばれる処理を登録
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	if (battleManager)
	{
		battleManager->OnTurnChanged().AddUObject(
			this,
			&UBattleUIManager::OnTurnChanged);
	}
}

void UBattleUIManager::CreateResultUI()
{
	if (m_ResultUIWidget) return;
	if (!m_ResultWidgetClass) return;

	// バトルUIを生成、表示
	m_ResultUIWidget = CreateWidget<UBattleResultWidget>(GetWorld(), m_ResultWidgetClass);
	if (m_ResultUIWidget) { m_ResultUIWidget->AddToViewport(); }
}

void UBattleUIManager::RemoveBattleUI()
{
	if (!m_BattleUIWidget) return;

	UBattleManager* battleManager =
		GetWorld()->GetSubsystem<UBattleManager>();

	UBattleGaugeManager* gaugeManager 
		= GetWorld()->GetSubsystem<UBattleGaugeManager>();

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

void UBattleUIManager::UpdateCommandSelection(int index)
{
	if (!m_BattleUIWidget) return;

	m_BattleUIWidget->GetCommandWidget()->UpdateSelection(index);
}

void UBattleUIManager::SetUIVisible(EBattleUIType type, bool flag)
{
	if (!m_BattleUIWidget) return;

	switch (type)
	{
	case EBattleUIType::STATUS:
		m_BattleUIWidget->GetHBCharacterStatus()->SetVisibility(flag ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		break;
	case EBattleUIType::COMMAND:
		m_BattleUIWidget->GetCommandWidget()->SetVisible(flag);
		break;
	case EBattleUIType::TARGET:
		SetEnemySelectionUI(flag);
		break;
	default:
		break;
	}
}

void UBattleUIManager::CreateDamageWidget(ACharacterBase* target, int damage)
{
	if (!m_DamageWidgetClass) return;

	UDamageWidget* widget = CreateWidget<UDamageWidget>(GetWorld(), m_DamageWidgetClass);

	widget->Init(target, damage);
	if (widget) { widget->AddToViewport(); }
}

void UBattleUIManager::OnTurnChanged(ACharacterBase* prevCharacter, ACharacterBase* currentCharacter)
{
	if (!m_BattleUIWidget || !prevCharacter || !currentCharacter) return;

	bool isPlayerTurn = currentCharacter->GetTeamType() == ETeamType::PLAYER;

	// コマンドUIの 表示 / 非表示 切り替え
	UCommandUIWidget* commandWidget = m_BattleUIWidget->GetCommandWidget();
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

void UBattleUIManager::LoadBattle()
{
	// バトル用のUIクラスを取得
	m_BattleWidgetClass = LoadClass<UBattleUIWidget>(
		nullptr,
		TEXT("/Game/MembarFiles/Matsubara/UI_MC/WBP_BattleUI.WBP_BattleUI_C"));

	// バトル用のUIクラスを取得
	m_ResultWidgetClass = LoadClass<UBattleResultWidget>(
		nullptr,
		TEXT("/Game/MembarFiles/Matsubara/UI_MC/Result/WBP_BattleResultWidget.WBP_BattleResultWidget_C"));

	// ダメージ用のUIクラスを取得
	m_DamageWidgetClass = LoadClass<UDamageWidget>(
		nullptr,
		TEXT("/Game/MembarFiles/Matsubara/UI_MC/Character/WBP_Damage.WBP_Damage_C"));
}

void UBattleUIManager::SetEnemySelectionUI(bool flag)
{
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	AEnemyBase* enemy = battleMg->GetEnemyCurrentSelect();
	if (!enemy) return;

	enemy->SetSelectionUIVisibility(flag);
}
