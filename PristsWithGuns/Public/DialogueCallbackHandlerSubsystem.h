// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DialogueCallbackHandlerSubsystem.generated.h"

DECLARE_DYNAMIC_DELEGATE(FCallbackDelegate);

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UDialogueCallbackHandlerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

    virtual void Initialize(FSubsystemCollectionBase &Collection) override;
    virtual void Deinitialize() override;

private:

    TMap<FString, FCallbackDelegate> Supported_Callbacks;

public:

    UFUNCTION(BlueprintCallable, Category = "Callbacks")
    void AddNewSupportedCallback(const FString& CallbackID, UPARAM(DisplayName = "Event") const FCallbackDelegate& Event);
    
    UFUNCTION(BlueprintCallable, Category = "Callbacks")
    void ExecuteFoundCallbacks(TArray<FString> Callbacks);

	
};
