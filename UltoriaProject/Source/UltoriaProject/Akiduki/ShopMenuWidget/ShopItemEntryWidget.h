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
    // アイテムアイコン
    UPROPERTY(meta = (BindWidget))
    UImage* Image_ItemIcon;
    // 選択中の枠
    UPROPERTY(meta = (BindWidget))
    UBorder* SelectionBorder;
};
