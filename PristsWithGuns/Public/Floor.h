// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floor.generated.h"

class APointLight;

UCLASS()
class PRISTSWITHGUNS_API AFloor : public AActor
{
    GENERATED_BODY()

private:
    UPROPERTY()
    TArray<TObjectPtr<APointLight>> AttachedLights;

    UFUNCTION()
void InitializeAttachedLightsArray();

public:
    AFloor();

    UFUNCTION(BlueprintCallable)
    void BringToElevator();

    UFUNCTION(BlueprintCallable)
    void MoveAway();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void SetIsActive(bool bValue);
    
    UFUNCTION(BlueprintCallable)
    void SetPointLightsEnabled(bool bNewEnabled);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int FloorNumber;

    UPROPERTY(EditDefaultsOnly)
    float ZWhenAway = -2000;

    FVector LocActive;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bIsActive = false;


};