// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Base/BattleSelectionUIBase.h"
#include "../../../Battle/Raid/RaidBattleTypes.h"
#include "RaidCommandUIWidget.generated.h"

class URaidCommandButtonWidget;

USTRUCT(BlueprintType)
struct FRaidCommandEntry
{
	GENERATED_BODY()

	// 送るコマンド
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid")
	ERaidCommandType Command = ERaidCommandType::ATTACK;

	// ボタンに表示する名前（空ならコマンド名がそのまま出る）
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Raid")
	FText Label;
};

UCLASS()
class ULTORIAPROJECT_API URaidCommandUIWidget : public UBattleSelectionUIBase
{
	GENERATED_BODY()

public:
	URaidCommandUIWidget(const FObjectInitializer& objectInitializer);

	virtual void CreateButton()override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	TSubclassOf<URaidCommandButtonWidget> m_ButtonClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Raid")
	TArray<FRaidCommandEntry> m_CommandEntries;
};
