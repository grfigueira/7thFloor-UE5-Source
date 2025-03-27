// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObservationDataChild.generated.h"


class UObservationDataChild;

#if 0

USTRUCT(BlueprintType)
struct FResponseData
{

    GENERATED_BODY()
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Response")
    FString ResponseOption;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Response")
    FString CallbackTag; // optional tag if the response has a special callback

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Response")
    TObjectPtr<UObservationDataChild> NextObservation = nullptr; // after the answer (and eventual callback), return to the conversation
};

USTRUCT(BlueprintType)
struct FObservationData
{
    GENERATED_BODY()
    // The speed at which the observation is "spoken"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    float ObservationSpeed = 1.f;

    // The text to display for the observation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    FString ObservationText = TEXT("This is interesting.");

    // A list of responses available for this observation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    TArray<FResponseData> ResponseOptions;

    //UPROPERTY(EditAnywhere, BlueprintReadWrite)
    //TSharedPtr<FObservationData> NextObservation;
};

#endif

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UObservationDataChild : public UObject
{
    GENERATED_BODY()

public:
#if 0
    // The speed at which the observation is "spoken"
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    float ObservationSpeed = 1.f;

    // The text to display for the observation
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    FString ObservationText = TEXT("This is interesting.");

    // A list of responses available for this observation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    TArray<FResponseData> ResponseOptions;

    // If there are no response options (or as a fallback), this pointer lets you chain to the next observation.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Observation")
    TObjectPtr<UObservationDataChild> NextObservation = nullptr;
#endif
};