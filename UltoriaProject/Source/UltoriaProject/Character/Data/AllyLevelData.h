#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "AllyLevelData.generated.h"

USTRUCT(BlueprintType)
struct FAllyLevelData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Lv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ATK;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MGC;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int DEF;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SPD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Break;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BreakGauge;
};
