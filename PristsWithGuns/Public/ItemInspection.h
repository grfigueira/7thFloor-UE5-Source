// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInspection.generated.h"

class AItemInspectionDisplay;
class UCanvasPanel;
class UTextBlock;
/**
 * 
 */

UCLASS()
class PRISTSWITHGUNS_API UItemInspection : public UUserWidget
{
    GENERATED_BODY()

    UItemInspection(const FObjectInitializer& ObjectInitializer);

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UCanvasPanel> CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UTextBlock> ItemName;
    
    TWeakObjectPtr<AItemInspectionDisplay> CurrentItem;
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<AItemInspectionDisplay> CurrentItemClass; 

public:
    UFUNCTION(BlueprintCallable)
    void SetItemToDisplay(const TSubclassOf<AItemInspectionDisplay>& ItemClass, const FText& Name);
    
    UFUNCTION(BlueprintCallable)
    void SetIsRotating(const bool bNewIsRotating) const;


};