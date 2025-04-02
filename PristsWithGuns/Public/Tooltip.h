// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IInteractable.h"
#include "GameFramework/Actor.h"
#include "Tooltip.generated.h"

class USpotLightComponent;

UCLASS()
class PRISTSWITHGUNS_API ATooltip : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	ATooltip();

protected:
	virtual void BeginPlay() override;

        UPROPERTY(EditAnywhere, Category="Tooltip")
        TObjectPtr<UStaticMeshComponent> Mesh;

        UPROPERTY(EditAnywhere, Category="Tooltip|Developer Commentary")
        FString NameOfDevComNode;
    
        UPROPERTY(EditAnywhere, Category="Tooltip")
        TObjectPtr<USoundWave> VoiceOver;


public:	
	virtual void Tick(float DeltaTime) override;

        UPROPERTY(EditAnywhere, Category="Tooltip|Animation")
        float BobbingHeight = 1.0f;
        
        UPROPERTY(EditAnywhere, Category="Tooltip|Animation")
        float BobbingSpeed = 1.0f;
    
        UPROPERTY(EditAnywhere, Category="Tooltip|Animation")
        float RotationSpeed = 1.0f;

        virtual void Interact(class ACharacter *Character) override;
        virtual FText GetInteractionText() const override;
private:
    float InitialZ;

};
