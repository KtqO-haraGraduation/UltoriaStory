// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"
#include "EnemyManager.h"
#include "../../UI/Battle/CharacterStatusWidget.h"
#include "../../Battle/Combat/Data/BattleDataManager.h"
#include "../../Battle/BattleManager.h"
#include "../../Job/Skill/SkillBaseComponent.h"
#include "../../Job/Skill/SkillParameter.h"
#include "../Data/CharacterDataManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"

// アイテムの落とす種類の乱数
constexpr int DROP_ITEM_TYPE_MIN = 1;
constexpr int DROP_ITEM_TYPE_MAX = 2;
// アイテムの落とす個数の乱数
constexpr int DROP_ITEM_COUNT_MIN = 1;
constexpr int DROP_ITEM_COUNT_MAX = 2;

// お金を落とす割合
constexpr float DROP_GOLD_RANGE_MIN = 0.8f;
constexpr float DROP_GOLD_RANGE_MAX = 1.2f;

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
	
	m_CharacterDataMg = GetWorld()->GetSubsystem<UCharacterDataManager>();

	m_TeamType = ETeamType::ENEMY;
	// ステータスを設定
	SetBaseStatus();
	CreateSkill();

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

	UEnemyManager* enemyMg = GetWorld()->GetSubsystem<UEnemyManager>();
	UBattleManager* battleMg = GetWorld()->GetSubsystem<UBattleManager>();
	if (enemyMg && battleMg && !battleMg->IsBattle())
	{
		enemyMg->AddEnemy(this);
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
	if (!m_CharacterDataMg) return result;

	// ランダムでパティーデータを取得
	FEnemyPartyData data = m_CharacterDataMg->GetRandomEnemyParty(m_ID);

	// 生成時の当たり判定緩和（ある程度キャラクター同士が重なってても生成してくれる）
	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (const FName& enemyId : data.Enemies)
	{
		if (enemyId.IsNone()) continue;

		// クラス取得(GetEnemyClassをEnemyBaseクラスに変更する)
		TSubclassOf<AEnemyBase> enemyClass = m_CharacterDataMg->GetEnemyClass(m_ID);
		if (!enemyClass) continue;

		// 敵生成
		AEnemyBase* enemy = GetWorld()->SpawnActor<AEnemyBase>(
			enemyClass,
			GetActorLocation(),
			FRotator::ZeroRotator,
			spawnParams);

		// 成功したら追加
		if (enemy)
		{
			// AIControllerを生成して敵に設定
			enemy->SpawnDefaultController();
			result.Add(enemy);
		}
	}

	// 自分自身を削除
	Destroy();

	return result;
}

void AEnemyBase::CreateSkill()
{
	USkillBaseComponent* skillComponent = GetComponent<USkillBaseComponent>();
	if (!skillComponent) return;

	skillComponent->Reset();
	const FEnemyMasterData data = m_CharacterDataMg->GetEnemyData(m_ID);

	for (const FName skill : data.Skills)
	{
		skillComponent->Create(skill);
	}
}

TArray<FDropItemData> AEnemyBase::GetDropItems()
{
	TArray<FDropItemData> result = {};
	const FEnemyMasterData data = m_CharacterDataMg->GetEnemyData(m_ID);

	// 確定ドロップ
	for (const FName& fixedItem : data.FixedDropItems)
	{
		AddDropItem(result, fixedItem, 1);
	}

	// 確率ドロップ
	// 取得するアイテムの種類を選択
	int randomValue = FMath::RandRange(DROP_ITEM_TYPE_MIN, DROP_ITEM_TYPE_MAX);

	// すでにドロップしたかの記録用リスト
	TArray<FName> droppedItems = {};

	// ドロップする種類分回す
	for (int i = 0; i < randomValue; i++)
	{
		FName dropItem;
		// すでにドロップしていれば、再抽選
		do
		{
			dropItem = GetRnadomDropItem();

		} while (droppedItems.Contains(dropItem));
		droppedItems.Add(dropItem);

		// ドロップの種類が2種類なら落とす個数（最大）が1になる
		int dropCountMax = randomValue == DROP_ITEM_COUNT_MIN ?
			DROP_ITEM_COUNT_MAX : DROP_ITEM_COUNT_MIN;

		// 獲得アイテムの個数を選択
		int itemGetCount = FMath::RandRange(DROP_ITEM_COUNT_MIN, dropCountMax);

		AddDropItem(result, dropItem, itemGetCount);
	}
	return result;
}

int AEnemyBase::GetDropGold()
{
	const FEnemyMasterData data = m_CharacterDataMg->GetEnemyData(m_ID);

	// 80％～120％の範囲で乱数を取得
	float randomRate = FMath::FRandRange(DROP_GOLD_RANGE_MIN, DROP_GOLD_RANGE_MAX);

	int dropGold = FMath::RoundToInt(data.DropGold * randomRate);
	return dropGold;
}

FName AEnemyBase::GetRnadomDropItem()
{
	const FEnemyMasterData data = m_CharacterDataMg->GetEnemyData(m_ID);

	if (data.DropItems.IsEmpty())
	{
		return "";
	}

	int index = FMath::RandRange(0, data.DropItems.Num() - 1);
	return data.DropItems[index];
}

void AEnemyBase::AddDropItem(TArray<FDropItemData>& items, const FName& itemID, int count)
{
	if (itemID.IsNone())
	{
		return;
	}

	FDropItemData* findItem = items.FindByPredicate(
		[&itemID](const FDropItemData& item)
		{
			return item.itemName == itemID;
		}
	);

	if (findItem)
	{
		findItem->count += count;
	}
	else
	{
		FDropItemData newItem;
		newItem.itemName = itemID;
		newItem.count = count;

		items.Add(newItem);
	}
}

FStatus AEnemyBase::GetTotalStatus() const
{
	// 基本値 + 装備補正値 + ジョブ補正値
	FStatus result = {};

	result.m_HP  = m_BaseStatus.m_HP;
	result.m_MP  = m_BaseStatus.m_MP;
	result.m_ATK = m_BaseStatus.m_ATK;
	result.m_DEF = m_BaseStatus.m_DEF;
	result.m_MGC = m_BaseStatus.m_MGC;
	result.m_SPD = m_BaseStatus.m_SPD;

	return result;
}

void AEnemyBase::SetStatusUIVisibility(bool flag)
{
	if (!m_StatusUIWidget) return;
	m_StatusUIWidget->SetVisibility(flag);
}

void AEnemyBase::SetBaseStatus()
{
	FEnemyMasterData data = m_CharacterDataMg->GetEnemyData(m_ID);

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

	FStatus totalStatus = GetTotalStatus();

	m_MaxHP = totalStatus.m_HP;
	m_MaxMP = totalStatus.m_MP;

	m_CurrentHP = totalStatus.m_HP;
	m_CurrentMP = totalStatus.m_MP;
}
