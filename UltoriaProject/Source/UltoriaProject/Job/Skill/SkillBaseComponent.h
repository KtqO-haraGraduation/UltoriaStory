// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Battle/Combat/Components/ComponentBase.h"
#include "SkillDataRow.h"
#include "SkillBaseComponent.generated.h"

class ACharacterBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ULTORIAPROJECT_API USkillBaseComponent : public UComponentBase
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillBaseComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	void Create(FName name);
	void Remove(int index);
	void Reset();

	bool CanUse(int index);
	void UseSkill(int index, const TArray<ACharacterBase*>& targets);

	bool IsUnlocked(int index);

	const FSkillDataRow* GetSkill(int index) const;
	const TArray<FSkillDataRow>& GetSkills() const { return m_SkillList; }
	TArray<FSkillDataRow> GetUnlockedSkills();

private:
	// äeÉXÉLÉãä÷êî
	void Attack(const TArray<ACharacterBase*>& targets);
	void Buff(const TArray<ACharacterBase*>& targets);
	void Heal(const TArray<ACharacterBase*>& targets);

private:
	TArray<FSkillDataRow> m_SkillList;


	const FSkillDataRow* m_NowUseSkillData;
};
