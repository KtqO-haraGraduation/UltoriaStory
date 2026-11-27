
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveManager.generated.h"

class UGameSaveData;
class APlayerCharacterBase;

UCLASS()
class ULTORIAPROJECT_API USaveManager : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:

    // セーブデータを保存する
    UFUNCTION(BlueprintCallable)
    bool SaveGameData();

    // セーブデータを読み込む
    UFUNCTION(BlueprintCallable)
    bool LoadGameData();

    // セーブデータが存在するか確認
    UFUNCTION(BlueprintCallable)
    bool DoesSaveExist() const;

    // 現在読み込んでいるセーブデータを取得
    UGameSaveData* GetCurrentSaveData() const { return m_CurrentSaveData; }

    // 現在のゲーム状態をセーブデータに格納する
    void CollectGameData();

    // セーブデータをゲーム内に反映
    void ApplyGameData(APlayerCharacterBase* Player);

    // プレイ時間の計測を開始
    void StartPlayTimeTracking();

    // 現在の累計プレイ時間を秒で取得
    int64 GetTotalPlayTimeSeconds() const;

private:

    // 保存先のスロット名
    static const FString SAVE_SLOT_NAME;

    UPROPERTY()
    TObjectPtr<UGameSaveData> m_CurrentSaveData;

    // プレイ時間の計測開始時刻
    double m_PlayTimeStartSeconds = 0.0;

    // プレイ時間を計測中か
    bool m_bPlayTimeTracking = false;
};
