#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ItemData/ItemData.h"
#include "BuyConfirmWidget.generated.h"

class UTextBlock;

UCLASS()
class ULTORIAPROJECT_API UBuyConfirmWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetupItem(const FItemData& InItem, int32 PlayerMoney);

	void ChangeCount(int32 Delta);

	int32 GetBuyCount() const
	{
		return BuyCount;
	}

	FItemData GetItemData() const
	{
		return ItemData;
	}

protected:

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* CountText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TotalPriceText;

	FItemData ItemData;

	int32 BuyCount = 1;
	int32 MaxBuyCount = 1;

	void RefreshUI();
};