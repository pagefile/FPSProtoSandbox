// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FPSProtoSandbox.h"
#include "FPSProtoSandboxCharacter.h"
#include "FPSProtoSandboxProjectile.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/InputSettings.h"
#include "Gun.h"
#include "Pickup.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

//////////////////////////////////////////////////////////////////////////
// AFPSProtoSandboxCharacter

AFPSProtoSandboxCharacter::AFPSProtoSandboxCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachParent = GetCapsuleComponent();
	FirstPersonCameraComponent->RelativeLocation = FVector(0, 0, 64.f); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

    // Create flashlight
    FlashLightComponent = CreateDefaultSubobject<USpotLightComponent>(TEXT("FPSFlashLight"));
    FlashLightComponent->AttachTo(FirstPersonCameraComponent);
    FlashLightComponent->bVisible = false;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	//Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	//Mesh1P->SetOnlyOwnerSee(true);
	//Mesh1P->AttachParent = GetCapsuleComponent();
	//Mesh1P->bCastDynamicShadow = false;
	//Mesh1P->CastShadow = false;

	// Create a gun mesh component
	//FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	//FP_Gun->SetOnlyOwnerSee(true);			// only the owning player will see this mesh
	//FP_Gun->bCastDynamicShadow = false;
	//FP_Gun->CastShadow = false;
	//FP_Gun->AttachTo(Mesh1P, TEXT("GripPoint"), EAttachLocation::SnapToTargetIncludingScale, true);

    EquipIndex = 0;
	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P are set in the
	// derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void AFPSProtoSandboxCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	// set up gameplay key bindings
	check(InputComponent);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	//InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSProtoSandboxCharacter::TouchStarted);
	if( EnableTouchscreenMovement(InputComponent) == false )
	{
		InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSProtoSandboxCharacter::OnTriggerDown);
        InputComponent->BindAction("Fire", IE_Released, this, &AFPSProtoSandboxCharacter::OnTriggerUp);
	}
	
	InputComponent->BindAxis("MoveForward", this, &AFPSProtoSandboxCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSProtoSandboxCharacter::MoveRight);
	
	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("TurnRate", this, &AFPSProtoSandboxCharacter::TurnAtRate);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("LookUpRate", this, &AFPSProtoSandboxCharacter::LookUpAtRate);

    InputComponent->BindAction("Flash Light", IE_Pressed, this, &AFPSProtoSandboxCharacter::ToggleFlashLight);
    InputComponent->BindAction("Drop Weapon", IE_Pressed, this, &AFPSProtoSandboxCharacter::DropWeapon);
    InputComponent->BindAction("Next Weapon", IE_Pressed, this, &AFPSProtoSandboxCharacter::NextWeapon);
    InputComponent->BindAction("Previous Weapon", IE_Pressed, this, &AFPSProtoSandboxCharacter::PrevWeapon);
}

void AFPSProtoSandboxCharacter::BeginPlay()
{
    // events
    OnActorBeginOverlap.AddDynamic(this, &AFPSProtoSandboxCharacter::OnBeginOverlap);
    GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    CurrentHealth = MaxHealth;
    IsDead = false;
}

void AFPSProtoSandboxCharacter::OnTriggerDown()
{ 
    AGun* gun = GetEquippedGun();
    if(gun == nullptr)
    {
        return;
    }
    gun->TriggerDown();
}

void AFPSProtoSandboxCharacter::OnTriggerUp()
{
    AGun* gun = GetEquippedGun();
    if(gun == nullptr)
    {
        return;
    }
    gun->TriggerUp();
}

void AFPSProtoSandboxCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if( TouchItem.bIsPressed == true )
	{
		return;
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void AFPSProtoSandboxCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	if( ( FingerIndex == TouchItem.FingerIndex ) && (TouchItem.bMoved == false) )
	{
		OnTriggerDown();
	}
	TouchItem.bIsPressed = false;
}

void AFPSProtoSandboxCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && ( TouchItem.FingerIndex==FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D( MoveDelta.X, MoveDelta.Y) / ScreenSize;									
					if (ScaledDelta.X != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (ScaledDelta.Y != 0.0f)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y* BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void AFPSProtoSandboxCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AFPSProtoSandboxCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AFPSProtoSandboxCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AFPSProtoSandboxCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

bool AFPSProtoSandboxCharacter::EnableTouchscreenMovement(class UInputComponent* InputComponent)
{
	bool bResult = false;
	if(FPlatformMisc::GetUseVirtualJoysticks() || GetDefault<UInputSettings>()->bUseMouseForTouch )
	{
		bResult = true;
		InputComponent->BindTouch(EInputEvent::IE_Pressed, this, &AFPSProtoSandboxCharacter::BeginTouch);
		InputComponent->BindTouch(EInputEvent::IE_Released, this, &AFPSProtoSandboxCharacter::EndTouch);
		InputComponent->BindTouch(EInputEvent::IE_Repeat, this, &AFPSProtoSandboxCharacter::TouchUpdate);
	}
	return bResult;
}

void AFPSProtoSandboxCharacter::ToggleFlashLight(void)
{
    FlashLightComponent->ToggleVisibility();
}

void AFPSProtoSandboxCharacter::DropWeapon(void)
{
    AGun *gunToThrow;
    gunToThrow = GetEquippedGun();
    if(gunToThrow == nullptr)
    {
        return;
    }
    UnequipWeap();

    // kill the gun
    // TODO: Not too sure about constantly spawning and destroying guns...
    APickup* thrown = GetEquippedPickup();
    if(thrown == nullptr)
    {
        return;
    }
    // TODO: Have some of this owner management code to APickup
    RemoveGunfromInv(thrown);

    // TODO: Put the following lines into a function. Order is apparently very important
    thrown->SetPreviousOwner(this);
    thrown->SetActorLocation(GetActorLocation());
    thrown->SetActorHiddenInGame(false);
    thrown->SetActorTickEnabled(true);
    thrown->SetActorEnableCollision(true);

    // TODO: So...uh...let's do some vector math and stuff
    FVector lookVec = FirstPersonCameraComponent->GetForwardVector();// + FVector::UpVector * 0.25f;
    FVector playerVel = GetVelocity();
    UPrimitiveComponent *thrownSM = Cast<UPrimitiveComponent>(thrown->GetRootComponent());
    if(thrownSM == nullptr)
    {
        return;
    }

    FVector angularVel = FMath::VRand() * 50.0f;
    thrownSM->SetPhysicsLinearVelocity(playerVel);
    thrownSM->SetWorldRotation(FirstPersonCameraComponent->GetComponentRotation());
    thrownSM->SetPhysicsAngularVelocity(angularVel);
    
    // random rotation
    FVector throwVec = lookVec * 1000.0f * thrownSM->GetMass();
    thrownSM->AddImpulse(throwVec);

    // Set the position
    thrownSM->SetWorldLocation(gunToThrow->Mesh1P->GetComponentLocation());

    gunToThrow->Unequip();


    if(WeaponInv.Num() > 0)
    {
        // Equip a new weapon
        //AGun *toEquip = GetEquippedGun();
        //EquipWeap(toEquip);
        EquipWeaponIndex(EquipIndex);
    }
}

void AFPSProtoSandboxCharacter::OnBeginOverlap(AActor *other)
{
    // Messy messy stuff, but this is the prototype phase. Clean it up later    

    // TODO: Add some code to handle inventory stuff
    if(other->IsA<APickup>())
    {
        APickup *pickup = Cast<APickup>(other);
        if(!pickup->CanPickup(this))
        {
            return;
        }
        AGun* gun = pickup->GetGun();
        if(gun == nullptr)
        {
            return;
        }
        if(WeaponInv.Num() == 0)
        {
            EquipWeap(gun);
        }
        other->SetActorHiddenInGame(true);
        other->SetActorTickEnabled(false);
        other->SetActorEnableCollision(false);
        AddGunToInv(pickup);
        pickup->SetOwner(this);

        if(GEngine)
        {
            GEngine->AddOnScreenDebugMessage(-1, 5, FColor::White, TEXT("Equipping gun"));
        }
    }
}

void AFPSProtoSandboxCharacter::AddGunToInv(APickup* gun)
{
    int index = WeaponInv.AddUnique(gun);
    if(WeaponInv.Num() == 1)
    {
        EquipIndex = index;
    }
}

void AFPSProtoSandboxCharacter::RemoveGunfromInv(APickup* gun)
{
    int removed = WeaponInv.Remove(gun);
    // Make sure the current EquipIndex is still valid
    int weapCount = WeaponInv.Num();
    if(EquipIndex >= weapCount)
    {
        EquipIndex = weapCount - 1;
    }
}

void AFPSProtoSandboxCharacter::EquipWeap(AGun* gun)
{
    gun->EquipTo(this);
    gun->SetActorHiddenInGame(false);
    gun->SetActorTickEnabled(true);
    gun->EquipTo(this);
}

void AFPSProtoSandboxCharacter::UnequipWeap()
{
    AGun* gun = GetEquippedGun();
    gun->SetActorHiddenInGame(true);
    gun->SetActorTickEnabled(false);
    gun->Unequip();
}

AGun* AFPSProtoSandboxCharacter::GetEquippedGun()
{
    if(WeaponInv.Num() == 0)
    {
        // nope.avi
        return nullptr;
    }
    APickup* pickup = WeaponInv[EquipIndex];
    AGun* gun = nullptr;
    if(pickup != nullptr)
    {
        gun = pickup->GetGun();
    }
    
    return gun;
}

// TODO: Could have global error pickup and gun objects to reference as error objects to hopefully keep crashes from occuring
// Might be worth doing?
APickup* AFPSProtoSandboxCharacter::GetEquippedPickup()
{
    return WeaponInv[EquipIndex];
}

void AFPSProtoSandboxCharacter::NextWeapon()
{
    if(WeaponInv.Num() > 1)
    {
        EquipWeaponIndex(EquipIndex + 1);
    }
}

void AFPSProtoSandboxCharacter::PrevWeapon()
{
    // TODO: Change this so that this unequips the current weap regardless of the "selected" index.
    // Need a more robust system for that
    if(WeaponInv.Num() > 1)
    {
        EquipWeaponIndex(EquipIndex - 1);
    }
}

void AFPSProtoSandboxCharacter::EquipWeaponIndex(int slot)
{
    UnequipWeap();
    int weapCount = WeaponInv.Num();
    if(slot < 0)
    {
        slot = weapCount - 1;
    }
    else if(slot >= weapCount)
    {
        slot = 0;
    }
    EquipIndex = slot;
    APickup* invItem = WeaponInv[EquipIndex];
    AGun* invGun = invItem->GetGun();
    EquipWeap(invGun);
}

void AFPSProtoSandboxCharacter::EquipSelectedWeapon()
{
    // For now the bounds rollover code is in here.
    int weapCount = WeaponInv.Num();
    if(EquipIndex < 0)
    {
        EquipIndex = weapCount - 1;
    }
    else if(EquipIndex >= weapCount)
    {
        EquipIndex = 0;
    }

    if(weapCount <= 1)
    {
        // Nothing to do in this case
        return;
    }

    APickup* invItem = WeaponInv[EquipIndex];
    AGun* invGun = invItem->GetGun();
    EquipWeap(invGun);
}

float AFPSProtoSandboxCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *Instigator, AActor *Causer)
{
    float actualDamage = DamageAmount;
    CurrentHealth -= MaxHealth;
    if(CurrentHealth <= 0.0f)
    {
        IsDead = true;

        // TODO: We'll have to figure something out for this later. Probably notify the game type or something
        Controller->UnPossess();
    }

    return actualDamage; 
}

bool AFPSProtoSandboxCharacter::IsPlayerDead()
{
    return IsDead;
}