#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RaidBattleTurnManager.generated.h"

class ACharacterBase;

UCLASS()
class ULTORIAPROJECT_API URaidBattleTurnManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize(const TArray<ACharacterBase*>& Characters);
    ACharacterBase* GetCurrentCharacter() const;
    ACharacterBase* AdvanceTurn();
    const TArray<ACharacterBase*>& GetTurnOrder() const { return m_Characters; }

private:
    bool IsAlive(ACharacterBase* Character) const;

    UPROPERTY()
    TArray<TObjectPtr<ACharacterBase>> m_Characters;

    int32 m_CurrentIndex = INDEX_NONE;
};
