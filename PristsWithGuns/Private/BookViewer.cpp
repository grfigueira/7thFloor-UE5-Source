// Fill out your copyright notice in the Description page of Project Settings.


#include "Books/BookViewer.h"

#include "FMODBlueprintStatics.h"
#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"


void UBookViewer::NextPage()
{
    if (bProcessingPageSwipe || CurrentPageIndex >= PagePairs.Num())
    {
        return;
    }

    bProcessingPageSwipe = true;

    ++CurrentPageIndex;
    UE_LOG(LogTemp, Warning, TEXT("CurrentPageIndex: %d"), CurrentPageIndex);
    
    UpdatePageCountUI();

    // Back cover
    if (CurrentPageIndex >= PagePairs.Num())
    {
        ChangeUIState(BACK_COVER);

        UFMODBlueprintStatics::PlayEvent2D(this, BookCloseSound, true);

        bProcessingPageSwipe = false;
        return;
    }

    // Middle pages
    if (CurrentPageIndex >= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Middle pages"));
        ChangeUIState(MIDDLE);
        UFMODBlueprintStatics::PlayEvent2D(this, PageSwipingSound, true);

        PageLeft->SetText(FText::FromString(PagePairs[CurrentPageIndex].PageLeft));
        PageRight->SetText(FText::FromString(PagePairs[CurrentPageIndex].PageRight));
    }
    bProcessingPageSwipe = false;
}

void UBookViewer::PrevPage()
{
    if (bProcessingPageSwipe || CurrentPageIndex < 0)
    {
        return;
    }

    bProcessingPageSwipe = true;

    --CurrentPageIndex;
    UE_LOG(LogTemp, Warning, TEXT("CurrentPageIndex: %d"), CurrentPageIndex);

    UpdatePageCountUI();

    // Front cover
    if (CurrentPageIndex < 0)
    {
        ChangeUIState(FRONT_COVER);

        UFMODBlueprintStatics::PlayEvent2D(this, BookCloseSound, true);

        bProcessingPageSwipe = false;
        return;
    }

    // Middle pages
    if (CurrentPageIndex >= 0)
    {
        UE_LOG(LogTemp, Warning, TEXT("Middle pages"));
        ChangeUIState(MIDDLE);
        UFMODBlueprintStatics::PlayEvent2D(this, PageSwipingSound, true);

        PageLeft->SetText(FText::FromString(PagePairs[CurrentPageIndex].PageLeft));
        PageRight->SetText(FText::FromString(PagePairs[CurrentPageIndex].PageRight));
    }
    bProcessingPageSwipe = false;
}

void UBookViewer::ChangeUIState(const EBookState NewBookState) const
{
    switch (NewBookState)
    {
    case FRONT_COVER:
        ClosedBookFront->SetRenderOpacity(1.0f);
        ClosedBookBack->SetRenderOpacity(0.0f);
        CoverTitleAuthor->SetRenderOpacity(1.0f);

        OpenedBook->SetRenderOpacity(0.0f);
        PageLeftBorder->SetRenderOpacity(0.0f);
        PageRightBorder->SetRenderOpacity(0.0f);

        PageCountBorder->SetRenderOpacity(0.0f);
        
        break;
    case MIDDLE:
        ClosedBookFront->SetRenderOpacity(0.0f);
        ClosedBookBack->SetRenderOpacity(0.0f);
        CoverTitleAuthor->SetRenderOpacity(0.0f);

        OpenedBook->SetRenderOpacity(1.0f);
        PageLeftBorder->SetRenderOpacity(1.0f);
        PageRightBorder->SetRenderOpacity(1.0f);
        
        PageCountBorder->SetRenderOpacity(1.0f);
        
        break;
    case BACK_COVER:
        ClosedBookFront->SetRenderOpacity(0.0f);
        ClosedBookBack->SetRenderOpacity(1.0f);
        CoverTitleAuthor->SetRenderOpacity(0.0f);

        OpenedBook->SetRenderOpacity(0.0f);
        PageLeftBorder->SetRenderOpacity(0.0f);
        PageRightBorder->SetRenderOpacity(0.0f);
        
        PageCountBorder->SetRenderOpacity(0.0f);
        
        break;
    }
}

void UBookViewer::InitializePagePairs(const TArray<FString> &Pages)
{
    if (CurrentBook)
    {
        CurrentPageIndex = -1;
        ChangeUIState(FRONT_COVER);
        PagePairs.Empty();
        for (int i = 0; i < Pages.Num(); i = i + 2)
        {
            FPagePair NewPagePair;
            NewPagePair.PageLeft = CurrentBook->Pages[i];
            NewPagePair.PageRight = (i + 1 < Pages.Num()) ? CurrentBook->Pages[i + 1] : "";
            PagePairs.Add(NewPagePair);
        }
        UpdatePageCountUI();
    }
}


void UBookViewer::OpenBook(const FDataTableRowHandle &Book)
{
    if (!Book.IsNull())
    {
        CurrentBook = Book.GetRow<FBooksData>("");

        const auto CoverText = FString::Printf(
            TEXT("<Title>%s</> \n <Author>by %s</>"), *CurrentBook->BookName.ToString(),
            *CurrentBook->Author.ToString());

        CoverTitleAuthor->SetText(FText::FromString(CoverText));

        CanvasPanel->SetRenderOpacity(1.0f);

        InitializePagePairs(CurrentBook->Pages);

        // TODO play rise animation
    }
}

void UBookViewer::UpdatePageCountUI() const
{
    PageCount->SetText(FText::FromString(FString::Printf(
            TEXT("%d / %d"), CurrentPageIndex + 1, PagePairs.Num())));
}

void UBookViewer::CloseBook() const
{
    if (CurrentBook)
    {
        CanvasPanel->SetRenderOpacity(0.0f);
    }
}