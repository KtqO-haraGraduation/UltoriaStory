// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
#include "EnemyBase.h"

void UEnemyManager::AddEnemy(AEnemyBase* enemy)
{
	if (!IsValid(enemy)) return;
	if (m_Enemies.Contains(enemy)) return;
	m_Enemies.Add(enemy);
}

void UEnemyManager::RemoveEnemy(AEnemyBase* enemy)
{
	if (!IsValid(enemy)) return;
	if (!m_Enemies.Contains(enemy)) return;
	m_Enemies.Remove(enemy);
}
