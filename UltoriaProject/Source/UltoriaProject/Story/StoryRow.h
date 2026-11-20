// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "StoryRow.generated.h"

USTRUCT(BlueprintType)
struct FStoryRow : public FTableRowBase
{
	GENERATED_BODY()

	// このイベントを発動するトリガーのID
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName TriggerID;

	// 流すイベント台本
	UPROPERTY(EditAnyWhere,BluePrintReadOnly)
	TSoftObjectPtr<UDataTable>Script;

	// 画面に出す目的テキスト
	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText ObjectiveText;
};

