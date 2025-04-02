// Fill out your copyright notice in the Description page of Project Settings.


#include "Tooltip.h"

#include "Components/LightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATooltip::ATooltip()
{
	PrimaryActorTick.bCanEverTick = true;
    
        Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    
        RootComponent = Mesh;
}

void ATooltip::BeginPlay()
{
	Super::BeginPlay();
        InitialZ = GetActorLocation().Z;
}

void ATooltip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
        FVector NewPos = GetActorLocation();
        NewPos.Z = InitialZ + FMath::Sin(GetWorld()->GetTimeSeconds() * BobbingSpeed) * BobbingHeight;
        SetActorLocation(NewPos);

        FRotator NewRotation = Mesh->GetRelativeRotation();
        NewRotation.Yaw += DeltaTime * RotationSpeed;
        Mesh->SetRelativeRotation(NewRotation);

}

void ATooltip::Interact(ACharacter *Character)
{
    UGameplayStatics::PlaySound2D(this, VoiceOver);
}

FText ATooltip::GetInteractionText() const
{
    return FText::FromString("Play\n\"" + NameOfDevComNode + "\"");
}

