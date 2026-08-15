#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../CharacterBase.h"
#include "AllyMasterData.generated.h"

USTRUCT(BlueprintType)
struct FAllyMasterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int InitialLv;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacterBase> Class;
};
