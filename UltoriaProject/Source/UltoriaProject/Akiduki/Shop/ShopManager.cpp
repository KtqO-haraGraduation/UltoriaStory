// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopManager.h"
#include "../Shop/Shop.h"
#include "../ShopMenuWidget/ShopMenuWidget.h"
#include "../ShopMenuWidget/BuyConfirmWidget.h"
#include "../ShopData/ShopData.h"
#include "../ItemData/ItemData.h"
#include "../../System/GameInstance/UltoriaGameInstance.h"
#include "../../System/Controller/PlayPlayerController.h"


void UShopManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	m_CurrentShop = nullptr;
	m_IsOpenedShop = false;
	m_IsBuyConfirmOpen = false;
	m_Money = 900;
	m_UIInputCooldown = 0.0f;
	m_UIOpenInputLockTime = 0.0f;

	// ストーリー進行度(仮)
	m_StoryProgress = 1;

	// 十字キー入力関数を追加
    m_InputDirectionalMap.Add(EUIInputType::UP, &UShopManager::ShopDirectionalUp);
	m_InputDirectionalMap.Add(EUIInputType::DOWN, &UShopManager::ShopDirectionalDown);
	m_InputDirectionalMap.Add(EUIInputType::LEFT, &UShopManager::ShopDirectionalLeft);
	m_InputDirectionalMap.Add(EUIInputType::RIGHT, &UShopManager::ShopDirectionalRight);

	// ゲームインスタンスを取得
	UUltoriaGameInstance* GI = GetWorld()->GetGameInstance<UUltoriaGameInstance>();
	// データテーブルを取得できたか
	if (GI)
	{
		ItemDataTable = GI->ItemDataTable;
		ShopDataTable = GI->ShopDataTable;
	}
}

void UShopManager::InputInteract()
{
	// 現在のショップがされていたら
	if (m_CurrentShop)
	{
		// ショップが開いていなければ
		if (!m_IsOpenedShop)
		{
			// ショップのアイテムリストを構築
			BuildShopItemList();
			UE_LOG(LogTemp, Warning, TEXT("CurrentItems = %d"),
				CurrentItems.Num());
			// ショップを開く
			m_CurrentShop->OpenShop();
			m_IsOpenedShop = true;
			APlayPlayerController* PC = Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());
			if (PC)
			{
				PC->ChangeInputMode(PlayInputMode::SHOP);
				PC->ChangeCursorMode(true);   // 必要ならUI入力モード
			}

			// UI開いた直後入力ロック
			m_UIOpenInputLockTime = 0.2f;
		}
	}
}

void UShopManager::SetCurrentShop(AShop* shop)
{
	m_CurrentShop = shop;
}

void UShopManager::ConfirmShopUI()
{
	// ショップが閉じていれば終了
	if (IsClosedShop())return;

	// ショップ画面を取得
	UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

	// ショップ画面が無ければ終了
	if (!shopUI)return;

	// 購入確認画面を開いてない
	if (!m_IsBuyConfirmOpen)
	{
		// 購入確認画面を開く
		m_IsBuyConfirmOpen = shopUI->ConfirmSelection();
	}
	else
	{
		// 購入確定
		shopUI->BuyCurrentItem();
		m_IsBuyConfirmOpen = false;
	}
}

void UShopManager::CancelShopUI()
{
	// 購入確認画面が開いていれば
	if (m_IsBuyConfirmOpen)
	{
		// ショップ画面を取得
		UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

		// ショップ画面が開いてる　&&　購入確認画面が開いていれば
		if (shopUI && shopUI->BuyConfirmWidget)
		{
			// 親ウィジェットから消す
			shopUI->BuyConfirmWidget->RemoveFromParent();
			// 購入確認画面の中身をnullに
			shopUI->BuyConfirmWidget = nullptr;
		}

		// 購入確認フラグをオフに
		m_IsBuyConfirmOpen = false;
		return;
	}

	// ショップ画面が開いていれば
	if (m_CurrentShop && m_IsOpenedShop && !m_IsBuyConfirmOpen)
	{
		// ショップ画面を閉じる
		m_CurrentShop->CloseShop();

		// ショップ使用中フラグをオフ
		m_IsOpenedShop = false;

		APlayPlayerController* PC = Cast<APlayPlayerController>(GetWorld()->GetFirstPlayerController());
		if (PC)
		{
			PC->ChangeInputMode(PlayInputMode::FIELD);
			PC->ChangeCursorMode(false);
		}
	}
}

void UShopManager::DirectionalInput(EUIInputType type)
{
	UE_LOG(LogTemp, Warning, TEXT("DirectionalInput Called"));
	DirectionalInputFunc* func = m_InputDirectionalMap.Find(type);

	if (func)
	{
		// 入力に対応した関数を実行
		(this->* * func)();
	}
}

void UShopManager::ShopDirectionalUp()
{
	// ショップが閉じていれば終了
	if (IsClosedShop())return;

	// ショップ画面を取得
	UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

	// ショップ画面が無ければ終了
	if (!shopUI)return;

	// 購入確認画面が開いていれば
	if (m_IsBuyConfirmOpen)
	{
		// カウントを1進める
		shopUI->BuyConfirmWidget->ChangeCount(+1);
	}
	else
	{
		// ショップナビを下に下げる
		shopUI->MoveSelection(-1);
	}
}

void UShopManager::ShopDirectionalDown()
{
	// ショップが閉じていれば終了
	if (IsClosedShop())return;

	// ショップ画面を取得
	UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

	// ショップ画面が無ければ終了
	if (!shopUI)return;

	// 購入確認画面が開いていれば
	if (m_IsBuyConfirmOpen)
	{
		// カウントを1戻す
		shopUI->BuyConfirmWidget->ChangeCount(-1);
	}
	else
	{
		// ショップナビを上に上げる
		shopUI->MoveSelection(+1);
	}
}

void UShopManager::ShopDirectionalLeft()
{
	// ショップが閉じていれば終了
	if (IsClosedShop())return;

	// ショップ画面を取得
	UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

	// ショップ画面が無ければ終了
	if (!shopUI)return;

	// 購入確認画面が開いていれば
	if (m_IsBuyConfirmOpen)
	{
		// カウントを10進める
		shopUI->BuyConfirmWidget->ChangeCount(-10);
	}
	else
	{
		// ショップナビを下に下げる
		shopUI->MoveSelection(-5);
	}
}

void UShopManager::ShopDirectionalRight()
{
	// ショップが閉じていれば終了
	if (IsClosedShop())return;

	// ショップ画面を取得
	UShopMenuWidget* shopUI = m_CurrentShop->GetShopMenuWidget();

	// ショップ画面が無ければ終了
	if (!shopUI)return;

	// 購入確認画面が開いていれば
	if (m_IsBuyConfirmOpen)
	{
		// カウントを10戻す
		shopUI->BuyConfirmWidget->ChangeCount(+10);
	}
	else
	{
		// ショップナビを上に上げる
		shopUI->MoveSelection(+5);
	}
}

void UShopManager::BuildShopItemList()
{
	// 現在のアイテムリストを空にする
	CurrentItems.Empty();

	// データテーブルが無ければ終了
	if (!ShopDataTable || !ItemDataTable || !m_CurrentShop)
	{
		return;
	}

	static const FString Context(TEXT("Shop"));

	// ShopDataを全取得
	TArray<FShopData*> ShopRows;
	ShopDataTable->GetAllRows(Context, ShopRows);

	for (FShopData* ShopRow : ShopRows)
	{
		// ShopRowが無ければ次のループへ
		if (!ShopRow)
		{
			continue;
		}
		// ショップカテゴリーが違う
		if (ShopRow->ShopCategory != m_CurrentShop->GetShopCategory())
		{
			continue;
		}
		// ストーリー進行度が足りない
		if (ShopRow->ShopLevel > m_StoryProgress)
		{
			continue;
		}

		// ItemDataをIDで取得
		FItemData* Item = ItemDataTable->FindRow<FItemData>(ShopRow->ItemID, Context);
		if (Item)
		{
			CurrentItems.Add(*Item);
		}
	}
}

