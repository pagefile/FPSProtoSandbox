// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/Character.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "FPSProtoSandboxCharacter.generated.h"

class UInputComponent;
class AGun;
class APickup;

UCLASS(config=Game)
class AFPSProtoSandboxCharacter : public ACharacter
{
	GENERATED_BODY()

	///** Pawn mesh: 1st person view (arms; seen only by self) */
	//UPROPERTY(EditDefaultsOnly, Category=Mesh)
	//class USkeletalMeshComponent* Mesh1P;

	///** Gun mesh: 1st person view (seen only by self) */
	//UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	//class USkeletalMeshComponent* FP_Gun;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;
public:
	AFPSProtoSandboxCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AFPSProtoSandboxProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    USpotLightComponent* FlashLightComponent;

    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
    float MaxHealth;

protected:
	
	/** Fires a projectile. */
	void OnTriggerDown();

    void OnTriggerUp();


	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

    void ToggleFlashLight(void);

    void DropWeapon(void);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;

    void NextWeapon();
    void PrevWeapon();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/* 
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so 
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* InputComponent);

public:
	/** Returns Mesh1P subobject **/
	/*FORCEINLINE class USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }*/
	/** Returns FirstPersonCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


// Collision Events
public:
    UFUNCTION()
    void OnBeginOverlap(AActor *other);

    // Overrides
    virtual void BeginPlay() override;

    bool IsPlayerDead();
   
protected:
    // Gameplay variables
    TArray<APickup*> WeaponInv;
    int EquipIndex;
    float CurrentHealth;
    bool IsDead;

    // Gameplay Functions
    void AddGunToInv(APickup* gun);

    void RemoveGunfromInv(APickup* gun);

    void EquipWeap(AGun* gun);

    void UnequipWeap();

    void EquipWeaponIndex(int slot);

    virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *Instigator, AActor *Causer);

// Utility functions
    AGun* GetEquippedGun();
    APickup* GetEquippedPickup();

    void EquipSelectedWeapon();

};

