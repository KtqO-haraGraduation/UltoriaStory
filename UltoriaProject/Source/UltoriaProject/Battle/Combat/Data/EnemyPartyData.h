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
	EStageType StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enemy1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enemy2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enemy3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Enemy4;
};
