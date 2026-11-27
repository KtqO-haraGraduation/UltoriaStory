// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemUseSubsystem.h"
#include "../../Akiduki/ItemData/ItemData.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Character/Ally/MainPlayer.h"
#include "../../Character/CharacterBase.h"
#include "../../Battle/BattleManager.h"
#include "../../UI/Battle/BattleUIManager.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

bool UItemUseSubsystem::UseItem(const FItemData& ItemData)
{
    // 現段階ではSelfのみ対応
    if (ItemData.Target != EItemUseTarget::Self)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("This item target is not implemented yet"));

        return false;
    }

    // インベントリを取得
    UInventorySubsystem* Inventory =
        GetGameInstance()->GetSubsystem<UInventorySubsystem>();

    // アイテムを所持しているか確認
    if (!Inventory->HasItem(ItemData.ID, 1))
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Item not found in inventory"));

        return false;
    }

    // プレイヤーコントローラーを取得
    UWorld* World = GetWorld();
    APlayerController* PC = World->GetFirstPlayerController();

    // 主人公を取得
    AMainPlayer* Player = Cast<AMainPlayer>(PC->GetPawn());

    // アイテムの効果を適用
    for (const FItemEffectData& Effect : ItemData.Effects)
    {
        switch (Effect.EffectType)
        {
        case EItemEffectType::HealHP:
        {
            const int32 OldHP = Player->GetCurrentHP();

            Player->SetHP(OldHP + Effect.Magnitude);

            UE_LOG(LogTemp, Log,
                TEXT("HP: %d -> %d"),
                OldHP,
                Player->GetCurrentHP());

            break;
        }
        case EItemEffectType::HealMP:
        {
            const int32 OldMP = Player->GetCurrentMP();

            Player->SetMP(OldMP + Effect.Magnitude);

            UE_LOG(LogTemp, Log,
                TEXT("MP: %d -> %d"),
                OldMP,
                Player->GetCurrentMP());

            break;
        }
        case EItemEffectType::HealHPPercent:
        {
            const int32 OldHP = Player->GetCurrentHP();

            // 最大HPの指定割合を回復
            const int32 HealAmount =
                FMath::RoundToInt(Player->GetMaxHP() * Effect.Magnitude / 100.0f);

            Player->SetHP(OldHP + HealAmount);

            UE_LOG(LogTemp, Log,
                TEXT("HP Percent Heal: %d -> %d"),
                OldHP,
                Player->GetCurrentHP());

            break;
        }

        case EItemEffectType::HealMPPercent:
        {
            const int32 OldMP = Player->GetCurrentMP();

            // 最大MPの指定割合を回復
            const int32 HealAmount =
                FMath::RoundToInt(Player->GetMaxMP() * Effect.Magnitude / 100.0f);

            Player->SetMP(OldMP + HealAmount);

            UE_LOG(LogTemp, Log,
                TEXT("MP Percent Heal: %d -> %d"),
                OldMP,
                Player->GetCurrentMP());

            break;
        }
        case EItemEffectType::CurePoison:
        {
            // 毒解除処理は後で実装
            break;
        }

        default:
            break;
        }
    }

    // 効果適用後にアイテムを1個消費
    if (!Inventory->RemoveItem(ItemData.ID, 1))
    {
        return false;
    }

    UE_LOG(LogTemp, Log,
        TEXT("Item consumed: %s"),
        *ItemData.ID.ToString());

    return true;
}

void UItemUseSubsystem::UseItemBattle(const FItemData& ItemData, const TArray<ACharacterBase*>& targets)
{
    // 現段階ではSelfのみ対応
    if (ItemData.Target != EItemUseTarget::Self)
    {
        return;
    }

    // インベントリを取得
    UInventorySubsystem* Inventory =
        GetGameInstance()->GetSubsystem<UInventorySubsystem>();

    // BattleManager
    UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
    if (!battleMg) return;

    // BattleUIManager
    UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
    if (!uiMg) return;

    // アイテムを所持しているか確認
    if (!Inventory->HasItem(ItemData.ID, 1))
    {
        return;
    }

    for (ACharacterBase* target : targets)
    {
        if (!target) continue;
        // アイテムの効果を適用
        for (const FItemEffectData& Effect : ItemData.Effects)
        {
            switch (Effect.EffectType)
            {
            case EItemEffectType::HealHP:
            {
                const int32 OldHP = target->GetCurrentHP();

                target->SetHP(OldHP + Effect.Magnitude);

                if (battleMg->IsBattle())
                {
                    uiMg->CreateDamageWidget(target, Effect.Magnitude);
                }
                break;
            }
            case EItemEffectType::HealMP:
            {
                const int32 OldMP = target->GetCurrentMP();

                target->SetMP(OldMP + Effect.Magnitude);
                if (battleMg->IsBattle())
                {
                    uiMg->CreateDamageWidget(target, Effect.Magnitude);
                }
                break;
            }
            case EItemEffectType::HealHPPercent:
            {
                const int32 OldHP = target->GetCurrentHP();

                // 最大HPの指定割合を回復
                const int32 HealAmount =
                    FMath::RoundToInt(target->GetMaxHP() * Effect.Magnitude / 100.0f);

                target->SetHP(OldHP + HealAmount);
                if (battleMg->IsBattle())
                {
                    uiMg->CreateDamageWidget(target, HealAmount);
                }
                break;
            }

            case EItemEffectType::HealMPPercent:
            {
                const int32 OldMP = target->GetCurrentMP();

                // 最大MPの指定割合を回復
                const int32 HealAmount =
                    FMath::RoundToInt(target->GetMaxMP() * Effect.Magnitude / 100.0f);

                target->SetMP(OldMP + HealAmount);
                if (battleMg->IsBattle())
                {
                    uiMg->CreateDamageWidget(target, HealAmount);
                }
                break;
            }
            case EItemEffectType::CurePoison:
            {
                // 毒解除処理は後で実装
                break;
            }
            default:
                break;
            }
        }
    }

    // 効果適用後にアイテムを1個消費
    if (!Inventory->RemoveItem(ItemData.ID, 1))
    {
        return;
    }
}
