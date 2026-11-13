#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "BattleParameter.h"
#include "BattleCommandData.generated.h"

// コマンドタイプ
UENUM(BlueprintType)
enum class ECommandType : uint8
{
	ATTACK,
	GUARD,
	ARCANA,
	SKILL,
	ITEM,
	ESCAPE,
};

USTRUCT(BlueprintType)
struct FBattleCommandData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECommandType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Desc;

	bool operator==(const FBattleCommandData& other) const
	{
		return Type == other.Type;
	}
};
