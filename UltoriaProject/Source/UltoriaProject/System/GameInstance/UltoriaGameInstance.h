// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "UltoriaGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API UUltoriaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

    // アイテムデータ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* ItemDataTable;

    // ショップデータ
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Data")
    UDataTable* ShopDataTable;

    // ストーリー進行度を取得
    int32 GetStoryProgress() const
    {
        return m_StoryProgress;
    }

    // ストーリー進行度を設定
    void SetStoryProgress(int32 Progress)
    {
        m_StoryProgress = FMath::Max(1, Progress);
    }

    
private:
    // ストーリー進行度
    int32 m_StoryProgress = 2;
	
};
