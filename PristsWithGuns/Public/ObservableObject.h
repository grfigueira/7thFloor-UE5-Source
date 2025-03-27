// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "ObservableObject.generated.h"

class UCrosshairWidget;
class UFMODAudioComponent;

UCLASS()
class PRISTSWITHGUNS_API AObservableObject : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    AObservableObject();

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, Category="Observation")
    FString Observation;

    UPROPERTY(EditAnywhere, Category="Observation")
    float Speed = 70;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Observation")
    TObjectPtr<UStaticMeshComponent> Mesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent *FmodAudioComponent; // typing sound


public:
    virtual void Interact(class ACharacter *Character) override;
    virtual FText GetInteractionText() const override;

    virtual void Tick(float DeltaTime) override;

private:
    UCrosshairWidget *CrosshairWidget;
    bool bIsObsTyping;
    float DelayAccumulator;
    int32 CurrentStringIndex;
    FString TempObsString;


};