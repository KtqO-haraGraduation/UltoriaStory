// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Battle/Combat/Components/ComponentBase.h"
#include "GameFramework/Character.h"
#include "../Data/EquipItemRow.h"
#include "CharacterBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHPChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMPChanged);

class UWidgetComponent;

#define LEVEL_MAX 20

// プレイヤータイプ
UENUM(BlueprintType)
enum class EPlayerType : uint8
{
	MAIN,	// 主人公
	ALLY1,	// 味方1
	ALLY2,	// 味方2
	ALLY3,	// 味方3
};

// チームタイプ
UENUM(BlueprintType)
enum class ETeamType : uint8
{
	PLAYER,
	ENEMY,
};

// プレイヤーと敵のステータスの構造体
struct FStatus
{
	int m_TotalEXP	 = 0;	// 累計経験値		※詳細：プレイヤーのレベルアップに必要な経験値
	int m_HP		 = 0;	// 体力
	int m_MP		 = 0;	// マナ
	int m_ATK		 = 0;	// 攻撃力
	int m_MGC		 = 0;	// 魔力
	int m_DEF		 = 0;	// 防御力
	int m_SPD		 = 0;	// 素早さ
	int m_Break		 = 0;	// ブレイク力
	int m_BreakGauge = 0;	// ブレイクゲージ	※詳細：体力のブレイク版
};

UCLASS()
class ULTORIAPROJECT_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACharacterBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//void LoadAllyStatusData(FStatus status[], EPlayerType type);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Get関数
	FStatus GetBaseStatus() const { return m_BaseStatus; }
	FStatus GetEquipmentBonus() const { return m_EquipmentBonus; }

	// 最終ステータス
	virtual FStatus GetTotalStatus() const { return FStatus(); }

	FName GetID() const { return m_ID; }
	FString GetCharacterName() const { return m_Name; }
	int GetLevel() const { return m_Level; }
	virtual int GetExp() const { return m_Exp; }
	int GetCurrentHP() const { return m_CurrentHP; }
	int GetCurrentMP() const { return m_CurrentMP; }
	int GetMaxHP() const { return m_MaxHP; }
	int GetMaxMP() const { return m_MaxMP; }
	ETeamType GetTeamType() const { return m_TeamType; }

	UFUNCTION()
	virtual float GetHPRate();
	UFUNCTION()
	virtual float GetMPRate();

	//Set関数
	virtual void SetBaseStatus();
	virtual void SetEquipmentBonus(FStatus bonus);

	virtual void SetHP(int newHP);
	virtual void SetMP(int newMP);

	void SetSelectionUIVisibility(bool flag); // 選択時のUIの表示 / 非表示

	// セーブデータからキャラクターのステータスを復元
	void LoadSaveStatus(int level, int exp, int hp, int mp);

	// コンポーネント取得
	template<typename T>
	T* GetComponent() const
	{
		return FindComponentByClass<T>();
	}

	// コンポーネント追加
	template<typename T>
	T* AddComponent(FName name)
	{
		T* component = NewObject<T>(this, name);

		component->RegisterComponent();
		AddInstanceComponent(component);

		m_Components.Add(component);

		return component;
	}

	FOnHPChanged& GetOnHPChanged() { return m_OnHPChanged; }
	FOnMPChanged& GetOnMPChanged() { return m_OnMPChanged; }

	// 装備している武器・防具を追加する関数
	UFUNCTION(BlueprintCallable)
	void AddEquipItem(EEquipSlot Slot, FName id);

	// 装備している武器・防具のIDを取得する関数
	UFUNCTION(BlueprintCallable)
	FName GetEquippedId(EEquipSlot Slot);

protected:
	bool CheckLevelUp(int nextLevelTotalExp);	// レベルが上がったか
	void LevelUp();								// レベルが上がった

	void InitComponent();	// コンポーネントの初期化

protected:
	UPROPERTY(EditAnywhere)
	FName m_ID;

	FString m_Name;	// 名前
	int m_Level;	// レベル
	int m_Exp;		// 経験値
	int m_CurrentHP;// 現在のHP
	int m_CurrentMP;// 現在のMP
	int m_MaxHP;	// 最大HP
	int m_MaxMP;	// 最大MP

	ETeamType m_TeamType; // チームタイプ

	FStatus m_BaseStatus;	  // ステータス(基本値)
	FStatus m_EquipmentBonus; // 装備補正値

	// 装備している武器・防具
	UPROPERTY()
	TMap<EEquipSlot, FName> m_EquippedIds;

	// コンポーネントリスト
	TArray<UComponentBase*> m_Components;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UWidgetComponent* m_SelectionUIWidget;

	UPROPERTY(BlueprintAssignable)
	FOnHPChanged m_OnHPChanged;

	UPROPERTY(BlueprintAssignable)
	FOnMPChanged m_OnMPChanged;
};
