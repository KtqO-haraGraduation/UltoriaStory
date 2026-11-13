#include "RaidBattleTurnManager.h"
#include "../../Character/CharacterBase.h"
#include "../Combat/Components/CombatComponent.h"

void URaidBattleTurnManager::Initialize(const TArray<ACharacterBase*>& Characters)
{
    m_Characters.Reset();
    for (ACharacterBase* Character : Characters)
    {
        if (IsValid(Character))
        {
            m_Characters.Add(Character);
        }
    }

    m_Characters.Sort([](const TObjectPtr<ACharacterBase>& A, const TObjectPtr<ACharacterBase>& B)
    {
        if (!A) return false;
        if (!B) return true;
        return A->GetBaseStatus().m_SPD > B->GetBaseStatus().m_SPD;
    });

    m_CurrentIndex = INDEX_NONE;
}

ACharacterBase* URaidBattleTurnManager::GetCurrentCharacter() const
{
    if (!m_Characters.IsValidIndex(m_CurrentIndex)) return nullptr;
    return m_Characters[m_CurrentIndex];
}

ACharacterBase* URaidBattleTurnManager::AdvanceTurn()
{
    if (m_Characters.IsEmpty()) return nullptr;

    for (int32 Count = 0; Count < m_Characters.Num(); ++Count)
    {
        m_CurrentIndex = (m_CurrentIndex + 1) % m_Characters.Num();
        ACharacterBase* Candidate = m_Characters[m_CurrentIndex];
        if (IsAlive(Candidate))
        {
            return Candidate;
        }
    }

    return nullptr;
}

bool URaidBattleTurnManager::IsAlive(ACharacterBase* Character) const
{
    if (!IsValid(Character) || Character->GetCurrentHP() <= 0) return false;

    UCombatComponent* Combat = Character->GetComponent<UCombatComponent>();
    return !Combat || Combat->GetCurrentState() != CombatState::DEAD;
}
