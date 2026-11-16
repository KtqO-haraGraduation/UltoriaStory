// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuMain.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Character/CharacterBase.h"
#include "Components/TextBlock.h"

void UMenuMain::OnEnter()
{
	m_Borders = {
		m_BorderStates, m_BorderItems, m_BorderQuest,
		m_BorderOptions, m_BorderSave, m_BorderEnd
	};

	for (auto border : m_Borders)
	{
		if (border)border->SetVisibility(ESlateVisibility::Hidden);
	}

	m_SelectedIndex = 0;
	UpdateSelection_Menu();

	// ステータス表示を更新
	UpdateStatusDisplay();
}

void UMenuMain::OnExit()
{

}

void UMenuMain::NavigateUp()
{
	m_SelectedIndex--;
	if (m_SelectedIndex < 0)
	{
		m_SelectedIndex = m_MaxIndex;
	}
	UpdateSelection_Menu();
}

void UMenuMain::NavigateDown()
{
	m_SelectedIndex++;
	if (m_SelectedIndex > m_MaxIndex)
	{
		m_SelectedIndex = 0;
	}
	UpdateSelection_Menu();
}

void UMenuMain::Confirm()
{
	// 現在のIndexをStateにCastする(+1はStateの最初がMainだから)
	EMenuState nextState = (EMenuState)(m_SelectedIndex + 1);

	// 次のメニューを開く
	UMenuStateManager* menuManger = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (menuManger)
	{
		menuManger->OpenMenu(nextState);
	}
}

void UMenuMain::Cancel()
{
	// メニュー全体を閉じて元のゲーム画面に戻る
	UMenuStateManager* MenuManager = GetGameInstance()->GetSubsystem<UMenuStateManager>();

	// メニューを閉じる
	MenuManager->CloseMenu();
}

void UMenuMain::UpdateSelection_Menu()
{
	for (int32 i = 0; i < m_Borders.Num(); i++)
	{
		if (!m_Borders[i])continue;

		m_Borders[i]->SetVisibility(
			i == m_SelectedIndex
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden
		);
	}
}

void UMenuMain::UpdateStatusDisplay()
{
	// プレイヤーコントローラーを取得
	APlayerController* PC =
		GetGameInstance()->GetFirstLocalPlayerController();
	// 操作中の主人公を取得
	AMainPlayer* Player = Cast<AMainPlayer>(PC->GetPawn());

	// 仲間のステータスを取得
	UPlayerPartyManager* PartyManager =
		GetWorld()->GetSubsystem<UPlayerPartyManager>();

	if (!PartyManager) return;

	// パーティーのキャラクター一覧を取得
	TArray<ACharacterBase*> Party = PartyManager->GetPlayerParty();
		

	//プレイヤー
	// レベル
	m_Lv->SetText(FText::AsNumber(Player->GetLevel()));
	// 現在HP
	m_NowHP->SetText(FText::AsNumber(Player->GetCurrentHP()));
	// 最大HP
	m_MaxHP->SetText(FText::AsNumber(Player->GetMaxHP()));
	// 現在MP
	m_NowMP->SetText(FText::AsNumber(Player->GetCurrentMP()));
	// 最大MP
	m_MaxMP->SetText(FText::AsNumber(Player->GetMaxMP()));
	// 所持金
	UInventorySubsystem* Inventory = GetGameInstance()->GetSubsystem<UInventorySubsystem>();		
	if (Inventory && m_Money)
	{
		m_Money->SetText(FText::AsNumber(Inventory->GetMoney()));
	}

	// 仲間1のステータス
	if (Party.IsValidIndex(1) && IsValid(Party[1]))
	{
		ACharacterBase* Ally1 = Party[1];

		// レベル
		if (m_Ally1Lv)
		{
			m_Ally1Lv->SetText(FText::AsNumber(Ally1->GetLevel()));
		}

		// 現在HP
		if (m_Ally1NowHP)
		{
			m_Ally1NowHP->SetText(FText::AsNumber(Ally1->GetCurrentHP()));
		}

		// 最大HP
		if (m_Ally1MaxHP)
		{
			m_Ally1MaxHP->SetText(FText::AsNumber(Ally1->GetMaxHP()));
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Party数：%d"), Party.Num());

	for (int32 i = 0; i < Party.Num(); i++)
	{
		if (IsValid(Party[i]))
		{
			UE_LOG(LogTemp, Warning,
				TEXT("Party[%d] 名前：%s / Lv：%d / HP：%d / MaxHP：%d"),
				i,
				*Party[i]->GetName(),
				Party[i]->GetLevel(),
				Party[i]->GetCurrentHP(),
				Party[i]->GetMaxHP());
		}
		else
		{
			UE_LOG(LogTemp, Warning,
				TEXT("Party[%d] は無効です"), i);
		}
	}
}
