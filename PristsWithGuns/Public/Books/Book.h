// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IInteractable.h"
#include "GameFramework/Actor.h"
#include "Book.generated.h"

class UFMODAudioComponent;

UCLASS()
class PRISTSWITHGUNS_API ABook : public AActor, public IInteractable
{
    GENERATED_BODY()

public:
    ABook();

    UPROPERTY(EditAnywhere)
    FString ReadableType = "Book";

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Book")
    FDataTableRowHandle BookDataHandle;

protected:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMeshComponent> Mesh;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Sound")
    UFMODAudioComponent *FmodAudioComponent;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

    virtual void Interact(class ACharacter *Character) override;
    virtual FText GetInteractionText() const override;

};