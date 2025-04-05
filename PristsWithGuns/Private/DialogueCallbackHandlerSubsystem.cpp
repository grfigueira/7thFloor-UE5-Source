// Fill out your copyright notice in the Description page of Project Settings.


#include "DialogueCallbackHandlerSubsystem.h"

void UDialogueCallbackHandlerSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UDialogueCallbackHandlerSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UDialogueCallbackHandlerSubsystem::AddNewSupportedCallback(const FString &CallbackID,
    const FCallbackDelegate &Event)
{
    Supported_Callbacks.Add(CallbackID, Event);
}

void UDialogueCallbackHandlerSubsystem::ExecuteFoundCallbacks(TArray<FString> Callbacks)
{
    for(FString& Callback : Callbacks)
    {
        if(Supported_Callbacks.Contains(Callback))
        {
            Supported_Callbacks[Callback].Execute();
        }  
    }
}

