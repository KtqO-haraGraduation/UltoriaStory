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

	// アイテム行Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> InventoryItemEntryClass;

	UPROPERTY()
	TArray<UInventoryItemEntryWidget*> ItemEntries;

	// 現在選択
	int32 CurrentIndex = 0;
};
