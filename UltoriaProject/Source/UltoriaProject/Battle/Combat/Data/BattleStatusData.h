#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BattleParameter.h"
#include "BattleStatusData.generated.h"

// ステータスのタイプ
UENUM(BlueprintType)
enum class StatusType : uint8
{
	GUARD,
	BREAK,
	ULTORIA,
	HEAL,
	POISON,
	BUFF,
	POWER_UP,
	DEFENSE_UP,
};

USTRUCT(BlueprintType)
struct FBattleStatusData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName id;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	StatusType type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int turn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int priority; // 優先度

	bool operator==(const FBattleStatusData& other) const
	{
		return id == other.id;
	}
};
