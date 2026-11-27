// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectiveWidget.generated.h"

class UTextBlock;

UCLASS()
class ULTORIAPROJECT_API UObjectiveWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// 目的テキストを設定
	void SetObjective(const FText& text);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_ObjectiveText;
};
