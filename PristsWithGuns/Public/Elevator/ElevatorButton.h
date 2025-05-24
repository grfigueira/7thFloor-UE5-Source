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

    UPROPERTY(EditAnywhere, Category="Button")
    TObjectPtr<UCurveFloat> ColorShiftCurve;
    
    UPROPERTY(EditAnywhere, Category="Button")
    int16 FloorNumber; // sure, could act like an id for a key

    UPROPERTY(EditAnywhere, Category="Button")
    bool bIsUnlocked;
    
    virtual void BeginPlay() override;

public:
    virtual void Interact(class ACharacter *Character) override;

};