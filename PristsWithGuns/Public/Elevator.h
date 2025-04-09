// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Elevator.generated.h"

UCLASS()
class PRISTSWITHGUNS_API AElevator : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AElevator();

    UFUNCTION(BlueprintCallable)
    void OpenDoors();

    UPROPERTY(EditAnywhere)
    float DoorSpeed = 2.0f; // Speed for opening/closing the doors
    
    UPROPERTY(EditAnywhere)
    float DoorOpenedDistance = 60.0f;

protected:
    virtual void BeginPlay() override;
    virtual void Tick(float DeltaSeconds) override;
    void OpenElevatorDoor(const TObjectPtr<UStaticMeshComponent>& Door, const FVector& TargetPos, bool& IsOpeningDoors, float DeltaTime);

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Elevator")
    TObjectPtr<UStaticMeshComponent> DoorLeft;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Elevator")
    TObjectPtr<UStaticMeshComponent> DoorRight;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Elevator")
    TObjectPtr<UStaticMeshComponent> Frame;

private:
    bool bOpeningDoors;
    FVector LeftTargetPos;
    FVector RightTargetPos;


};