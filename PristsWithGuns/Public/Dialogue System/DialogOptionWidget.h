// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DialogOptionWidget.generated.h"

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UDialogOptionWidget : public UUserWidget
{
	GENERATED_BODY()

public:

    UPROPERTY(meta = (BindWidget))
    class UButton* OptionButton;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* OptionText;
    
    TFunction<void()> ClickedCallback;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Option")
    FText OptionTextValue;

    UFUNCTION(BlueprintCallable)
    void OnOptionButtonClicked();
	
};
