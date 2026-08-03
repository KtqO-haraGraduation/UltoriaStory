// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipItemEntry.generated.h"

class UTextBlock;
class UBorder;

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UEquipItemEntry : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetItemName(const FString& Name);

	UFUNCTION(BlueprintCallable)
	void SetSelected(bool isSelected);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UBorder* m_HighlightBorder;
};
