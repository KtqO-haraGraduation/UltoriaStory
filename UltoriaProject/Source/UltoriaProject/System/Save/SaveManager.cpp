
#include "SaveManager.h"
#include "GameSaveData.h"
#include "HAL/PlatformTime.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Character/CharacterBase.h"
#include "../../Battle/PlayerPartyManager.h"
#include "Kismet/GameplayStatics.h"
#include <UltoriaProject/Character/Ally/PlayerCharacterBase.h>

// セーブスロットの名前
const FString USaveManager::SAVE_SLOT_NAME = TEXT("SaveSlot_1");

// セーブデータを保存
bool USaveManager::SaveGameData()
{
    if (!m_CurrentSaveData)
    {
        m_CurrentSaveData = NewObject<UGameSaveData>(this);
    }

    // 保存前のプレイ時間を退避
    int64 PreviousPlayTime =
        m_CurrentSaveData->TotalPlayTimeSeconds;

    // 現在のゲーム状態をセーブデータに格納
    CollectGameData();

    // セーブ実行
    bool bResult = UGameplayStatics::SaveGameToSlot(m_CurrentSaveData,SAVE_SLOT_NAME,0);

    if (bResult)
    {
        // 保存成功後、計測開始時刻を更新
        if (m_bPlayTimeTracking)
        {
            m_PlayTimeStartSeconds =
                FPlatformTime::Seconds();
        }
    }
    else
    {
        // 保存失敗時は以前の累計時間に戻す
        m_CurrentSaveData->TotalPlayTimeSeconds =
            PreviousPlayTime;
    }


    // 常に同じスロットへ保存
    // return UGameplayStatics::SaveGameToSlot(m_CurrentSaveData, SAVE_SLOT_NAME, 0);
  
    return bResult;
}

// セーブデータを読み込む
bool USaveManager::LoadGameData()
{
    
    USaveGame* loadedData = UGameplayStatics::LoadGameFromSlot(SAVE_SLOT_NAME,0);

    m_CurrentSaveData = Cast<UGameSaveData>(loadedData);

    if (!m_CurrentSaveData)
    {
        return false;
    }

    // ゲームへの反映はプレイヤー初期化後に行う
    return true;
}

// セーブデータが存在するか確認
bool USaveManager::DoesSaveExist() const
{
    return UGameplayStatics::DoesSaveGameExist(SAVE_SLOT_NAME,0);
}

void USaveManager::CollectGameData()
{
    // セーブデータがなければ新しく作成
    if (!m_CurrentSaveData)
    {
        m_CurrentSaveData = NewObject<UGameSaveData>(this);
    }

    // プレイヤーを取得
    APlayerController* PlayerController =
        UGameplayStatics::GetPlayerController(this, 0);

    if (!PlayerController)
    {
        return;
    }

    ACharacterBase* Player =
        Cast<ACharacterBase>(PlayerController->GetPawn());

    if (!Player)
    {
        return;
    }

    // プレイヤーの情報を保存
    m_CurrentSaveData->PlayerData.CharacterID = Player->GetID();
    m_CurrentSaveData->PlayerData.Level = Player->GetLevel();
    m_CurrentSaveData->PlayerData.EXP = Player->GetExp();
    m_CurrentSaveData->PlayerData.CurrentHP = Player->GetCurrentHP();
    m_CurrentSaveData->PlayerData.CurrentMP = Player->GetCurrentMP();

    // 前回の保存日時を記録
    m_CurrentSaveData->PreviousSaveDateTime =
        m_CurrentSaveData->SaveDateTime;

    // 今回の保存日時を記録
    m_CurrentSaveData->SaveDateTime = FDateTime::Now();

    // 累計プレイ時間を保存
    m_CurrentSaveData->TotalPlayTimeSeconds =
        GetTotalPlayTimeSeconds();

    // 所持金を保存
    UInventorySubsystem* Inventory =
        GetGameInstance()->GetSubsystem<UInventorySubsystem>();

    if (Inventory)
    {
        m_CurrentSaveData->Money = Inventory->GetMoney();

        // 前回のデータをクリア
        m_CurrentSaveData->InventoryItems.Empty();

        // インベントリ内のアイテムを取得
        const TArray<FInventoryItem>& Items = Inventory->GetInventory();

        for (const FInventoryItem& Item : Items)
        {
            FSaveItemData SaveItem;

            SaveItem.ItemID = Item.ItemID;
            SaveItem.Count = Item.Count;

            m_CurrentSaveData->InventoryItems.Add(SaveItem);
        }
    }

    // パーティメンバーを保存
    UPlayerPartyManager* PartyManager =
        GetWorld()->GetSubsystem<UPlayerPartyManager>();

    if (PartyManager)
    {
        // 前回のパーティデータをクリア
        m_CurrentSaveData->PartyCharacters.Empty();

        // パーティに登録されているキャラクターIDを取得
        const TArray<FPartyData>& PartyData = PartyManager->GetPartyData();

        for (const FPartyData& Party : PartyData)
        {
            FSaveCharacterData SaveCharacter;

            // キャラクターIDを保存
            SaveCharacter.CharacterID = Party.characterID;

            m_CurrentSaveData->PartyCharacters.Add(SaveCharacter);
        }
    }
}

void USaveManager::ApplyGameData(APlayerCharacterBase* Player)
{
    
    // nullチェック
    if (!m_CurrentSaveData || !Player)
    {
        return;
    }

    // プレイヤーのステータスを復元
    const FSaveCharacterData& Data = m_CurrentSaveData->PlayerData;

    Player->LoadSaveStatus(Data.Level,Data.EXP,Data.CurrentHP,Data.CurrentMP);

    // 所持金を復元
    UInventorySubsystem* Inventory =
        GetGameInstance()->GetSubsystem<UInventorySubsystem>();

    if (Inventory)
    {
        // 所持金
        Inventory->SetMoney(m_CurrentSaveData->Money);

        TArray<FInventoryItem> Items;

        for (const FSaveItemData& SaveItem :
            m_CurrentSaveData->InventoryItems)
        {
            FInventoryItem Item;

            Item.ItemID = SaveItem.ItemID;
            Item.Count = SaveItem.Count;

            Items.Add(Item);
        }

        // インベントリ
        Inventory->SetInventory(Items);
    }

    // パーティ情報を復元
    UPlayerPartyManager* PartyManager =
        GetWorld()->GetSubsystem<UPlayerPartyManager>();

    if (PartyManager)
    {
        // セーブデータに登録されているキャラクターを追加
        for (const FSaveCharacterData& SaveCharacter :
            m_CurrentSaveData->PartyCharacters)
        {
            // IDが空の場合は無視
            if (SaveCharacter.CharacterID.IsNone())
            {
                continue;
            }

            // 主人公は追加しない
            // ※現在のパーティ登録仕様に合わせる
            //if (SaveCharacter.CharacterID ==
            //    m_CurrentSaveData->PlayerData.CharacterID)
            //{
            //    continue;
            //}

            FPartyData Party;
            Party.characterID = SaveCharacter.CharacterID;

            // 既存の登録関数を使用
            PartyManager->AddPlayerParty(Party);
        }
    }
}

// プレイ時間の計測を開始
void USaveManager::StartPlayTimeTracking()
{
    // すでに計測中なら二重に開始しない
    if (m_bPlayTimeTracking)
    {
        return;
    }

    m_PlayTimeStartSeconds = FPlatformTime::Seconds();
    m_bPlayTimeTracking = true;
}


// 現在の累計プレイ時間を秒で取得
int64 USaveManager::GetTotalPlayTimeSeconds() const
{
    // セーブデータがない場合は0秒から開始
    int64 TotalSeconds = 0;

    if (m_CurrentSaveData)
    {
        TotalSeconds =
            m_CurrentSaveData->TotalPlayTimeSeconds;
    }

    // 現在のプレイ時間を加算
    if (m_bPlayTimeTracking)
    {
        double ElapsedSeconds =
            FPlatformTime::Seconds() - m_PlayTimeStartSeconds;

        TotalSeconds += static_cast<int64>(ElapsedSeconds);
    }

    return TotalSeconds;
}