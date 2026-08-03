// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuStates.h"


void UMenuStates::NativeConstruct()
{
	Super::NativeConstruct();

	if (m_TabDataAsset)
	{
		m_TabClassMap.Add(EStatusTab::Strength, m_TabDataAsset->m_StrengthTabClass);
		m_TabClassMap.Add(EStatusTab::Equip, m_TabDataAsset->m_EquipTabClass);
		m_TabClassMap.Add(EStatusTab::Job, m_TabDataAsset->m_JobTabClass);
	}
}

void UMenuStates::OnEnter()
{
	// 最初のタブを強さにしてタブを更新
	m_CurrentTab = EStatusTab::Strength;
	m_CurrentCharacterIndex = 0;
	UpdateTab();
}

void UMenuStates::OnExit()
{

}

void UMenuStates::OnLButton()
{
	// タブの列挙体をint型にCastしてIndexを取得
	int32 tabIndex = (int32)m_CurrentTab - 1;
	// Jobにループ
	if (tabIndex < 0) tabIndex = 2;
	// インデックスを列挙型にまた戻す
	m_CurrentTab = (EStatusTab)tabIndex;
	// タブを更新
	UpdateTab();
}

void UMenuStates::OnRButton()
{
	// タブの列挙体をint型にCastしてIndexを取得
	int32 tabIndex = (int32)m_CurrentTab + 1;
	// Strengthにループ
	if (tabIndex > 2)tabIndex = 0;
	// インデックスを列挙型にまた戻す
	m_CurrentTab = (EStatusTab)tabIndex;
}

void UMenuStates::OnL2Button()
{

}

void UMenuStates::OnR2Button()
{

}

TSubclassOf<UMenuBase> UMenuStates::GetTabClass(EStatusTab tab)
{
	TSubclassOf<UMenuBase>* tabClass = m_TabClassMap.Find(tab);
	return tabClass ? *tabClass : nullptr;
}

void UMenuStates::UpdateTab()
{
	// 全タブを非表示
	for (auto& pair : m_TabMap)
	{
		if (pair.Value)pair.Value->Hide();
	}

	// 現在のタブがまだ生成されていなければ
	if (!m_TabMap.Contains(m_CurrentTab))
	{
		// 現在のタブのクラスを検索して取得
		TSubclassOf<UMenuBase>* tabClass = m_TabClassMap.Find(m_CurrentTab);
		if (tabClass && *tabClass)
		{
			// 現在のタブを生成
			UMenuBase* newTab = CreateWidget<UMenuBase>(GetOwningPlayer(), *tabClass);
			if (newTab)
			{
				// コンテナに追加
				m_TabMap.Add(m_CurrentTab, newTab);
			}
		}
	}

	// 現在のタブを表示
	UMenuBase** currentTab = m_TabMap.Find(m_CurrentTab);
	if (currentTab && *currentTab)
	{
		m_TabContentArea->AddChildToOverlay(*currentTab);
		(*currentTab)->Show();
		m_CurrentTabWidget = *currentTab;
	}
}
