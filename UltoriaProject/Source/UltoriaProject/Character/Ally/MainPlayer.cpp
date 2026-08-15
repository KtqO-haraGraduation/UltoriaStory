// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "../../Job/JobDataSubsystem.h"
#include "../../Battle/Combat/Components/PlayerCombatComponent.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Battle/Encounter/EncounterManager.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// デバッグ用Include
#include "Engine/Engine.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_JobBase = nullptr;
	m_JobData = nullptr;
	m_JobId = 0;
}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	// 仮実装（Playerをパーティーに追加）
	UPlayerPartyManager* partyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (partyMg)
	{
		partyMg->AddPlayerParty(this);
	}

	// 仮実装（ステージに合わせた敵のパーティーデータをロード）
	UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (dataMg)
	{
		dataMg->LoadEnemyPartyData();
	}

	m_AttackBox = FindComponentByClass<UBoxComponent>();

	if (m_AttackBox)
	{
		m_AttackBox->SetHiddenInGame(true);
		m_AttackBox->SetCollisionEnabled(
			ECollisionEnabled::NoCollision
		);
	}

	m_CameraBoom = FindComponentByClass<USpringArmComponent>();
	m_FollowCamera = FindComponentByClass<UCameraComponent>();

	m_TeamType = ETeamType::PLAYER;
	SetStatus();

	//LoadAllyStatusData(m_Status, m_PlayerType);	// 味方のステータスのマスターデータをロード

	JobChange();
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_IsAttacking)
	{
		m_TestCount -= GetWorld()->GetDeltaSeconds();

		if (m_TestCount <= 0)
		{
			AttackEnd();
			m_TestCount = 60;
			m_IsAttacking = false;
		}
	}
}

void AMainPlayer::SetPrevPos()
{
	SetActorLocation(m_PrevPos);
}

void AMainPlayer::Move(const FInputActionValue& Value)
{
	if (m_IsAttacking) return;

	// アナログスティックの入力をベクトルで取得
	FVector2D moveVec2D = Value.Get<FVector2D>();

	if (Controller)
	{
		FRotator rot = Controller->GetControlRotation();
		FRotator yawRot(0.0f, rot.Yaw, 0.0f);

		// カメラの前方・右方向を取得
		FVector forward = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);
		FVector right = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);

		// カメラの前方・右方向を基準に移動
		AddMovementInput(forward, moveVec2D.Y);
		AddMovementInput(right, moveVec2D.X);
	}
}

void AMainPlayer::Attack(const FInputActionValue& Value)
{
	if (m_IsAttacking) return;

	m_IsAttacking = true;

	// 一時的に当たり判定を作る

	m_AttackBox->SetHiddenInGame(false);
	m_AttackBox->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	UE_LOG(LogTemp, Warning, TEXT("Attack Start"));
}

void AMainPlayer::AttackEnd()
{
	m_AttackBox->SetHiddenInGame(true);
	m_AttackBox->SetCollisionEnabled(
		ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Warning, TEXT("Attack End"));
}

void AMainPlayer::StartBattle(ACharacterBase* otherActor, bool isPreemptive)
{
	if (!otherActor) return;
	if (otherActor->GetTeamType() != ETeamType::ENEMY) return;

	m_PrevPos = GetActorLocation();

	// エンカウント
	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	if (encounter)
	{
		encounter->StartEncounter(this, otherActor, isPreemptive);
	}
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

// デバッグ用のレベルに合わせたステータスにする
//FStatus AMainPlayer::SetStatusByDebugLevel()
//{
//	FStatus status;
//	
//	if (m_DebugLevel < 1)	// Level1未満だったら
//	{
//		m_DebugLevel = 1;	// Level1にする
//	}
//	else if (m_DebugLevel > LEVEL_MAX)	// LevelMaxを超えたら
//	{
//		m_DebugLevel = LEVEL_MAX;	// LevelMaxにする
//	}
//	// デバッグ用のレベルに合わせたステータスにする
//	status = m_Status[m_DebugLevel];
//
//	m_Level = m_DebugLevel;	
//	m_Exp = m_Status[m_DebugLevel].m_TotalEXP;
//
//	return status;
//}

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
