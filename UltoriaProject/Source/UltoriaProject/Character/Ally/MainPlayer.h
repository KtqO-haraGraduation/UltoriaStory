// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CharacterBase.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "MainPlayer.generated.h"

struct FJobData;
struct FJobBase;
/**
 * 
 */
UCLASS()
class ULTORIAPROJECT_API AMainPlayer : public ACharacterBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

public:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	// 当たり判定
	UFUNCTION(BlueprintCallable)
	virtual void BeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp);

public:
	// Called every frame
	void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// ----- フィールド上の操作 ----- //
	// スティック移動
	void InputStickMove(const FInputActionValue& value);
	// 十字キー・キーボード移動
	void InputUpMove(const FInputActionValue& value);		// 上
	void InputDownMove(const FInputActionValue& value);		// 下
	void InputLeftMove(const FInputActionValue& value);		// 左
	void InputRightMove(const FInputActionValue& value);	// 右
	// ジャンプ
	void InputJump(const FInputActionValue& value);
	// 先制攻撃
	void InputPreemptiveAttack(const FInputActionValue& value);

	
	// ------------------------------ //

public: // getter/setter



private: 
	void JobChange();	// ジョブの切り替え
	void StartBattle();	// バトル開始
	void EndBattle();	// バトル終了
	bool IsControll();	// 操作可能な状態であるか
	FStatus TotalStatus();	// 総合ステータス

public:	// デバッグ用の関数
	void InputCheckStatus(const FInputActionValue& value);	// ステータス確認
	void InputJobChange(const FInputActionValue& value);	// ジョブの切り替え
	FStatus SetStatusByDebugLevel();		// レベルに合わせたステータスにする


private: // デバッグ用の変数
	UPROPERTY(EditAnywhere)
	UInputAction* m_CheakStatus;
	UPROPERTY(EditAnywhere)
	UInputAction* m_JobChange;

	UPROPERTY(EditAnywhere)
	bool m_DebugMord;		// trueであればデバッグ用の処理を行う

	UPROPERTY(EditAnywhere)
	int m_DebugLevel;		// 初期レベル設定

	UPROPERTY(EditAnywhere)
	int m_ChengeJobId;		// ジョブIDの切り替え先

private:
	UPROPERTY(EditAnywhere)
	UInputMappingContext* m_InputMap;

	UPROPERTY(EditAnywhere)
	UInputAction* m_StickMoveAction;	// スティックでキャラクターを動かす

	UPROPERTY(EditAnywhere)
	UInputAction* m_JumpAction;

	// --- 十字キー・キーボードでキャラクターを動かす --- //
	UPROPERTY(EditAnywhere)
	UInputAction* m_UpMoveAction;		//上
	UPROPERTY(EditAnywhere)
	UInputAction* m_DownMoveAction;		//下
	UPROPERTY(EditAnywhere)
	UInputAction* m_LeftMoveAction;		//左
	UPROPERTY(EditAnywhere)
	UInputAction* m_RightMoveAction;	//右
	// ---------------------------------------- //]
	UPROPERTY(EditAnywhere)
	EPlayerType m_PlayerType;

	UPROPERTY(EditAnywhere)
	float m_MoveSpeed;
	
	UPROPERTY(EditAnywhere)
	int m_JobId;

	FStatus m_Status[LEVEL_MAX];// プレイヤーのメインステータス

	// ジョブの情報
	FJobBase* m_JobBase;	// ベース
	FJobData* m_JobData;	// データ

	FVector2D m_StickInputVec;	// スティックの入力ベクトル
	FVector m_LaunchVec;
	FVector m_RespawnPos;

	bool m_BattleMord;	// trueであれば戦闘中
};
