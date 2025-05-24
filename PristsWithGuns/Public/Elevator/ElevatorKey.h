// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory System/BaseItem.h"
#include "ElevatorKey.generated.h"

/******************************************************************************
 *                   Elevator Key Item
 *                   
 *   The player can inspect it but there is no UseAction()
 *   since the way keys will work is, when interacting with
 *   the elevator, the elevator checks if player has the key item in
 *   inventory.
 ******************************************************************************/

UCLASS()
class PRISTSWITHGUNS_API UElevatorKey : public UBaseItem 
{
    GENERATED_BODY()

public:
    UElevatorKey();

    virtual void InspectAction() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMesh> InspectMesh;

};