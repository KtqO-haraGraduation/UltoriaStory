// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemUseConfirmWidget.generated.h"

/**
 * 
 */

class UTextBlock;

UCLASS()
class ULTORIAPROJECT_API UItemUseConfirmWidget : public UUserWidget
{
	GENERATED_BODY()
public:
    // 確認メッセージを設定
    void SetMessage(const FText& Message);
    // アイテム名を設定
    void SetItemName(const FText& ItemName);

protected:
    virtual void NativeConstruct() override;

    // 確認メッセージ
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> MessageText;

    // アイテム名
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemNameText;
};
