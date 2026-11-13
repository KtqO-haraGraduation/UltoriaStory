
#include "SaveManager.h"
#include "GameSaveData.h"
#include "../../Inventory/InventorySubsystem.h"
#include "../../Character/CharacterBase.h"
#include "../../Battle/PlayerPartyManager.h"
#include "Kismet/GameplayStatics.h"

// スロット番号から保存名を作成
FString USaveManager::GetSlotName(int32 SlotIndex) const
{
    return FString::Printf(TEXT("SaveSlot_%d"), SlotIndex);
}

// セーブデータを保存
bool USaveManager::SaveGameData(int32 SlotIndex)
{
    if (SlotIndex < 1 || SlotIndex > MAX_SAVE_SLOT)
    {
        return false;
    }

    if (!m_CurrentSaveData)
    {
        m_CurrentSaveData = NewObject<UGameSaveData>(this);
    }

    // 現在のゲーム状態をセーブデータに格納
    CollectGameData();

    return UGameplayStatics::SaveGameToSlot(m_CurrentSaveData,GetSlotName(SlotIndex),0);

}

// セーブデータを読み込む
bool USaveManager::LoadGameData(int32 SlotIndex)
{
    if (SlotIndex < 1 || SlotIndex > MAX_SAVE_SLOT)
    {
        return false;
    }

    if (!DoesSaveExist(SlotIndex))
    {
        return false;
    }

    USaveGame* loadedData = UGameplayStatics::LoadGameFromSlot(GetSlotName(SlotIndex),0);

    m_CurrentSaveData = Cast<UGameSaveData>(loadedData);

    return m_CurrentSaveData != nullptr;
}

// セーブデータが存在するか確認
bool USaveManager::DoesSaveExist(int32 SlotIndex) const
{
    if (SlotIndex < 1 || SlotIndex > MAX_SAVE_SLOT)
    {
        return false;
    }

    return UGameplayStatics::DoesSaveGameExist(GetSlotName(SlotIndex),0);
 
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
