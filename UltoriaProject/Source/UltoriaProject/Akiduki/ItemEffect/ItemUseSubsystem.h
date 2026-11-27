// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemUseSubsystem.generated.h"

// アイテムデータの前方宣言
struct FItemData;
class ACharacterBase;

UCLASS()
class ULTORIAPROJECT_API UItemUseSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    // アイテムを使用する
    bool UseItem(const FItemData& ItemData);

    // バトル用のアイテム使用
    void UseItemBattle(const FItemData& ItemData, const TArray<ACharacterBase*>& targets);
};
