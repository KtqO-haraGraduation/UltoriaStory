// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "Components/Border.h"
#include "Components/ScrollBox.h"
#include "EquipItemEntry.h"
#include "EquipTab.generated.h"

/**
 * 
 */

UENUM(BluePrintType)
enum class EEquipSlot : uint8
{
	Wepon,
	Head,
	Body,
	Waist,
	Feet
};

UENUM(BluePrintType)
enum class EEquipTabMode : uint8
{
	SlotSelect,
	ItemSelect
};

UCLASS()
class ULTORIAPROJECT_API UEquipTab : public UMenuBase
{
	GENERATED_BODY()
	
public:
	virtual void OnEnter()override;
	virtual void OnExit()override;
	virtual void NavigateUp()override;
	virtual void NavigateDown()override;
	virtual void Confirm()override;
	virtual void Cancel()override;

private:
	void UpdateSlotDisplay();
	void UpdateItemList();

private:
// 装備中の装備名テキスト
	// 剣
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_EquippedNameWeapon;

	// 兜
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_EquippedNameHead;

	// 鎧
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_EquippedNameBody;

	// 腰
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_EquippedNameWaist;

	// 足
	UPROPERTY(meta = (BindWidget))
	UTextBlock* m_EquippedNameFeet;

	// 選択中のハイライト
	UPROPERTY(meta = (BindWidget))
	UBorder* m_FrameEquip;

	UPROPERTY(meta = (BindWidget))
	UBorder* m_FrameEquip1;

	UPROPERTY(meta = (BindWidget))
	UBorder* m_FrameEquip2;

	UPROPERTY(meta = (BindWidget))
	UBorder* m_FrameEquip3;

	UPROPERTY(meta = (BindWidget))
	UBorder* m_FrameEquip4;

	// アイテムリスト小窓
	UPROPERTY(meta = (BindWidget))
	UBorder* m_ItemListPopup;

	// アイテムスクロールボックス
	UPROPERTY(meta = (BindWidget))
	UScrollBox *m_ItemScrollBox;

	// アイテムエントリーWidgetクラス
	UPROPERTY(EditAnywhere, Category = "Equip")
	TSubclassOf<UEquipItemEntry> m_ItemEntryClass;

	// ステータス変動量
	UPROPERTY(meta = (BindWidget))
	class UBorder* m_ChangeStatus;

	// 装備説明
	UPROPERTY(meta = (BindWidget))
	class UBorder* m_EquipItemStatus;

	// 現在の装備スロット
	EEquipSlot m_CurrentSlot;
	// 現在のアイテムの順番
	int32 m_CurrentItemIndex;
	// 現在所持している装備のリスト
	TArray<FName> m_CurrentSlotItems;
	// 現在のタブの状態
	EEquipTabMode m_CurrentMode;
};
