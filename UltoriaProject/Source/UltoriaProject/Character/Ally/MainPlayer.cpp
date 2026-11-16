// Fill out your copyright notice in the Description page of Project Settings.

#include "MainPlayer.h"
#include "../../Job/JobDataSubsystem.h"
#include "../../Battle/Combat/Components/PlayerCombatComponent.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "../../Battle/PlayerPartyManager.h"
#include "../../Battle/Encounter/EncounterManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Stage/StageManager.h"
#include "../../Camera/CameraManager.h"
#include "../Data/CharacterDataManager.h"


#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;
	if (battleMg->IsBattle()) return;

	// 仮実装（Playerをパーティーに追加）
	UPlayerPartyManager* partyMg = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	if (partyMg)
	{
		FPartyData data_1;
		data_1.characterID = m_ID;

		FPartyData data_2;
		data_2.characterID = "Ally001";

		FPartyData data_3;
		data_3.characterID = "Ally002";

		FPartyData data_4;
		data_4.characterID = "Ally003";

		partyMg->AddPlayerParty(data_1);
		partyMg->AddPlayerParty(data_2);
		partyMg->AddPlayerParty(data_3);
		partyMg->AddPlayerParty(data_4);

		//partyMg->CreatePlayerParty(this);
	}

	partyMg->SetFieldPlayer(this);

	// 仮実装（ステージに合わせた敵のパーティーデータをロード）
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (characterDataMg)
	{
		characterDataMg->LoadEnemyPartyData();
	}

	// 先制攻撃用当たり判定を取得
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

	FRotator initRot = GetActorRotation();

	// スティックを上に入力時、Yawは90.0fになるので、「奥移動 = 0」になるように補正を掛ける
	m_InitYawOffset = initRot.Yaw + 90.0f;
}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 先制攻撃の仮実装
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

	UCameraManager* cameraMg = GetWorld()->GetGameInstance()->GetSubsystem<UCameraManager>();
	if(!cameraMg) return;
	UCameraComponent* camera = cameraMg->GetFieldCameraComponent();

	// アナログスティックの入力をベクトルで取得
	FVector2D moveVec2D = Value.Get<FVector2D>();

	if (Controller && camera)
	{
		FRotator rot = camera->GetComponentRotation();
		FRotator yawRot(0.0f, rot.Yaw, 0.0f);

		// カメラの前方・右方向を取得
		FVector forward = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);
		FVector right = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);

		// カメラの前方・右方向を基準に移動
		AddMovementInput(forward, moveVec2D.Y);
		AddMovementInput(right, moveVec2D.X);

		// プレイヤーの回転処理
		// スティックのYaw(ヨー)を取得
		float stickYaw = FMath::RadiansToDegrees(FMath::Atan2(moveVec2D.Y, -moveVec2D.X));

		float targetYaw = stickYaw - m_InitYawOffset;

		// プレイヤーを回転させる
		SetActorRotation(FRotator(0.0f, targetYaw, 0.0f));
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

void AMainPlayer::ZoomCamera(const FInputActionValue& Value)
{
	if (m_IsAttacking) return;

	// 入力をfloatで取得し、ズーム量にする
	float zoomValue = Value.Get<float>() * 20.0f;

	// ズーム量を加算
	m_CameraBoom->TargetArmLength += zoomValue;

	// TargetArmLengthがズームの最大値、また最小値に達していないか
	// 達して入れば各値に変更する
	if (m_CameraBoom->TargetArmLength > 10000.0f)
	{
		m_CameraBoom->TargetArmLength = 10000.0f;
	}
	else if(m_CameraBoom->TargetArmLength < 500.0f)
	{
		m_CameraBoom->TargetArmLength = 500.0f;
	}
}

void AMainPlayer::ZoomReset(const FInputActionValue& Value)
{
	if (m_IsAttacking) return;

	// 入力時の値が2未満なら無視
	if (Value.Get<float>() < 2.0f)
	{
		return;
	}

	// ズーム量を初期化
	m_CameraBoom->TargetArmLength = 5000.0f;
}

/// <summary>
/// オーバーラップ処理（バトル開始）
/// </summary>
/// <param name="otherActor">当たったアクター</param>
/// <param name="isPreemptive">先制攻撃に成功したかどうか（true：成功, false：失敗）</param>
void AMainPlayer::StartBattle(ACharacterBase* otherActor, bool isPreemptive)
{
	if (!otherActor) return;
	if (otherActor->GetTeamType() != ETeamType::ENEMY) return;

	m_PrevPos = GetActorLocation();

	// エンカウント
	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	if (encounter)
	{
		
		encounter->SetEnemy(otherActor);
		encounter->EnterBattle();

		//encounter->StartEncounter(otherActor, isPreemptive);
	}
}

void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
