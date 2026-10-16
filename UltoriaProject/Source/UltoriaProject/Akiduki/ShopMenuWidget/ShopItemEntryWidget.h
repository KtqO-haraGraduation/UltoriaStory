// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ItemData/ItemData.h"
#include "ShopItemEntryWidget.generated.h"

class UTextBlock;
class UBorder;
class UImage;

UCLASS()
class ULTORIAPROJECT_API UShopItemEntryWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	virtual void NativeConstruct() override;

	// アイテムデータ
	FItemData ItemData;

	// データセット
	void SetItemData(const FItemData& Data);

	// 選択状態切り替え
	void SetSelected(bool bSelected);

protected:

    // アイテム名
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_ItemName;
    // アイテム価格
    UPROPERTY(meta = (BindWidget))
    UTextBlock* Text_Price;
    // 選択中の枠
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
};
