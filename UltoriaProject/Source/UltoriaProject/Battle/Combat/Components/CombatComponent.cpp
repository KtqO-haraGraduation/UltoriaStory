// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Status/CombatStatusComponent.h"
#include "../../../Character/CharacterBase.h"
#include "../../BattleManager.h"
#include "../../BattleGaugeManager.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../UI/Battle/DamageWidget.h"
#include "../Data/BattleDataManager.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UCombatComponent::BattleStart()
{
	UCombatComponent* combat = m_Owner->GetComponent<UCombatComponent>();
	UCombatStatusComponent* status = m_Owner->GetComponent<UCombatStatusComponent>();

	if (combat && status)
	{
		combat->SetActiveComponent(true);
		status->SetActiveComponent(true);
	}
}

void UCombatComponent::BattleEnd()
{
	UCombatComponent* combat = m_Owner->GetComponent<UCombatComponent>();
	UCombatStatusComponent* status = m_Owner->GetComponent<UCombatStatusComponent>();

	if (combat && status)
	{
		combat->SetActiveComponent(false);
		status->SetActiveComponent(false);
	}
}

void UCombatComponent::StartTurn()
{
}

void UCombatComponent::Attack(ACharacterBase* target)
{
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();

	if (battleManager)
	{
		m_CurrentState = CombatState::ATTACK;

		// 指定した位置まで移動

		// 攻撃アニメーション開始

		// 攻撃アニメーション終了時に、ダメージ処理


		m_Target = target;

		// プレイヤーはアニメーションがないため、一時的にNextTurnへ
		if (m_Owner->GetTeamType() == ETeamType::PLAYER)
		{
			OnAttackHit();

			// 次のキャラへ（0.2秒後）
			FTimerHandle Timer;
			GetWorld()->GetTimerManager().SetTimer(
				Timer,
				[this, battleManager]()
				{
					battleManager->NextTurn();
				},
				0.2f,
				false
			);
		}
	}
}

void UCombatComponent::Guard()
{
	m_CurrentState = CombatState::GUARD;

	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	UCombatStatusComponent* statusComponent = m_Owner->GetComponent<UCombatStatusComponent>();
	statusComponent->SetGuarding(true);

	statusComponent->AddStatus(StatusType::GUARD, "Guard");

	// 次のキャラへ（0.2秒後）
	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(
		Timer,
		[this, battleManager]()
		{
			battleManager->NextTurn();
		},
		0.2f,
		false
	);
}

void UCombatComponent::Skill()
{
	m_CurrentState = CombatState::SKILL;
}

void UCombatComponent::Item()
{
	m_CurrentState = CombatState::ITEM;
}

bool UCombatComponent::Escape()
{
	UBattleManager* battleManager = GetWorld()->GetSubsystem<UBattleManager>();
	m_CurrentState = CombatState::ESCAPE;
	int rand = FMath::RandRange(0, 99);
	

	bool isEscape = rand < m_EscapeRate;

	if (!isEscape && battleManager)
	{
		// 次のキャラへ（0.2秒後）
		FTimerHandle Timer;
		GetWorld()->GetTimerManager().SetTimer(
			Timer,
			[this, battleManager]()
			{
				battleManager->NextTurn();
			},
			0.2f,
			false
		);

		UE_LOG(LogTemp, Warning, TEXT("No Escape"));

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Yes Escape"));
	}

	return isEscape;
}

void UCombatComponent::TakeDamage(int damage)
{
	m_CurrentState = CombatState::DAMEGE;

	UCombatStatusComponent* statusComponent = m_Owner->GetComponent<UCombatStatusComponent>();
	bool isGuarding = statusComponent->IsGuarding();

	UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	FBattleStatusData data;

	if (isGuarding) data = dataMg->FindStatusData("Guard");

	// 防御中は、ダメージを半分にする
	int nweDamage = isGuarding ? (damage / data.value) : damage;

	int newHP = m_Owner->GetBaseStatus().m_HP - nweDamage;
	
	m_Owner->SetHP(newHP);

	// オーナーからHPを取得して、damege 分減らす

	// HP が 0 以下だったら、死亡判定
	if (newHP <= 0)
	{
		// HP を 0 にする
		Dead();
	}

	// ダメージのUIを表示
	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (uiMg)
	{
		uiMg->CreateDamageWidget(m_Owner, nweDamage);
	}
}

void UCombatComponent::Deading()
{
	m_CurrentState = CombatState::DEADING;
}

void UCombatComponent::Dead()
{
	if (m_CurrentState == CombatState::DEAD) return;

	m_CurrentState = CombatState::DEAD;
}

void UCombatComponent::ResetState()
{
	if (m_CurrentState == CombatState::DEADING ||
		m_CurrentState == CombatState::DEAD) return;

	m_CurrentState = CombatState::IDLE;
	m_IsGuarding = false;
}

void UCombatComponent::OnAttackHit()
{
	if (!m_Target) return;
	UCombatComponent* combat = m_Target->GetComponent<UCombatComponent>();
	combat->TakeDamage(m_Owner->GetBaseStatus().m_ATK);
}

void UCombatComponent::OnActionEnd()
{
	ResetState();

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (battleMg)
	{
		battleMg->NextTurn();
	}
}

void UCombatComponent::OnDead()
{
	m_Owner->SetActorHiddenInGame(true);
}
