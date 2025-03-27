// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ObservationDataChild.h"
#include "Engine/DataAsset.h"
#include "ObservationData.generated.h"


class UObservationData_OLD;


/******************************************************************************
*     Speech root: DataAsset wrapper
******************************************************************************/
UCLASS()
class PRISTSWITHGUNS_API UObservationData_OLD : public UDataAsset
{
	GENERATED_BODY()

public:
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    TObjectPtr<UObservationDataChild> RootObservation = nullptr;
	
};
