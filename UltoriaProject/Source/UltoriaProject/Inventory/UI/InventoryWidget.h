// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Blueprint/UserWidget.h"
#include "../../UI/Menu/MenuBase.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UDataTable;
class UInventoryItemEntryWidget;
class UItemUseConfirmWidget;
class UImage;

UCLASS()
class ULTORIAPROJECT_API UInventoryWidget : public UMenuBase
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;

public:

	// 上方向入力を受け取ったときに呼ばれる
	virtual void NavigateUp() override;

	// 下方向入力を受け取ったときに呼ばれる
	virtual void NavigateDown() override;

	// 決定ボタンを押したときに呼ばれる
	virtual void Confirm() override;

	// キャンセルボタンを押したときに呼ばれる
	virtual void Cancel() override;

	// 選択移動
	void MoveSelection(int32 Direction);

	// 選択更新
	void UpdateSelection();

	// アイテムセットアップ
	void SetupItems();

	// 所持金更新
	void UpdateMoney();

public:

	// ScrollBox
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemList;

	// アイテム名
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	// アイテム説明文
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	// 選択中アイテムのアイコン
	UPROPERTY(meta = (BindWidget))
	UImage* ItemImage;

	// プレイヤーの所持金
	UPROPERTY(meta = (BindWidget))
	UTextBlock* MoneyText;

	// アイテム行Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryItemEntryClass;

	UPROPERTY()
	TArray<UInventoryItemEntryWidget*> ItemEntries;

	// 現在選択
	int32 CurrentIndex = 0;
protected:

	// 確認ウィジェットのクラス
	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	TSubclassOf<UItemUseConfirmWidget> ItemUseConfirmWidgetClass;

	// 現在表示している確認ウィジェット
	UPROPERTY()
	TObjectPtr<UItemUseConfirmWidget> ItemUseConfirmWidget;

	// アイテム使用確認中かどうか
	bool bIsConfirmingItemUse = false;

	// 使用確認中のアイテムデータを保持
	FItemData PendingItemData;

	// 確認画面を開く
	void OpenItemUseConfirm(const FItemData& ItemData);

	// アイテムを実際に使用する
	void ExecutePendingItemUse();
};
