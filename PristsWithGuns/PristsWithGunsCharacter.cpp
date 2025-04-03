// Copyright Epic Games, Inc. All Rights Reserved.

#include "PristsWithGunsCharacter.h"

#include "CrosshairWidget.h"
#include "Inventory System/InventoryWidget.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Interfaces/IInteractable.h"
#include "InputActionValue.h"
#include "Dialogue System/ObservableObjectComponent.h"
#include "Pause Menu/PauseMenu.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GridPuzzles/PuzzleGrid.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Shakes/LegacyCameraShake.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// APristsWithGunsCharacter

APristsWithGunsCharacter::APristsWithGunsCharacter()
{

    PrimaryActorTick.bCanEverTick = true;

    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

    // Create a CameraComponent	
    FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
    FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
    FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
    Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
    Mesh1P->SetOnlyOwnerSee(true);
    Mesh1P->SetupAttachment(FirstPersonCameraComponent);
    Mesh1P->bCastDynamicShadow = false;
    Mesh1P->CastShadow = false;
    Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

    // Create an FMOD Component
    FmodAudioComponent = CreateDefaultSubobject<UFMODAudioComponent>(TEXT("Footsteps Audio Component"));
    FmodAudioComponent->SetupAttachment(GetMesh());
    FmodAudioComponent->SetAutoActivate(false);

    InteractableActorInRange = nullptr;
    InteractableInRange = nullptr;

}


//////////////////////////////////////////////////////////////////////////// Input

bool APristsWithGunsCharacter::TryAcquireBookWidgetOwnership(TObjectPtr<UBookViewer> &OutBookViewer)
{
    if (bIsBookBeingUsed)
    {
        return false;
    }

    bIsBookBeingUsed = true;
    OutBookViewer = BookWidget;
    return true;

}

void APristsWithGunsCharacter::ReleaseBookWidget()
{
    bIsBookBeingUsed = false;
}

void APristsWithGunsCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}


void APristsWithGunsCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::NextObservation);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APristsWithGunsCharacter::Move);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::StartedMoving);
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this,
                                           &APristsWithGunsCharacter::SetMovingFalse);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APristsWithGunsCharacter::Look);

        // Interacting
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::Interact);

        // Pause Game
        EnhancedInputComponent->BindAction(PauseGameAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::PauseGame);

        /////// Puzzle /////////

        EnhancedInputComponent->BindAction(PuzzleMoveAction, ETriggerEvent::Triggered, this,
                                           &APristsWithGunsCharacter::MovePuzzle);
        EnhancedInputComponent->BindAction(PuzzleMoveAction, ETriggerEvent::Completed, this,
                                           &APristsWithGunsCharacter::ResetPuzzleCanMove);

        /////// Book /////////

        EnhancedInputComponent->BindAction(BookLeftSwipe, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::BookPrevPage);

        EnhancedInputComponent->BindAction(BookRightSwipe, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::BookNextPage);

        EnhancedInputComponent->BindAction(CloseBook, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::ExitBookContext);

        /////// Inventory /////////
        EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::OpenInventory);

        EnhancedInputComponent->BindAction(CloseInventoryAction, ETriggerEvent::Started, this,
                                           &APristsWithGunsCharacter::CloseInventory);

    }
    else
    {
        UE_LOG(LogTemplateCharacter, Error,
               TEXT(
                   "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
               ), *GetNameSafe(this));
    }
}


void APristsWithGunsCharacter::BeginPlay()
{
    Super::BeginPlay();

    if (InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UInventoryWidget>(GetWorld(), InventoryWidgetClass);
    }

    if (CrosshairWidgetClass)
    {
        CrosshairWidget = CreateWidget<UCrosshairWidget>(GetWorld(), CrosshairWidgetClass);
        if (CrosshairWidget)
        {
            CrosshairWidget->AddToViewport();
        }
    }

    if (BookViewerWidgetClass)
    {
        BookWidget = CreateWidget<UBookViewer>(GetWorld(), BookViewerWidgetClass);
    }

    if (PauseMenuWidgetClass)
    {
        PauseMenuWidget = CreateWidget<UPauseMenu>(GetWorld(), PauseMenuWidgetClass);
    }
}


void APristsWithGunsCharacter::Move(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        if (!MovementVector.IsNearlyZero())
        {
            MovementVector.Normalize();

            if (PlayerController)
            {
                PlayerController->ClientStartCameraShake(CameraShake);
            }

        }

        // add movement 
        AddMovementInput(GetActorForwardVector(), MovementVector.Y * WalkSpeed / 600.0f);
        AddMovementInput(GetActorRightVector(), MovementVector.X * WalkSpeed / 600.0f);
    }
}

void APristsWithGunsCharacter::OpenInventory()
{

    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->SetShowMouseCursor(true);

        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);

        if (InventoryWidget)
        {
            InventoryWidget->AddToViewport();
        }

        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(DefaultMappingContext);
            Subsystem->AddMappingContext(InventoryMappingContext, 0);
        }
    }
}

void APristsWithGunsCharacter::CloseInventory()
{
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                     TEXT("Ivoked CloseInventory"));
    if (InventoryWidget)
    {
        if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->SetShowMouseCursor(false);
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);

            if (InventoryWidget)
            {
                InventoryWidget->RemoveFromParent();
            }
            if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
                UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
            {
                Subsystem->RemoveMappingContext(InventoryMappingContext);
                Subsystem->AddMappingContext(DefaultMappingContext, 0);
            }
        }

    }
}

void APristsWithGunsCharacter::StartedMoving(const FInputActionValue &Value)
{
    FVector2D MovementVector = Value.Get<FVector2D>();
    bIsMoving = GetCharacterMovement()->IsMovingOnGround();

}

void APristsWithGunsCharacter::Look(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X * LookSensitivity);
        AddControllerPitchInput(LookAxisVector.Y * LookSensitivity);
    }
}

UActorComponent *APristsWithGunsCharacter::HasInteractableComponent(AActor *Actor)
{
    if (Actor)
    {
        for (TObjectPtr<UActorComponent> Component : Actor->GetComponents())
        {
            if (Component->Implements<UInteractable>()) { return Component.Get(); }
        }
    }
    return nullptr;
}

void APristsWithGunsCharacter::CheckForInteractableInRange()
{
    const FVector LineStart = FirstPersonCameraComponent->GetComponentLocation();
    const FVector LineEnd = LineStart + FirstPersonCameraComponent->GetForwardVector() * InteractRange;

    FHitResult HitResult;
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(this);

    bool bHit = GetWorld()->LineTraceSingleByChannel(
        HitResult,
        LineStart,
        LineEnd,
        ECC_Visibility,
        QueryParams);
    if (bHit)
    {
        if (AActor *HitActor = HitResult.GetActor())
        {
            if (HitActor->Implements<UInteractable>())
            {
                InteractableInRange = HitActor;
                InteractableActorInRange = HitActor;
            }

            else if (UActorComponent *ValidComponent = HasInteractableComponent(HitActor))
            {
                InteractableInRange.SetObject(ValidComponent);
                InteractableInRange.SetInterface(Cast<IInteractable>(ValidComponent));
            }

            else
            {
                HideCrosshair();
                return;
            }

            if (!bShowingCrosshair && !bIsInteracting)
            {
                CrosshairWidget->SetInteractionText(InteractableInRange->GetInteractionText());
                CrosshairWidget->PlayGrowAnimation(false);
                bShowingCrosshair = true;
            }
        }
    }
    else
    {
        HideCrosshair();
    }

    if (bPrintDebug)
    {
        DrawDebugLine(GetWorld(),
                      LineStart,
                      LineEnd,
                      InteractableInRange ? FColor::Green : FColor::Red,
                      false,
                      -1.0f,
                      0,
                      1.0f);
    }
}

void APristsWithGunsCharacter::BookPrevPage()
{
    BookWidget->PrevPage();
}

void APristsWithGunsCharacter::BookNextPage()
{
    BookWidget->NextPage();
}

void APristsWithGunsCharacter::EnterBookContext()
{

    if (APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(DefaultMappingContext);
            Subsystem->AddMappingContext(BookMappingContext, 0);
            if (BookWidget)
            {
                BookWidget->AddToViewport();
            }
        }
    }
}

void APristsWithGunsCharacter::ExitBookContext()
{

    BookWidget->CloseBook();

    if (BookWidget)
    {
        BookWidget->RemoveFromViewport();
    }
    if (APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(BookMappingContext);
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
    ReleaseBookWidget();
}

void APristsWithGunsCharacter::HideCrosshair()
{
    if (InteractableInRange)
    {
        if (UObservableObjectComponent *ObservableComponent = Cast<UObservableObjectComponent>(
            InteractableInRange.GetObject()))
        {
            ObservableComponent->ResetObservation();
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,
                                             TEXT("ObservableComponent setting bIsInteracting to false"));
        }
    }

    InteractableInRange = nullptr;
    InteractableActorInRange = nullptr;
    if (bShowingCrosshair)
    {
        CrosshairWidget->PlayGrowAnimation(true);
        CrosshairWidget->StopPlayFlipFlopButtonAnimation();
        CrosshairWidget->ClearDialogueOptions();
        bShowingCrosshair = false;
    }
}

UPhysicalMaterial *APristsWithGunsCharacter::GetPhysicalMaterialByLineTrace(const float &OffsetZ, const bool &bDebug)
{
    const FVector LineStart = GetActorLocation();
    const FVector LineEnd = LineStart + FVector(0, 0, Offset);

    EDrawDebugTrace::Type DebugType;
    FHitResult HitResult;

    if (bDebug)
    {
        DebugType = EDrawDebugTrace::ForDuration;
    }
    else
    {
        DebugType = EDrawDebugTrace::None;
    }

    UKismetSystemLibrary::LineTraceSingle(this, LineStart, LineEnd, UEngineTypes::ConvertToTraceType(ECC_Visibility),
                                          false, TArray<AActor *>(), DebugType, HitResult, true, FLinearColor::Red,
                                          FLinearColor::Green, 2.f);

    return Cast<UPhysicalMaterial>(HitResult.PhysMaterial);

}

void APristsWithGunsCharacter::SetFootstepsParameter(const UPhysicalMaterial *HitPhysicalMaterial, const bool &bDebug,
                                                     const FName ParameterName)
{
    if (PhysicalMaterialMap.Contains(HitPhysicalMaterial))
    {
        FmodAudioComponent->SetParameter(ParameterName, PhysicalMaterialMap[HitPhysicalMaterial]);

        if (bDebug)
        {
            GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Orange,
                                             "Physical Material: " + HitPhysicalMaterial->GetName());
        }
    }
    else
    {
        FmodAudioComponent->SetParameter(ParameterName, 0); // Default to carpet
    }
}

void APristsWithGunsCharacter::MovePuzzle(const FInputActionValue &Value)
{
    if (!bPuzzleCanMove)
    {
        return;
    }
    bPuzzleCanMove = false;
    FVector2D Input = Value.Get<FVector2D>();
    if (InteractableActorInRange.IsValid())
    {
        if (APuzzleGrid *PuzzleGrid = Cast<APuzzleGrid>(InteractableActorInRange.Get()))
        {
            PuzzleGrid->HandleMovableInput(Input);
        }
    }
}

void APristsWithGunsCharacter::ResetPuzzleCanMove(const FInputActionValue &InputActionValue)
{
    bPuzzleCanMove = true;
}

void APristsWithGunsCharacter::ExitGridPuzzle()
{
    if (APlayerController *PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->SetViewTargetWithBlend(this, CameraTransitionTime);
    }
    bIsInteracting = false;
    if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(PuzzleGridMappingContext);
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void APristsWithGunsCharacter::EnterGridPuzzle(APuzzleGrid *PuzzleGrid)
{
    bIsInteracting = true;

    if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
    {
        PlayerController->bFindCameraComponentWhenViewTarget = true;
        PlayerController->SetViewTargetWithBlend(PuzzleGrid, CameraTransitionTime);

        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<
            UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->RemoveMappingContext(DefaultMappingContext);
            Subsystem->AddMappingContext(PuzzleGridMappingContext, 0);
        }
    }
    PuzzleGrid->SetPlayer(this);
}

void APristsWithGunsCharacter::Interact()
{
    if (InteractableInRange)
    {
        if (bShowingCrosshair)
        {
            CrosshairWidget->PlayGrowAnimation(true);
            bShowingCrosshair = false;
        }
        InteractableInRange->Interact(this);

    }

}

void APristsWithGunsCharacter::PauseGame()
{
    if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
    {
        PlayerController->SetPause(true);
        PlayerController->SetShowMouseCursor(true);
        FInputModeGameAndUI InputMode;
        InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
        PlayerController->SetInputMode(InputMode);

        if (PauseMenuWidget)
        {
            PauseMenuWidget->FadeIn();
        }
    }

}

void APristsWithGunsCharacter::UnpauseGame()
{
    if (PauseMenuWidget)
    {
        if (APlayerController *PlayerController = Cast<APlayerController>(GetController()))
        {
            PlayerController->SetPause(false);
            PlayerController->SetShowMouseCursor(false);
            FInputModeGameOnly InputMode;
            PlayerController->SetInputMode(InputMode);

            if (PauseMenuWidget)
            {
                PauseMenuWidget->FadeOut();
            }
        }

    }
}

void APristsWithGunsCharacter::NextObservation()
{
    // if player is looking at an observable object
    if (UObservableObjectComponent *ObservableComponent = Cast<UObservableObjectComponent>(
        InteractableInRange.GetObject()))
    {
        if (ObservableComponent->bIsInteracting)
        {
            ObservableComponent->NextObservation();
        }
    }
}

void APristsWithGunsCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    CheckForInteractableInRange();

    if (bIsMoving && !GetMovementComponent()->IsFalling())
    {
        FootstepTimeAccumulator += DeltaTime;

        if (CameraShake)
        {
            ULegacyCameraShake *LegacyShakeInstance = Cast<ULegacyCameraShake>(CameraShake->GetDefaultObject());

            if (LegacyShakeInstance)
            {
                const float PitchFrequency = LegacyShakeInstance->RotOscillation.Pitch.Frequency;
                float FootstepInterval = (1 / PitchFrequency) * FootstepFrequencyAdjuster;
                if (FootstepTimeAccumulator >= FootstepInterval) // F = 1 / T
                {
                    if (bPrintDebug)
                    {
                        GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Green,
                                                         FString::Printf(
                                                             TEXT("FootstepInterval: %f and bIsMoving: %hhd"),
                                                             FootstepInterval, bIsMoving));
                    }
                    SetFootstepsParameter(GetPhysicalMaterialByLineTrace(Offset, bPrintDebug), bPrintDebug,
                                          TEXT("SurfaceType"));
                    FmodAudioComponent->Play();
                    FootstepTimeAccumulator = 0;
                }
            }
        }
    }
    else
    {
        FmodAudioComponent->Stop();
    }

}