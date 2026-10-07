// Fill out your copyright notice in the Description page of Project Settings.

#include "TestPlayerMC.h"
#include "../../Combat/Components/PlayerCombatComponent.h"
#include "../../BattleManager.h"
#include "../../PlayerPartyManager.h"
#include "../../Combat/Data/BattleDataManager.h"
#include "../../Encounter/EncounterManager.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ATestPlayerMC::ATestPlayerMC()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATestPlayerMC::BeginPlay()
{
	Super::BeginPlay();

	// 仮実装（Playerをパーティーに追加）
	UPlayerPartyManager* playerParty = GetWorld()->GetSubsystem<UPlayerPartyManager>();
	//playerParty->AddPlayerParty(this);

	// 仮実装（バトルに必要なデータを読み込む）
	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();
	if (dataManager)
	{
		dataManager->LoadEnemyPartyData();
	}

	m_AttackBox = FindComponentByClass<UBoxComponent>();

	if (m_AttackBox)
	{
		m_AttackBox->SetHiddenInGame(true);
		m_AttackBox->SetCollisionEnabled(
			ECollisionEnabled::NoCollision);
	}

	m_CameraBoom   = FindComponentByClass<USpringArmComponent>();
	m_FollowCamera = FindComponentByClass<UCameraComponent>();

	m_TeamType = ETeamType::PLAYER;
	m_Name = "Player";
	m_Level = 1;
	m_Exp = 300;

	m_MaxHP = 2000;
	m_MaxMP = 500;

	// 仮実装
	m_BaseStatus.m_TotalEXP = 0;
	m_BaseStatus.m_HP = m_MaxHP;
	m_BaseStatus.m_MP = m_MaxMP;
	m_BaseStatus.m_ATK = 300;
	m_BaseStatus.m_DEF = 300;
	m_BaseStatus.m_MGC = 300;
	m_BaseStatus.m_SPD = 300;
	m_BaseStatus.m_Break = 300;
	m_BaseStatus.m_BreakGauge = 300;
}

// Called every frame
void ATestPlayerMC::Tick(float DeltaTime)
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

void ATestPlayerMC::SetPrevPos()
{
	SetActorLocation(m_PrevPos);
}

void ATestPlayerMC::Move(const FInputActionValue& Value)
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

void ATestPlayerMC::Attack(const FInputActionValue& Value)
{
	if (m_IsAttacking) return;

	m_IsAttacking = true;

	// 一時的に当たり判定を作る

	m_AttackBox->SetHiddenInGame(false);
	m_AttackBox->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	UE_LOG(LogTemp, Warning, TEXT("Attack Start"));
}

void ATestPlayerMC::AttackEnd()
{
	m_AttackBox->SetHiddenInGame(true);
	m_AttackBox->SetCollisionEnabled(
		ECollisionEnabled::NoCollision);

	UE_LOG(LogTemp, Warning, TEXT("Attack End"));
}

void ATestPlayerMC::StartBattle(ACharacterBase* otherActor, bool isPreemptive)
{
	if (!otherActor) return;
	if (otherActor->GetTeamType() != ETeamType::ENEMY) return;

	m_PrevPos = GetActorLocation();

	// エンカウント
	UEncounterManager* encounter = GetWorld()->GetSubsystem<UEncounterManager>();
	if (encounter)
	{
		encounter->StartEncounter(otherActor, isPreemptive);
	}
}

