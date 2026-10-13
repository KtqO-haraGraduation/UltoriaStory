// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "InventoryItemEntryWidget.generated.h"

class UTextBlock;
class UBorder;
/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UInventoryItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// アイテム情報と所持数をEntryWidgetに設定
	void SetItemData(const FItemData& InItemData,int32 InCount);
	// 選択状態を変更
	void SetSelected(bool bSelected);
	// アイテム情報を取得
	const FItemData& GetItemData() const { return ItemData; }

	// アイテム名
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;
	// 所持数
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemCountText;
	// 選択されているか
	UPROPERTY(meta = (BindWidget))
	UBorder* SelectionBorder;

private:

	// ItemData
	FItemData ItemData;
	// 現在表示しているアイテム所持数
	int32 Count = 0;
	
};
