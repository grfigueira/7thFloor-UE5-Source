// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "ReceptionBell.generated.h"

class UActorSequenceComponent;
class UFMODAudioComponent;

UCLASS()
class PRISTSWITHGUNS_API AReceptionBell : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AReceptionBell();
    
    UActorSequenceComponent* ButtonPressSequence;

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;


    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent *FmodAudioComponent;

public:
    virtual void Interact(class ACharacter *Character) override;
    virtual FText GetInteractionText() const override;


};