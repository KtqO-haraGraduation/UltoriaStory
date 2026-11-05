// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "../ItemData/ItemData.h"
#include "../ShopData/ShopData.h"
#include "Engine/DataTable.h"
#include "ShopManager.generated.h"

class AShop;
enum class EUIInputType;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UShopManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	void Initialize(FSubsystemCollectionBase& Collection)override;

// 入力関連関数
	// ショップインタラクト
	void InputInteract();
	void SetCurrentShop(AShop* shop);

	// ショップUI操作
	void ConfirmShopUI();
	void CancelShopUI();
	void DirectionalInput(EUIInputType type);
	void ShopDirectionalUp();
	void ShopDirectionalDown();
	void ShopDirectionalLeft();
	void ShopDirectionalRight();

	// 十字キー入力関数呼び出し用関数ポインタ
	typedef void (UShopManager::* DirectionalInputFunc)();

	// ショップのアイテムリストを構築する
	void BuildShopItemList();
	// 現在のショップのアイテムリストを取得する
	const TArray<FItemData>& GetCurrentItems() const { return CurrentItems; }
	
private:
	// ショップ使用中確認関数
	bool IsClosedShop() { return !m_CurrentShop || !m_IsOpenedShop; }
	
private:
	// 現在開いているショップ
	UPROPERTY()
	AShop* m_CurrentShop;

	// スティック入力のロックタイム
	float m_UIOpenInputLockTime;
	// スティック入力のクールタイム
	float m_UIInputCooldown;
	// ショップ使用中フラグ
	bool m_IsOpenedShop;
	// 購入確認フラグ
	UPROPERTY()
	bool m_IsBuyConfirmOpen;

	// 入力関数用マップ
	TMap<EUIInputType, DirectionalInputFunc> m_InputDirectionalMap;

	// アイテムデータテーブル
	UPROPERTY(EditAnywhere)
	UDataTable* ItemDataTable;

	// ショップデータテーブル
	UPROPERTY(EditAnywhere)
	UDataTable* ShopDataTable;
	// 現在のショップのアイテムリスト
	TArray<FItemData> CurrentItems;
};
