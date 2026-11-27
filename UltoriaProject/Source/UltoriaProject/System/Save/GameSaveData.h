// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSaveData.generated.h"

// キャラクター1人分のセーブデータ
USTRUCT(BlueprintType)
struct FSaveCharacterData
{
    GENERATED_BODY()

    // キャラクターID
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FName CharacterID;

    // レベル
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 Level = 1;

    // 経験値
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 EXP = 0;

    // 現在HP
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 CurrentHP = 0;

    // 現在MP
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 CurrentMP = 0;
};

// アイテム1種類分のセーブデータ
USTRUCT(BlueprintType)
struct FSaveItemData
{
    GENERATED_BODY()

    // アイテムID
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FName ItemID;

    // 所持数
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 Count = 0;
};

// セーブデータ本体
UCLASS()
class ULTORIAPROJECT_API UGameSaveData : public USaveGame
{
    GENERATED_BODY()

public:

    // ストーリー進行度
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 StoryProgress = 0;

    // 現在のステージID
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FName CurrentStageID;

    // 保存日時（今回のセーブ日時）
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FDateTime SaveDateTime;

    // 前回の保存日時
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FDateTime PreviousSaveDateTime;

    // 累計プレイ時間（秒）
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int64 TotalPlayTimeSeconds = 0;

    // 所持金
    UPROPERTY(SaveGame, BlueprintReadWrite)
    int32 Money = 0;

    // メインプレイヤーの情報
    UPROPERTY(SaveGame, BlueprintReadWrite)
    FSaveCharacterData PlayerData;

    // 仲間の情報
    UPROPERTY(SaveGame, BlueprintReadWrite)
    TArray<FSaveCharacterData> PartyCharacters;

    // 所持アイテム
    UPROPERTY(SaveGame, BlueprintReadWrite)
    TArray<FSaveItemData> InventoryItems;
};