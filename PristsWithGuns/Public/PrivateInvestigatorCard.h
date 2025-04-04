// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Inventory System/BaseItem.h"
#include "PrivateInvestigatorCard.generated.h"

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UPrivateInvestigatorCard : public UBaseItem
{
	GENERATED_BODY()

public:

    virtual void InspectAction() override;

    UPROPERTY(EditAnywhere)
    TObjectPtr<UStaticMesh> InspectMesh;
    
	
};
