// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_DELEGATE_TwoParams(FOnSelectionChanged, int, int);

/**
 * 
 */
template<typename T>
class TSelectionSystem
{
public:
	void SetTargets(const TArray<T>& targets)
	{
		// 前回選択したターゲットを取得
		T current = GetCurrent();
		int oldIndex = GetCurrentIndex();
		// 新しいターゲット群を設定
		m_Targets = targets;

		// 現在選択してるターゲットの配列番号を検索
		int index = m_Targets.Find(current);

		// インデックス更新
		m_CurrentIndex = (index != INDEX_NONE) ? index : 0;

		m_OnSelectionChanged.ExecuteIfBound(
			oldIndex,
			m_CurrentIndex);
	}

	void Move(int delta)
	{
		if (m_Targets.Num() == 0) return;

		int oldIndex = m_CurrentIndex;

		m_CurrentIndex += delta;

		if (m_CurrentIndex >= m_Targets.Num())
		{
			m_CurrentIndex = 0;
		}
		else if (m_CurrentIndex < 0)
		{
			m_CurrentIndex = m_Targets.Num() - 1;
		}

		m_OnSelectionChanged.ExecuteIfBound(oldIndex, m_CurrentIndex);
	}

	const TArray<T>& GetTargets() const
	{
		return m_Targets;
	}

	T GetCurrent() const
	{
		if (m_Targets.Num() == 0)
		{
			return T{};
		}

		return m_Targets[m_CurrentIndex];
	}

	int GetCurrentIndex() const
	{
		return m_CurrentIndex;
	}

	void SetCurrentIndex(int index)
	{
		m_CurrentIndex = index;
	}

public:
	FOnSelectionChanged m_OnSelectionChanged;

private:
	int m_CurrentIndex  = 0;
	TArray<T> m_Targets;
};
