// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "../../Job/JobDataSubsystem.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
// デバッグ用Include
#include "Engine/Engine.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	for (int i = 1; i <= LEVEL_MAX; i++)
	{
		m_Status[i].m_TotalEXP = 0;
		m_Status[i].m_HP = 0;
		m_Status[i].m_MP = 0;
		m_Status[i].m_ATK = 0;
		m_Status[i].m_MGC = 0;
		m_Status[i].m_DEF = 0;
		m_Status[i].m_SPD = 0;
		m_Status[i].m_Break = 0;
		m_Status[i].m_BreakGauge = 0;
	}

	m_JobBase = nullptr;
	m_JobData = nullptr;
	m_JobId = 0;

	m_BattleMord = false;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	// インプットマップを設定
	APlayerController* playerController = Cast<APlayerController>(Controller);
	if (playerController)
	{
		UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer());
		if (subsystem)
		{
			subsystem->AddMappingContext(m_InputMap, 0);
		}
	}

	LoadAllyStatusData(m_Status, m_PlayerType);	// 味方のステータスのマスターデータをロード

	// デバッグ用
	if (m_DebugMord)
	{
		// レベルに合わせたステータスにする
		m_BaseStatus = SetStatusByDebugLevel();
	}

	JobChange();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 入力アクションを設定
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// ジャンプ
		enhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Started, this, &AMainPlayer::InputJump);
		enhancedInputComponent->BindAction(m_JumpAction, ETriggerEvent::Completed, this, &AMainPlayer::StopJumping);
		// 移動
		enhancedInputComponent->BindAction(m_StickMoveAction, ETriggerEvent::Triggered,this, &AMainPlayer::InputStickMove);
		enhancedInputComponent->BindAction(m_UpMoveAction, ETriggerEvent::Triggered, this, &AMainPlayer::InputUpMove);
		enhancedInputComponent->BindAction(m_DownMoveAction, ETriggerEvent::Triggered, this, &AMainPlayer::InputDownMove);
		enhancedInputComponent->BindAction(m_LeftMoveAction, ETriggerEvent::Triggered, this, &AMainPlayer::InputLeftMove);
		enhancedInputComponent->BindAction(m_RightMoveAction, ETriggerEvent::Triggered, this, &AMainPlayer::InputRightMove);
		// デバッグ用
		// ステータス確認
		enhancedInputComponent->BindAction(m_CheakStatus, ETriggerEvent::Started, this, &AMainPlayer::InputCheckStatus);
		// ジョブの切り替え
		enhancedInputComponent->BindAction(m_JobChange, ETriggerEvent::Started, this, &AMainPlayer::InputJobChange);
	}
}

/// <summary>
/// スティック入力移動処理
/// </summary>
/// <param name="value">入力値(FVector2D)</param>
void AMainPlayer::InputStickMove(const FInputActionValue& value)
{
	// アナログスティックの入力を取得
	m_StickInputVec = value.Get<FVector2D>();

	if (!IsControll()) return;

	// アナログスティックの座標系をUEの3D座標系に変換
	FVector moveVec(m_StickInputVec.Y, m_StickInputVec.X, 0.0f);

	// 移動
	AddMovementInput(moveVec, m_MoveSpeed);
}

/// <summary>
/// キーボード入力移動処理
/// </summary>
/// <param name="value">入力値(float)</param>
void AMainPlayer::InputUpMove(const FInputActionValue& value)
{
	// 入力加減を取得する キーボードなら0か1
	float inputValue = value.Get<float>();

	FVector moveVec(inputValue, 0.0f, 0.0f);

	AddMovementInput(moveVec, m_MoveSpeed);
}
void AMainPlayer::InputDownMove(const FInputActionValue& value)
{
	// 入力加減を取得する キーボードなら0か1
	float inputValue = value.Get<float>();

	FVector moveVec(-inputValue, 0.0f, 0.0f);

	AddMovementInput(moveVec, m_MoveSpeed);
}
void AMainPlayer::InputLeftMove(const FInputActionValue& value)
{
	// 入力加減を取得する キーボードなら0か1
	float inputValue = value.Get<float>();

	FVector moveVec(0.0f, -inputValue, 0.0f);

	AddMovementInput(moveVec, m_MoveSpeed);
}
void AMainPlayer::InputRightMove(const FInputActionValue& value)
{
	// 入力加減を取得する キーボードなら0か1
	float inputValue = value.Get<float>();

	FVector moveVec(0.0f, inputValue, 0.0f);

	AddMovementInput(moveVec, m_MoveSpeed);
}

// ジャンプ
void AMainPlayer::InputJump(const FInputActionValue& value)
{
	if (!IsControll()) return;

	Jump();
}
// 先制攻撃
void AMainPlayer::InputPreemptiveAttack(const FInputActionValue& value)
{
	if (!IsControll()) return;
}

/// <summary>
/// オーバーラップ処理
/// </summary>
/// <param name="otherActor">当たったアクター</param>
/// <param name="otherComp">当たったコンポーネント</param>
void AMainPlayer::BeginOverlap(AActor* otherActor, UPrimitiveComponent* otherComp)
{

}

// ジョブの切り替え
void AMainPlayer::JobChange()
{
	UJobDataSubsystem* jobDataSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UJobDataSubsystem>();
	if (!jobDataSubsystem)
	{
		return;	// UJobDataSubsystemの取得失敗
	}
	m_JobData = jobDataSubsystem->GetJobData(m_JobId);
	m_JobBase = jobDataSubsystem->GetJobBase(m_JobId);
}

// 戦闘開始
void AMainPlayer::StartBattle()
{
	m_BattleMord = true;
}

// 戦闘終了
void AMainPlayer::EndBattle()
{
	m_BattleMord = false;
}

// 操作可能な状態であるか
bool AMainPlayer::IsControll()
{
	if (m_BattleMord) return false;

	return true;
}

// ステータス確認
void AMainPlayer::InputCheckStatus(const FInputActionValue& value)
{
	// デバッグ専用の処理が使えるか
	if (!m_DebugMord)	return;
	if (!IsControll())	return;
	//if (!m_JobData) return;

	if (GEngine)
	{
		for (int i = 5; i >= 0; i--)
		{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_Duration	: %d"), i + 1, m_JobBase->m_SkillData[i]->m_Duration));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_UnLockLevel	: %d"), i + 1, m_JobBase->m_SkillData[i]->m_UnLockLv));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_BreakMult	: %f"), i + 1, m_JobBase->m_SkillData[i]->m_BreakMult));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_EffectValue	: %f"), i + 1, m_JobBase->m_SkillData[i]->m_EffectValue));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_MpCost		: %d"), i + 1, m_JobBase->m_SkillData[i]->m_MpCost));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_Name		: %s"), i + 1, *m_JobBase->m_SkillData[i]->m_Name));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Skill%d_Type		: %s"), i + 1, *m_JobBase->m_SkillData[i]->m_Type));
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("BreakGauge			: %d"), TotalStatus().m_BreakGauge));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Break				: %d"), TotalStatus().m_Break));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("SPD					: %d"), TotalStatus().m_SPD));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("DEF					: %d"), TotalStatus().m_DEF));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("MGC					: %d"), TotalStatus().m_MGC));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("ATK					: %d"), TotalStatus().m_ATK));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("MP					: %d"), TotalStatus().m_MP));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("HP					: %d"), TotalStatus().m_HP));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Job_EXP				: %d"), m_JobBase->m_Exp));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Job_LV				: %d"), m_JobBase->m_Lv));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Job_Name			: %s"), *m_JobData->m_Name));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player_EXP			: %d"), m_Exp));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player_LV			: %d"), m_Level));
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Player_Name			: %s"), *m_Name));
	}
}

// ジョブの切り替え(仮)
void AMainPlayer::InputJobChange(const FInputActionValue& value)
{
	if (!IsControll())	return;

	m_JobId = m_ChengeJobId;

	JobChange();
}

// デバッグ用のレベルに合わせたステータスにする
FStatus AMainPlayer::SetStatusByDebugLevel()
{
	FStatus status;
	
	if (m_DebugLevel < 1)	// Level1未満だったら
	{
		m_DebugLevel = 1;	// Level1にする
	}
	else if (m_DebugLevel > LEVEL_MAX)	// LevelMaxを超えたら
	{
		m_DebugLevel = LEVEL_MAX;	// LevelMaxにする
	}
	// デバッグ用のレベルに合わせたステータスにする
	status = m_Status[m_DebugLevel];

	m_Level = m_DebugLevel;	
	m_Exp = m_Status[m_DebugLevel].m_TotalEXP;

	return status;
}

/// <summary>
/// 総合ステータス
/// </summary>
/// <returns>総合ステータスを返す</returns>
FStatus AMainPlayer::TotalStatus()
{
	if (!m_JobData)
	{
		return m_BaseStatus;
	}

	FStatus result;

	result.m_HP			= m_BaseStatus.m_HP			* m_JobData->m_HP;
	result.m_MP			= m_BaseStatus.m_MP			* m_JobData->m_MP;
	result.m_ATK		= m_BaseStatus.m_ATK		* m_JobData->m_ATK;
	result.m_MGC		= m_BaseStatus.m_MGC		* m_JobData->m_MGC;
	result.m_DEF		= m_BaseStatus.m_DEF		* m_JobData->m_DEF;
	result.m_SPD		= m_BaseStatus.m_SPD		* m_JobData->m_SPD;
	result.m_Break		= m_BaseStatus.m_Break;
	result.m_BreakGauge = m_BaseStatus.m_BreakGauge;

	return result;
}