// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MenuClassDataAsset.h"
#include "../../System/Controller/PlayPlayerController.h"
#include "MenuStateManager.generated.h"


class UMenuBase;

UENUM(BlueprintType)
enum class EMenuState : uint8
{
	MENU_MAIN,
	MENU_STATES,
	MENU_ITEMS,
	MENU_QUEST,
	MENU_OPTIONS,
	MENU_SAVE,
	MENU_END,
	MENU_NONE = 255
};

UCLASS()
class ULTORIAPROJECT_API UMenuStateManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	// メニューを開く
	UFUNCTION(BlueprintCallable)
	void OpenMenu(EMenuState State);
	// メニューを閉じる
	UFUNCTION(BlueprintCallable)
	void CloseMenu();
	// ひとつ前のメニューに戻る
	UFUNCTION(BlueprintCallable)
	void BackMenu();

	void ChangeInputMode(PlayInputMode mode);

	// 選択ナビ入力関数
	UFUNCTION(BlueprintCallable)
	void InputNavigateUp();
	UFUNCTION(BlueprintCallable)
	void InputNavigateDown();
	UFUNCTION(BlueprintCallable)
	void InputNavigateLeft();
	UFUNCTION(BlueprintCallable)
	void InputNavigateRight();

	//  LRボタン入力処理
	UFUNCTION(BlueprintCallable)
	void InputLButton();
	UFUNCTION(BlueprintCallable)
	void InputRButton();
	UFUNCTION(BlueprintCallable)
	void InputL2Button();
	UFUNCTION(BlueprintCallable)
	void InputR2Button();

	// 決定入力関数
	UFUNCTION(BlueprintCallable)
	void InputConfirm_Menu();
	// キャンセル関数
	UFUNCTION(BlueprintCallable)
	void InputCancel_Menu();

	// メニュー戻る関数
	void InputBuck();

private:
	// 対応するメニュークラス取得関数
	TSubclassOf<UMenuBase> GetMenuClass(EMenuState State);

	// ボタン入力共通処理関数
	void DispatchMenuInput(void (UMenuBase::* func)());

private:
	// 現在のメニュー状態
	EMenuState m_CurrentMenuState;

	// 各メニューのインスタンス
	UPROPERTY()
	TMap<EMenuState, UMenuBase*> m_MenuMap;

	// 各メニューと対応するクラス
	UPROPERTY()
	TMap<EMenuState, TSubclassOf<UMenuBase>> m_MenuClassMap;

	// メニューの遷移履歴
	TArray<EMenuState> m_MenuHistory;

	// 戻る処理中かどうか
	bool m_IsReturning = false;
};
