// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ComponentBase.h"
#include "../../Data/BattleStatusData.h"
#include "CombatStatusComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ULTORIAPROJECT_API UCombatStatusComponent : public UComponentBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UCombatStatusComponent();

	// 全ての状態コンポーネントを更新
	void UpdateTurn();

	// 状態リストに追加
	void AddStatus(StatusType type);

	// 状態リストの中身を検索
	bool HasStatus(StatusType type) const;
	// 取得
	FBattleStatusData* FindStatus(StatusType type);

	bool IsBreaking() const { return m_IsBreaking; }
	bool IsGuarding() const { return m_IsGuarding; }
	bool IsUltoria() const { return m_IsUltoria; }

	void SetBreaking(bool flag) { if(m_IsBreaking != flag) m_IsBreaking = flag; }
	void SetGuarding(bool flag) { if(m_IsGuarding != flag) m_IsGuarding = flag; }
	void SetUltoria(bool flag)  { if(m_IsUltoria != flag) m_IsUltoria = flag; }

private:
	// それぞれの効果
	void UpdateHeal(const FBattleStatusData& status);
	void UpdatePoison(const FBattleStatusData& status);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
private:
	bool m_IsBreaking; // ブレイク中か
	bool m_IsGuarding; // ガード中か
	bool m_IsUltoria;  // ウルトリア中か
	TArray<FBattleStatusData> m_StatusList;
};
