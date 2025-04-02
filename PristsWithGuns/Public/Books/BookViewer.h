// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BooksData.h"
#include "Blueprint/UserWidget.h"
#include "FMODAudioComponent.h"
#include "BookViewer.generated.h"


class UVerticalBox;
class UTextBlock;

enum EBookState 
{
    FRONT_COVER,
    MIDDLE,
    BACK_COVER
};

struct FPagePair
{
    FString PageLeft;
    FString PageRight;
};

/**
 * 
 */
UCLASS()
class PRISTSWITHGUNS_API UBookViewer : public UUserWidget
{
    GENERATED_BODY()

private:
    int CurrentPageIndex = -1;
    bool bProcessingPageSwipe = false; // used to avoid race conditions
    FBooksData* CurrentBook;
    TArray<FPagePair> PagePairs;

public:

    UPROPERTY(meta = (BindWidget))
    class URichTextBlock* CoverTitleAuthor;

    UPROPERTY(meta = (BindWidget))
    class UImage* ClosedBookFront;
    
    UPROPERTY(meta = (BindWidget))
    UImage* ClosedBookBack;

    UPROPERTY(meta = (BindWidget))
    UImage* OpenedBook;
    
    UPROPERTY(meta = (BindWidget))
    URichTextBlock* PageLeft;
    
    UPROPERTY(meta = (BindWidget))
    URichTextBlock* PageRight;
    
    UPROPERTY(meta = (BindWidget))
    UTextBlock* PageCount;

    UPROPERTY(meta = (BindWidget))
    class UCanvasPanel* CanvasPanel;

    UPROPERTY(meta = (BindWidget))
    class UBorder* TitleBorder;
    
    UPROPERTY(meta = (BindWidget))
    UBorder* PageLeftBorder;

    UPROPERTY(meta = (BindWidget))
    UBorder* PageRightBorder;

    UPROPERTY(meta = (BindWidget))
    UVerticalBox* PageCountBorder;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
    UFmodEvent* PageSwipingSound;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
    UFmodEvent* BookCloseSound;
    
    void NextPage();
    void PrevPage();
    void ChangeUIState(EBookState NewBookState) const;
    void InitializePagePairs(const TArray<FString>& Pages);
    void OpenBook(const FDataTableRowHandle &Book); // Plays rise animation and darkens background
    void UpdatePageCountUI() const;
    void CloseBook() const;
};