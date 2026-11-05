#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "../CharacterBase.h"
#include "Engine/Texture2D.h"
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
	FName JobType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ACharacterBase> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Image;
};
