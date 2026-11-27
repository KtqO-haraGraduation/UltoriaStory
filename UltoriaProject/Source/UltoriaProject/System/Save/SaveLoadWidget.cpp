// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveLoadWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "SaveManager.h"
#include "../../UI/Menu/MenuStateManager.h"
#include "GameFramework/PlayerController.h"


void USaveLoadWidget::NativeConstruct()
{
    Super::NativeConstruct();

    // セーブボタンのイベント登録
    if (SaveButton)
    {
        SaveButton->OnClicked.AddDynamic(this,&USaveLoadWidget::OnSaveClicked);
    }
}

// メニュー表示
void USaveLoadWidget::OnEnter()
{
	Super::OnEnter();

    // プレイ時間を更新
    UpdateTimeDisplay();

    // 表示時にフォーカスを設定
    if (APlayerController* PC = GetOwningPlayer())
    {
        PC->SetInputMode(FInputModeGameAndUI());

        PC->SetShowMouseCursor(true);

        if (IsValid(SaveButton))
        {
            SaveButton->SetUserFocus(PC);
        }
    }
}

// セーブボタン
void USaveLoadWidget::OnSaveClicked()
{
    USaveManager* SaveManager = GetGameInstance()->GetSubsystem<USaveManager>();

    if (!SaveManager)
    {
        UE_LOG(LogTemp, Error, TEXT("SaveManager not found"));
        return;
    }

    // セーブ実行
    const bool bResult = SaveManager->SaveGameData();

    UE_LOG(
        LogTemp,
        Warning,
        TEXT("Save Result : %s"),
        bResult ? TEXT("Success") : TEXT("Failed")
    );

    // 保存後に画面を閉じる
    if (bResult)
    {
        Cancel();
    }
}

// 決定
void USaveLoadWidget::Confirm()
{
    // セーブする
    OnSaveClicked();
}

// キャンセル
void USaveLoadWidget::Cancel()
{
    // メニュー管理クラスを取得
    UMenuStateManager* MenuManager =
        GetGameInstance()->GetSubsystem<UMenuStateManager>();

    // 前のメニューへ戻る
    if (IsValid(MenuManager))
    {
        MenuManager->BackMenu();
    }
}

// プレイ時間表示を更新
void USaveLoadWidget::UpdateTimeDisplay()
{
    USaveManager* SaveManager =
        GetGameInstance()->GetSubsystem<USaveManager>();

    if (!SaveManager || !PlayTimeText)
    {
        return;
    }

    // 累計プレイ時間を秒で取得
    int64 TotalSeconds =
        SaveManager->GetTotalPlayTimeSeconds();

    // 時間に変換
    int64 Hours = TotalSeconds / 3600;

    // 分に変換
    int64 Minutes = (TotalSeconds % 3600) / 60;

    // 表示文字列を作成（時間：分）
    FString PlayTimeString = FString::Printf(
        TEXT("%02lld:%02lld"),
        static_cast<long long>(Hours),
        static_cast<long long>(Minutes)
    );

    // 画面に反映
    PlayTimeText->SetText(
        FText::FromString(PlayTimeString)
    );
}