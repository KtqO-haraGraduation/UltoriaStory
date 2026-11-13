#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RaidBattleTypes.h"
#include "RaidBattleCommandProxy.generated.h"

class ARaidBattleManager;
class ACharacterBase;
class APlayerController;

UCLASS()
class ULTORIAPROJECT_API ARaidBattleCommandProxy : public AActor
{
    GENERATED_BODY()

public:
    ARaidBattleCommandProxy();

    void Initialize(ARaidBattleManager* Manager, APlayerController* OwningController);

    UFUNCTION(BlueprintPure, Category = "Raid")
    ARaidBattleManager* GetRaidManager() const { return m_RaidManager; }

    UFUNCTION(BlueprintPure, Category = "Raid")
    APlayerController* GetOwningController() const { return Cast<APlayerController>(GetOwner()); }

    UFUNCTION(BlueprintCallable, Category = "Raid")
    void SubmitCommand(ERaidCommandType Command, ACharacterBase* Target = nullptr);

protected:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    UFUNCTION(Server, Reliable)
    void ServerSubmitCommand(ERaidCommandType Command, ACharacterBase* Target);

private:
    UPROPERTY(Replicated)
    TObjectPtr<ARaidBattleManager> m_RaidManager;
};
