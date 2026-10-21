// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "InventoryItemEntryWidget.generated.h"

class UTextBlock;
class UBorder;
class UImage;
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
	// カテゴリ画像の設定
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Category Icons")
	TSoftObjectPtr<UTexture2D> ConsumableIcon;// 消費アイテム

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Category Icons")
	TSoftObjectPtr<UTexture2D> MaterialIcon;// 素材

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Category Icons")
	TSoftObjectPtr<UTexture2D> WeaponIcon;// 武器

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Category Icons")
	TSoftObjectPtr<UTexture2D> ArmorIcon;// 防具

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Category Icons")
	TSoftObjectPtr<UTexture2D> KeyItemIcon;// キーアイテム

	// カテゴリ画像の表示先
	UPROPERTY(meta = (BindWidget))
	UImage* CategoryIcon;

private:

	// ItemData
	FItemData ItemData;
	// 現在表示しているアイテム所持数
	int32 Count = 0;
	
};
