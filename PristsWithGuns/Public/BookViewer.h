// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BookViewer.generated.h"

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UBookViewer : public UUserWidget
{
    GENERATED_BODY()

private:
    int CurrentPageIndex;

public:
    void NextPage();
    void PrevPage();
    void LoadPage(int Index);
    void OpenBook(FDataTableRowHandle &Book); // Plays rise animation and darkens background
    void CloseBook();
};