
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManager.generated.h"

class UGameSaveData;

UCLASS()
class ULTORIAPROJECT_API USaveManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    // セーブデータを保存する
    UFUNCTION(BlueprintCallable)
    bool SaveGameData(int32 SlotIndex);

    // セーブデータを読み込む
    UFUNCTION(BlueprintCallable)
    bool LoadGameData(int32 SlotIndex);

    // 指定スロットにセーブデータがあるか確認する
    UFUNCTION(BlueprintCallable)
    bool DoesSaveExist(int32 SlotIndex) const;

    // 現在読み込んでいるセーブデータを取得
    UGameSaveData* GetCurrentSaveData() const { return m_CurrentSaveData; }

    // 現在のゲーム状態をセーブデータに格納する
    void CollectGameData();

private:

    // 現在使用しているセーブデータ
    UPROPERTY()
    TObjectPtr<UGameSaveData> m_CurrentSaveData;

    // セーブスロットの名前を取得
    FString GetSlotName(int32 SlotIndex) const;

    // セーブスロットの最大数
    static constexpr int32 MAX_SAVE_SLOT = 3;
};
