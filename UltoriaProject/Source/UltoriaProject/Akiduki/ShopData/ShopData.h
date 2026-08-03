#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ShopData.generated.h"

USTRUCT(BlueprintType)
struct FShopData : public FTableRowBase
{
	GENERATED_BODY()

public:

	// ショップカテゴリー
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ShopCategory;

	// 売るアイテムID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	// 店で売る順番
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SortOrder = 0;

	// ストーリー進行度
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ShopLevel = 0;
};
