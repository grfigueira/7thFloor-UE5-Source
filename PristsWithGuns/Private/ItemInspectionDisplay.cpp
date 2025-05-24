// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInspectionDisplay.h"

#include "MathUtil.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AItemInspectionDisplay::AItemInspectionDisplay() : LastMousePos(FVector2D::ZeroVector)
{
    PrimaryActorTick.bCanEverTick = true;

    Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
    RootComponent = Root;

    Pivot = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot"));
    Pivot->SetupAttachment(Root);

    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    Mesh->SetupAttachment(Pivot);

    SceneCapture = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCapture"));
    SceneCapture->SetupAttachment(Root);

}

void AItemInspectionDisplay::SetIsRotating(const bool bValue)
{
    bIsRotating = bValue;
    if (bIsRotating)
    {
        if (const APlayerController *PC = UGameplayStatics::GetPlayerController(this, 0))
        {
            PC->GetMousePosition(LastMousePos.X, LastMousePos.Y);
        }
    }
}

void AItemInspectionDisplay::BeginPlay()
{
    Super::BeginPlay();
    SceneCapture->ShowOnlyActorComponents(this);

}

void AItemInspectionDisplay::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bIsRotating)
        return;

    const APlayerController *PC = UGameplayStatics::GetPlayerController(this, 0);
    if (!PC)
        return;

    float DeltaX, DeltaY;
    PC->GetInputMouseDelta(DeltaX, DeltaY);

    const FVector CameraToPivot = Pivot->GetRelativeTransform().GetLocation() - SceneCapture->GetRelativeTransform().
                                  GetLocation();
    
    const FVector CameraToPivotPerpendicular = FVector(-CameraToPivot.Y, CameraToPivot.X, CameraToPivot.Z).GetSafeNormal();

    
    if (CameraToPivotPerpendicular.IsZero())
        return;
    
    const FQuat VerticalRotationQuaternion = FQuat(CameraToPivotPerpendicular, (DeltaY * RotationSpeed) * (FMathf::Pi / 180));

    const FRotator DeltaRotation(0.0f,
                                 DeltaX * RotationSpeed,
                                 0.0f);

    
    Pivot->AddLocalRotation(DeltaRotation);

    const float AngleZtoCamera = FVector::DotProduct(CameraToPivot.GetSafeNormal(), -Pivot->GetUpVector().GetSafeNormal());

    if((AngleZtoCamera > -0.98 || DeltaY > 0) && (AngleZtoCamera < 0.98 || DeltaY < 0))
    {
        Pivot->SetRelativeRotation(VerticalRotationQuaternion * Pivot->GetRelativeTransform().GetRotation());
    }
    

}