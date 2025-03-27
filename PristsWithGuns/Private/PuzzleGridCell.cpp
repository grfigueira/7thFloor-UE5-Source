// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleGridCell.h"

// Sets default values
APuzzleGridCell::APuzzleGridCell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
        Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
        Mesh->SetWorldScale3D(FVector(0.1f, 0.1f, 0.1f));
        RootComponent = Mesh;
        CellType = EGridCellType::EMPTY; // default value
        GridPosition = FIntPoint(0, 0); // default value

}

void APuzzleGridCell::InitializeCell()
{
    // Create a dynamic material instance for the wall
    if (Mesh)
    {
        if (UMaterialInterface* Material = Mesh->GetMaterial(0))
        {
            DynamicMaterial = UMaterialInstanceDynamic::Create(Material, this);
            Mesh->SetMaterial(0, DynamicMaterial.Get());
            UE_LOG(LogTemp, Warning, TEXT("Got material for wall"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Failed to get material for wall"));
        }
    }
}

void APuzzleGridCell::LerpWallColor(FLinearColor StartColor, FLinearColor EndColor, float Duration)
{
    UE_LOG(LogTemp, Warning, TEXT("Starting LerpWallColor"));

    if (DynamicMaterial.IsValid())
    {
        float* ElapsedTimePtr = new float(0.0f);  // Dynamically allocate elapsed time to track it across lambda calls

        GetWorld()->GetTimerManager().SetTimer(
            ColorLerpHandle,
            [this, StartColor, EndColor, Duration, ElapsedTimePtr]()
            {
                *ElapsedTimePtr += 0.01f;

                float Alpha = FMath::Clamp(*ElapsedTimePtr / Duration, 0.0f, 1.0f);
                FLinearColor CurrentColor = FMath::Lerp(StartColor, EndColor, Alpha);
                
                DynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), CurrentColor);
                
                if (Alpha >= 1.0f)
                {
                    GetWorld()->GetTimerManager().ClearTimer(ColorLerpHandle);
                    delete ElapsedTimePtr;  // Free dynamically allocated memory
                }
            },
            0.01f,
            true
        );
    }
}


// Called when the game starts or when spawned
void APuzzleGridCell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APuzzleGridCell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

