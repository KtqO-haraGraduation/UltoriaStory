#pragma once

#include "CoreMinimal.h"
#include "../../UI/Menu/MenuBase.h"
#include "SaveLoadWidget.generated.h"

class UButton;
class UButton;
class UTextBlock;

UCLASS()
class ULTORIAPROJECT_API USaveLoadWidget : public UMenuBase
{
    GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;

    // レポートに記録するボタン
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UButton> SaveButton;

    // セーブボタンのクリック処理
    UFUNCTION()
    void OnSaveClicked();

    // メニュー表示時
    virtual void OnEnter() override;

    // 決定
    virtual void Confirm() override;

    // キャンセル
    virtual void Cancel() override;

    // プレイ時間表示を更新
    void UpdateTimeDisplay();

    // 現在の時間表示用テキスト
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> NowTimeText;

    // プレイ時間表示用テキスト
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> PlayTimeText;

    // 前回のセーブ時間表示用テキスト
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> OldSaveTimeText;


};