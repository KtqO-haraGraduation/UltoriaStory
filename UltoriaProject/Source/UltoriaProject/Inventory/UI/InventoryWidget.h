// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UDataTable;

UCLASS()
class ULTORIAPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;

public:
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	TSubclassOf<UUserWidget> ShopItemEntryClass;

	//UPROPERTY()
	//TArray<UShopItemEntryWidget*> ItemEntries;

	// 現在選択
	int32 CurrentIndex = 0;
};
