#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "../ItemData/ItemData.h"
#include "../Shop/ShopManager.h"
#include "ShopMenuWidget.generated.h"

class UTextBlock;
class UScrollBox;
class UDataTable;
class UShopItemEntryWidget;
class UBuyConfirmWidget;

UCLASS()
class ULTORIAPROJECT_API UShopMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

public:
	// 選択移動
	void MoveSelection(int32 Direction);

	// 選択更新
	void UpdateSelection();

	// 決定
	bool ConfirmSelection();

	// 購入
	void BuyCurrentItem();

	// 所持金更新
	void UpdateMoney();

	// アイテムセットアップ
	void SetupItems(const TArray<FItemData>& Items);

public:

	// ScrollBox
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemList;

	// アイテム名
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	// アイテム説明文
	UPROPERTY(meta = (BindWidget))
	UTextBlock* DescriptionText;

	// アイテム行Widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UUserWidget> ShopItemEntryClass;

	// アイテム購入確認
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shop")
	TSubclassOf<UBuyConfirmWidget> BuyConfirmClass;

	// プレイヤーの所持金
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category = "Shop")
	UTextBlock* MoneyText;

	UPROPERTY()
	TArray<UShopItemEntryWidget*> ItemEntries;

	UPROPERTY()
	UBuyConfirmWidget* BuyConfirmWidget;

	// 現在選択
	int32 CurrentIndex = 0;
};