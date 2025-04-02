// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/IInteractable.h"
#include "GameFramework/Actor.h"
#include "ButtonInteractive.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnButtonPressed);

UCLASS()
class PRISTSWITHGUNS_API AButtonInteractive : public AActor, public IInteractable
{
	GENERATED_BODY()
	
public:	
	AButtonInteractive();

    
        UPROPERTY(EditAnywhere, Category="Button|Materials")
        TObjectPtr<UMaterialInstance> NeutralMaterial;
    
        UPROPERTY(EditAnywhere, Category="Button|Materials")
        TObjectPtr<UMaterialInstance> PressedMaterial;

        UPROPERTY(EditAnywhere, Category="Button")
        TObjectPtr<UAnimationAsset> ButtonPressAnim;
    
        UPROPERTY(BlueprintAssignable, Category = "Button")
        FOnButtonPressed OnButtonPressed;


protected:
	virtual void BeginPlay() override;
    
        UPROPERTY(EditAnywhere)
        TObjectPtr<USkeletalMeshComponent> ButtonMeshComponent;


public:	
	virtual void Tick(float DeltaTime) override;
    
        virtual void Interact(class ACharacter *Character) override;
        virtual FText GetInteractionText() const override;

};
