// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuBase.h"
#include "Components/Overlay.h"
#include "StatusTabDataAsset.h"
#include "MenuStates.generated.h"

UENUM(BlueprintType)
enum class EStatusTab : uint8
{
	Strength,
	Equip,
	Job
};


UCLASS()
class ULTORIAPROJECT_API UMenuStates : public UMenuBase
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct()override;

	void OnEnter()override;
	void OnExit()override;

	void OnLButton()override;
	void OnRButton()override;
	void OnL2Button()override;
	void OnR2Button()override;

private:
	TSubclassOf<UMenuBase> GetTabClass(EStatusTab tab);

	void UpdateTab();

private:
	TMap<EStatusTab, TSubclassOf<UMenuBase>> m_TabClassMap;
	TMap<EStatusTab, UMenuBase*> m_TabMap;
	UMenuBase* m_CurrentTabWidget;

	UPROPERTY(EditAnywhere, Category = "Status")
	UStatusTabDataAsset* m_TabDataAsset;

	UPROPERTY(meta = (BindWidget))
	class UOverlay* m_TabContentArea;

	EStatusTab m_CurrentTab;
	int32 m_CurrentCharacterIndex;
};

