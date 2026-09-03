// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayPlayerController.generated.h"

// プレイシーン時の、入力の種類
enum class PlayInputMode
{
	FIELD,
	BATTLE,
	MENU,
	SHOP,
};

enum class EUIInputType
{
	NONE = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	L1,
	R1,
	L2,
	R2,
	INPUTTYPE_MAX
};

/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API APlayPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// 入力のモードの切り替え
	void ChangeInputMode(PlayInputMode mode);
	// マウスカーソルの表示切り替え
	void ChangeCursorMode(bool isShow);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	virtual bool InputKey(const FInputKeyEventArgs& Params) override;

	void UpdateCursorByInput(bool bGamepad);

	bool IsCurrentInputMode(PlayInputMode mode) { return mode == m_CurrentInputMode; }

// 入力処理関数群
	// 十字キー(矢印/WASD)入力関数
	void InputDirectional(const FInputActionValue& Value);
	// マウス移動入力関数
	void InputMouseMove(const FInputActionValue& Value);
	// 決定関数
	void InputConfirm();
	// インタラクト入力関数
	void InputInteract();
	// キャンセル関数
	void InputCancel();

// フィールド用入力関数
    // 移動関数
	// 移動入力関数
	void InputMove(const FInputActionValue& Value);
	// 攻撃関数
	void InputAttack(const FInputActionValue& Value);
	// インタラクト関数
	void InputInteract_Field();

// バトル用入力関数
    // 決定入力関数
	void InputConfirm_Battle();
	// キャンセル関数
	void InputCancel_Battle();
	// 十字キー(矢印/WASD)入力関数
	void InputDirectional_Battle(EUIInputType type);


// メニュー用入力関数
    // メニューオープン関数
	void InputOpen_Menu();
    // 決定入力関数
	void InputConfirm_Menu();
    // キャンセル関数
	void InputCancel_Menu();
	// 十字キー(矢印/WASD)入力関数
	void InputDirectional_Menu(EUIInputType type);
	// L1入力関数
	void InputL1_Menu();
	// L2入力関数
	void InputL2_Menu();
	// R1入力関数
	void InputR1_Menu();
	// R2入力関数
	void InputR2_Menu();


// ショップ用入力関数
	// 決定入力関数
	void InputConfirm_Shop();
	// キャンセル入力関数
	void InputCancel_Shop();
	// インタラクト入力関数
	void InputInteract_Shop();
	//　// 十字キー(矢印/WASD)入力関数
	void InputDirectional_Shop(EUIInputType type);

private:
// 現在の入力モード
	PlayInputMode m_CurrentInputMode;

// フィールド入力関連
	// フィールド用IMC
	UPROPERTY(EditAnywhere)
	UInputMappingContext* m_InputField;
	// 移動用IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputMove;
	// フィールド攻撃IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputAttack;
	// インタラクトIA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputInteract;

// バトル入力関連
	// バトル用IMC
	UPROPERTY(EditAnywhere)
	UInputMappingContext* m_InputBattle;

// メニュー入力関連
	// メニュー入力用IMC
	UPROPERTY(EditAnywhere)
	UInputMappingContext* m_InputMenu;
	// メニューオープン入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputMenuOpen;

// ショップ入力関連
	// ショップ入力用IMC
	UInputMappingContext* m_InputShop;

// 汎用入力関連
	// 十字キー(矢印/WASD)入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_DirectionalInput;
	// L1入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputL1;
	// L2入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputL2;
	// R1入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputR1;
	// R2入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputR2;
	// マウス移動入力IA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputMouseMove;
	// 決定ボタンIA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputConfirn;
	// キャンセルボタンIA
	UPROPERTY(EditAnywhere)
	UInputAction* m_InputCancel;
};
