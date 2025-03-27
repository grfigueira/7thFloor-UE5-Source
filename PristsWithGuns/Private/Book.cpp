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

void ABook::Interact(ACharacter *Character)
{

    if(APristsWithGunsCharacter *ActualCharacter = Cast<APristsWithGunsCharacter>(Character))
    {
        const TUniquePtr<UBookViewer> BookWidget = ActualCharacter->GetBookWidgetOwnership();
        BookWidget->OpenBook(BookDataHandle);
    }
}

FText ABook::GetInteractionText() const
{
    return FText::FromString("Read book");
}

