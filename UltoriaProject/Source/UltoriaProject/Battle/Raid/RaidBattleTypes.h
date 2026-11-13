#pragma once

#include "CoreMinimal.h"
#include "RaidBattleTypes.generated.h"

UENUM(BlueprintType)
enum class ERaidCommandType : uint8
{
    ATTACK,
    GUARD,
    SKILL,
    ITEM,
    END_TURN
};

UENUM(BlueprintType)
enum class ERaidBattleResult : uint8
{
    NONE,
    VICTORY,
    DEFEAT
};

USTRUCT(BlueprintType)
struct FRaidCharacterState
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    TObjectPtr<class ACharacterBase> Character = nullptr;

    UPROPERTY(BlueprintReadOnly)
    FString CharacterName;

    UPROPERTY(BlueprintReadOnly)
    int32 CurrentHP = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 MaxHP = 0;

    UPROPERTY(BlueprintReadOnly)
    int32 Speed = 0;

    UPROPERTY(BlueprintReadOnly)
    bool bIsBoss = false;
};
