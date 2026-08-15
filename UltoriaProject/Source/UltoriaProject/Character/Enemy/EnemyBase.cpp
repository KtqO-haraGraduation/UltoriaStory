// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "../../UI/Battle/CharacterStatusWidget.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "../Data/CharacterDataManager.h"
#include "../../Battle/BattleManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	m_TeamType = ETeamType::ENEMY;
	// ステータスを設定
	SetStatus();

	// UIを取得
	TArray<UWidgetComponent*> widgets;
	GetComponents<UWidgetComponent>(widgets);

	for (UWidgetComponent* widget : widgets)
	{
		if (widget->ComponentHasTag(TEXT("Status")))
		{
			m_StatusUIWidget = widget;
			

			// 仮実装（バトル時のみ、キャラクターをセットして、ステータスUIを表示する）
			UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
			if (battleMg->IsBattle())
			{
				// 敵だけMPのUIを非表示に設定する
				UCharacterStatusWidget* statusWidget = Cast<UCharacterStatusWidget>(widget->GetUserWidgetObject());
				if (statusWidget)
				{
					m_StatusUIWidget->SetVisibility(true);
					statusWidget->SetCharacter(this);
					statusWidget->HideMPUI();
				}
			}
		}
		else if (widget->ComponentHasTag(TEXT("Selection")))
		{
			m_SelectionUIWidget = widget;
		}
	}
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// コンポーネントの更新処理
	for (auto& component : m_Components)
	{
		if (component->IsActiveComponent())
		{
			component->Update();
		}
	}
}

// Called to bind functionality to input
void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

TArray<ACharacterBase*> AEnemyBase::CreateEnemyParty()
{
	TArray<ACharacterBase*> result;

	UBattleDataManager* dataManager = GetWorld()->GetSubsystem<UBattleDataManager>();
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();

	if (dataManager)
	{
		// ランダムでパティーデータを取得
		FEnemyPartyData data = dataManager->GetRandomEnemyParty(m_ID);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *data.Name.ToString());

		TArray<FName> enemyIds =
		{
			data.Enemy1,
			data.Enemy2,
			data.Enemy3,
			data.Enemy4,
		};

		for (const FName& enemyId : enemyIds)
		{
			if (enemyId.IsNone()) continue;

			// クラス取得(GetEnemyClassをEnemyBaseクラスに変更する)
			TSubclassOf<AEnemyBase> enemyClass = characterDataMg->GetEnemyClass(m_ID);
			if (!enemyClass) continue;

			// 敵生成
			AEnemyBase* enemy = GetWorld()->SpawnActor<AEnemyBase>(
				enemyClass,
				GetActorLocation(),
				FRotator::ZeroRotator);

			// 成功したら追加
			if (enemy)
			{
				// AIControllerを生成して敵に設定
				enemy->SpawnDefaultController();
				result.Add(enemy);
			}
		}
	}

	// 自分自身を削除
	Destroy();

	return result;
}

void AEnemyBase::SetStatusUIVisibility(bool flag)
{
	if (!m_StatusUIWidget) return;
	m_StatusUIWidget->SetVisibility(flag);
}

void AEnemyBase::SetSelectionUIVisibility(bool flag)
{
	if (!m_SelectionUIWidget) return;
	m_SelectionUIWidget->SetVisibility(flag);
}

void AEnemyBase::SetStatus()
{
	UCharacterDataManager* characterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();
	if (!characterDataMg) return;

	FEnemyMasterData data = characterDataMg->GetEnemyData(m_ID);

	m_Name = data.Name.ToString();
	m_BaseStatus.m_TotalEXP		= data.EXP;
	m_BaseStatus.m_HP			= data.HP;
	m_BaseStatus.m_MP			= data.MP;
	m_BaseStatus.m_ATK			= data.ATK;
	m_BaseStatus.m_MGC			= data.MGC;
	m_BaseStatus.m_DEF			= data.DEF;
	m_BaseStatus.m_SPD			= data.SPD;
	m_BaseStatus.m_Break		= data.Break;
	m_BaseStatus.m_BreakGauge	= data.BreakGauge;

	m_MaxHP = data.HP;
	m_MaxMP = data.MP;
}
