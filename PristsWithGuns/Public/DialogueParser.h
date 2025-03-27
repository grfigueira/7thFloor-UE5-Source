// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DialogueParser.generated.h"

USTRUCT(BlueprintType)
struct FDialogueNode
{

    GENERATED_BODY()
    
public:
    
    UPROPERTY()
    int ID;
    
    UPROPERTY()
    int Next_Node_ID;
    
    UPROPERTY()
    int Node_Type;
    
    UPROPERTY()
    TArray<int> Responses;
    
    UPROPERTY()
    TArray<FString> Selected_Callbacks;
    
    UPROPERTY()
    FString Text;
};

/**
 * 
 */
class PRISTSWITHGUNS_API FDialogueParser
{
public:
    FDialogueParser();
    ~FDialogueParser() = default;

    static TMap<int32, FDialogueNode> ParseDialogueJson(const FString &JsonString, bool &Success);
    static TMap<int32, FDialogueNode> LoadDialogueFromFile(const FString &FilePath, bool &Success);
};