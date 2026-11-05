// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUseConfirmWidget.h"
#include "Components/TextBlock.h"

void UItemUseConfirmWidget::NativeConstruct()
{
    Super::NativeConstruct();
}

void UItemUseConfirmWidget::SetMessage(const FText& Message)
{
    if (IsValid(MessageText))
    {
        MessageText->SetText(Message);
    }
}

void UItemUseConfirmWidget::SetItemName(const FText& ItemName)
{
    if (IsValid(ItemNameText))
    {
        ItemNameText->SetText(ItemName);
    }
}
