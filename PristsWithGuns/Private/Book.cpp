// Fill out your copyright notice in the Description page of Project Settings.


#include "Book.h"

#include "BookViewer.h"
#include "Blueprint/UserWidget.h"
#include "PristsWithGuns/PristsWithGunsCharacter.h"

// Sets default values
ABook::ABook()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
        Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BookMesh"));
}

// Called when the game starts or when spawned
void ABook::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABook::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/*void ABook::Interact(ACharacter *Character)
{

    if(APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character))
    {
        TObjectPtr<UBookViewer> BookViewer;
        if(ActualCharacter->TryAcquireBookWidgetOwnership(BookViewer))
        {
            ActualCharacter->EnterBookContext();
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("NEW CODE RUNNING"));
            if(BookViewer)
            {
                BookViewer->OpenBook(BookDataHandle);
            }
        }
        else
        {
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Couldn't acquire book widget"));
        }
    }
}
*/
void ABook::Interact(ACharacter *Character) {
    // Verify Character is valid
    if (!Character) {
        UE_LOG(LogTemp, Warning, TEXT("ABook::Interact - Called with null Character"));
        return;
    }
    
    // Verify cast to game character
    APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character);
    if (!ActualCharacter) {
        UE_LOG(LogTemp, Warning, TEXT("ABook::Interact - Failed to cast Character to APristsWithGunsCharacter"));
        return;
    }
    
    // Verify BookDataHandle is valid before using it
    if (BookDataHandle.IsNull()) {
        UE_LOG(LogTemp, Error, TEXT("ABook::Interact - BookDataHandle is invalid for book: %s"), *GetName());
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Book data is missing or invalid"));
        return;
    }
    
    // Create a null BookViewer
    TObjectPtr<UBookViewer> BookViewer = nullptr;
    
    // Try to acquire book widget with null check on return
    bool bAcquiredWidget = false;
    if (IsValid(ActualCharacter)) {
        bAcquiredWidget = ActualCharacter->TryAcquireBookWidgetOwnership(BookViewer);
    }
    
    if (!bAcquiredWidget || !BookViewer) {
        UE_LOG(LogTemp, Warning, TEXT("ABook::Interact - Couldn't acquire book widget"));
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Couldn't acquire book widget"));
        return;
    }
    
    // Now that we have a valid BookViewer, enter book context
    ActualCharacter->EnterBookContext();
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("NEW CODE RUNNING"));
    
    // Final check before calling OpenBook
    if (IsValid(BookViewer)) {
        BookViewer->OpenBook(BookDataHandle);
    } else {
        UE_LOG(LogTemp, Error, TEXT("ABook::Interact - BookViewer became invalid after acquisition"));
    }
}
FText ABook::GetInteractionText() const
{
    return FText::FromString("Read book");
}

