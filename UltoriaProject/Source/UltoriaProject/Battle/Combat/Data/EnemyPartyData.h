#pragma once

#include "CoreMinimal.h"
#include "../../../Stage/StageManager.h"
#include "Engine/DataTable.h"
#include "EnemyPartyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyPartyData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Enemies;
};
