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
	
};
