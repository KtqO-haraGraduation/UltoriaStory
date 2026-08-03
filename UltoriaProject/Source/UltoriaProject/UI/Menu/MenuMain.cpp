// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuMain.h"


void UMenuMain::OnEnter()
{
	m_Borders = {
		m_BorderStates, m_BorderItems, m_BorderQuest,
		m_BorderOptions, m_BorderSave, m_BorderEnd
	};

	for (auto border : m_Borders)
	{
		if (border)border->SetVisibility(ESlateVisibility::Hidden);
	}

	m_SelectedIndex = 0;
	UpdateSelection_Menu();
}

void UMenuMain::OnExit()
{

}

void UMenuMain::NavigateUp()
{
	m_SelectedIndex--;
	if (m_SelectedIndex < 0)
	{
		m_SelectedIndex = m_MaxIndex;
	}
	UpdateSelection_Menu();
}

void UMenuMain::NavigateDown()
{
	m_SelectedIndex++;
	if (m_SelectedIndex > m_MaxIndex)
	{
		m_SelectedIndex = 0;
	}
	UpdateSelection_Menu();
}

void UMenuMain::Confirm()
{
	// Œ»Ý‚ÌIndex‚ðState‚ÉCast‚·‚é(+1‚ÍState‚ÌÅ‰‚ªMain‚¾‚©‚ç)
	EMenuState nextState = (EMenuState)(m_SelectedIndex + 1);

	// ŽŸ‚Ìƒƒjƒ…[‚ðŠJ‚­
	UMenuStateManager* menuManger = GetGameInstance()->GetSubsystem<UMenuStateManager>();
	if (menuManger)
	{
		menuManger->OpenMenu(nextState);
	}
}

void UMenuMain::UpdateSelection_Menu()
{
	for (int32 i = 0; i < m_Borders.Num(); i++)
	{
		if (!m_Borders[i])continue;

		m_Borders[i]->SetVisibility(
			i == m_SelectedIndex
			? ESlateVisibility::Visible
			: ESlateVisibility::Hidden
		);
	}
}
