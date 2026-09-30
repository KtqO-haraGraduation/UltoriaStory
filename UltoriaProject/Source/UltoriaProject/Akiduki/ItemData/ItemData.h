#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

// ItemTarget
UENUM(BlueprintType)
enum class EItemType : uint8
{
    Consumable,
    Material,
    Weapon,
    Armor,
    KeyItem
};

// UseTarget
UENUM(BlueprintType)
enum class EItemUseTarget : uint8
{
    None,
    Ally,
    Enemy,
    Self,
    AllAllies
};


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

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EItemUseTarget Target;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUsableInField;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bUsableInBattle;
};