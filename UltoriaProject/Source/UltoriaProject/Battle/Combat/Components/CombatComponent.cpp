// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Status/CombatStatusComponent.h"
#include "../../../Character/CharacterBase.h"
#include "../../../UI/Battle/BattleUIManager.h"
#include "../../../UI/Battle/DamageWidget.h"
#include "../../../System/Effect/EffectManager.h"
#include "../../../System/Sound/SoundManager.h"
#include "../../BattleManager.h"
#include "../../BattleGaugeManager.h"
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
	if (!target) return;

	m_CurrentState = CombatState::ATTACK;
	m_Targets.Add(target);
}

void UCombatComponent::Guard()
{
	m_CurrentState = CombatState::GUARD;

	UCombatStatusComponent* statusComponent = m_Owner->GetComponent<UCombatStatusComponent>();
	statusComponent->SetGuarding(true);

	statusComponent->AddStatus(StatusType::GUARD);

	// 次のキャラへ（0.2秒後）
	NextTurn();
}

void UCombatComponent::Skill(const TArray<ACharacterBase*>& targets)
{
	m_CurrentState = CombatState::SKILL;
	
	

}

void UCombatComponent::Item(ACharacterBase* target)
{
	m_CurrentState = CombatState::ITEM;


}

bool UCombatComponent::Escape()
{
	m_CurrentState = CombatState::ESCAPE;
	int rand = FMath::RandRange(0, 99);

	bool isEscape = rand < m_EscapeRate;
	if (!isEscape)
	{
		// 次のキャラへ（0.2秒後）
		NextTurn();
	}

	return isEscape;
}

void UCombatComponent::TakeDamage(int damage)
{
	m_CurrentState = CombatState::DAMEGE;

	UCombatStatusComponent* statusComponent = m_Owner->GetComponent<UCombatStatusComponent>();
	// ガード中か
	bool isGuarding = statusComponent->IsGuarding();

	UBattleDataManager* dataMg = GetWorld()->GetSubsystem<UBattleDataManager>();
	FBattleStatusData data;

	// ガード値を取得
	if (isGuarding)
	{
		data = dataMg->FindStatusData(StatusType::GUARD);
	}

	// 防御中は、ダメージを半分にする
	int nweDamage = isGuarding ? (damage / data.value) : damage;

	int newHP = m_Owner->GetCurrentHP() - nweDamage;
	
	m_Owner->SetHP(newHP);

	// SoundとEffectを再生
	USoundManager* soundMg   = GetWorld()->GetGameInstance()->GetSubsystem<USoundManager>();
	UEffectManager* effectMg = GetWorld()->GetGameInstance()->GetSubsystem<UEffectManager>();

	if (soundMg) soundMg->PlaySE("SE_Hit");
	if (effectMg) effectMg->PlayEffect("NS_Hit", m_Owner->GetActorLocation());

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

void UCombatComponent::HPHeal(int healHp)
{
	m_CurrentState = CombatState::HEAL;

	int newHp = m_Owner->GetCurrentHP() + healHp;
	m_Owner->SetHP(newHp);

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (uiMg)
	{
		uiMg->CreateDamageWidget(m_Owner, healHp);
	}

	// 次のキャラへ（0.2秒後）
	NextTurn();
}

void UCombatComponent::MPHeal(int healMp)
{
	m_CurrentState = CombatState::HEAL;

	int newMp = m_Owner->GetCurrentMP() + healMp;
	m_Owner->SetMP(newMp);

	UBattleUIManager* uiMg = GetWorld()->GetSubsystem<UBattleUIManager>();
	if (uiMg)
	{
		uiMg->CreateDamageWidget(m_Owner, newMp);
	}

	// 次のキャラへ（0.2秒後）
	NextTurn();
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

void UCombatComponent::NextTurn()
{
	// 攻撃状態 かつ 敵はスキップ
	if (m_CurrentState == CombatState::ATTACK && 
		m_Owner->GetTeamType() == ETeamType::ENEMY) return;

	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (!battleMg) return;

	// 待機時間
	float waitTime = 0.3f;

	OnAttackHit();
	
	// 状態をリセット
	ResetState();
	ResetTargets();

	FTimerHandle Timer;
	GetWorld()->GetTimerManager().SetTimer(
		Timer,
		[this, battleMg]()
		{
			battleMg->NextTurn();
		},
		waitTime,
		false
	);
}

void UCombatComponent::OnAttackHit()
{
	if (m_Targets.IsEmpty()) return;

	for (ACharacterBase* target : m_Targets)
	{
		if (!target) continue;
		UCombatComponent* combat = target->GetComponent<UCombatComponent>();

		combat->TakeDamage(m_Owner->GetBaseStatus().m_ATK);
	}
}

void UCombatComponent::OnActionEnd()
{
	ResetState();
	ResetTargets();

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

void UCombatComponent::ResetTargets()
{
	m_Targets.Empty();
}
