#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../Enemy/EnemyBase.h"
#include "../../Stage/StageParameter.h"
#include "Engine/Texture2D.h"
#include "EnemyMasterData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyMasterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStageType StageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EXP;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;
};
