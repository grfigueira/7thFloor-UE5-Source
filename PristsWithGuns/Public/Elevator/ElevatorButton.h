// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ButtonInteractive.h"
#include "ElevatorButton.generated.h"

class UActorSequenceComponent;
struct FTimeline;
/**
 *  TODO This shouldn't be an ElevatorButton
 *  TODO instead it should be abstracted to ButtonLockable
 */
UCLASS()
class PRISTSWITHGUNS_API AElevatorButton : public AButtonInteractive
{
    GENERATED_BODY()

public:
    AElevatorButton();

protected:
    UPROPERTY(EditAnywhere, Category="Button")
    TObjectPtr<class UFmodEvent> ErrorSound;
    
    UPROPERTY(EditAnywhere, Category="Button")
    TObjectPtr<UFmodEvent> SucceessSound;

    UPROPERTY(EditAnywhere, Category="Button|Materials")
    TObjectPtr<UMaterialInstance> ErrorMaterial;
    
    UPROPERTY(EditAnywhere, Category="Button")
    int16 ButtonKeyId;

    UPROPERTY(EditAnywhere, Category="Button")
    bool bIsUnlocked;

    UFUNCTION(BlueprintImplementableEvent)
    void TryToUseLockedButton();


    virtual void BeginPlay() override;

public:
    virtual void Interact(class ACharacter *Character) override;

    UFUNCTION(BlueprintCallable)
    bool IsButtonLocked() const { return !bIsUnlocked; }

    // Unlocks button if Id corresponds
    UFUNCTION()
    bool UseKey(int16 keyId);
    
    UFUNCTION(BlueprintCallable)
    void LockButton();
    
    UFUNCTION(BlueprintCallable)
    void UnlockButton();

};