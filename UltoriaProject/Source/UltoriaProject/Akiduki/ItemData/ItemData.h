#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Category;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString NameJP;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Effect;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Price;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSoftObjectPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxStack = 99;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bCanSell = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 SellPrice = 0;
};