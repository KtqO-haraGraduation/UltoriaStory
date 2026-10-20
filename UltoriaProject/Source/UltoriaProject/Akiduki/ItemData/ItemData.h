#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ItemData.generated.h"

// アイテムの種類
UENUM(BlueprintType)
enum class EItemType : uint8
{
    // 消費アイテム
    Consumable,
    // 素材
    Material,
    // 武器
    Weapon,
    // 防具
    Armor,
    // 重要アイテム
    KeyItem
};

// アイテムを使用する対象
UENUM(BlueprintType)
enum class EItemUseTarget : uint8
{
    // 対象なし
    None,
    // 味方単体
    Ally,
    // 敵単体
    Enemy,
    // 自分自身
    Self,
    // 味方全体
    AllAllies,
    // 敵全体
    AllEnemies
};

// アイテムに設定できる効果の種類
UENUM(BlueprintType)
enum class EItemEffectType : uint8
{
    // HPを回復する
    HealHP,
    // MPを回復する
    HealMP,
    // 毒状態を解除する
    CurePoison
};

// アイテムに設定する個々の効果データ
USTRUCT(BlueprintType)
struct FItemEffectData
{
    GENERATED_BODY()

    // 実行する効果の種類
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effect")
    EItemEffectType EffectType = EItemEffectType::HealHP;

    // 効果の強さ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effect")
    int32 Magnitude = 0;
};


USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_BODY()

public:

    // アイテムのカテゴリ
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Category;

    // アイテムID
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FName ID;

    // 日本語名
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString NameJP;

    // アイテムの説明文
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Description;

    // 既存の効果説明文字列。実際の効果処理にはEffectsを使用する想定
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString Effect;

    // 購入価格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 Price = 0;

    // アイテムアイコン
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TSoftObjectPtr<UTexture2D> Icon;

    // 最大所持数
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxStack = 99;

    // 売却可能か
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCanSell = true;

    // 売却価格
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 SellPrice = 0;

    // アイテムの種類
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemType Type = EItemType::Consumable;

    // 使用対象
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    EItemUseTarget Target = EItemUseTarget::Self;

    // フィールドで使用可能か
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bUsableInField = true;

    // 戦闘中に使用可能か
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bUsableInBattle = true;

    // アイテムが持つ効果
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Effects")
    TArray<FItemEffectData> Effects;
};