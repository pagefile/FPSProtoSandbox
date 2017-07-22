// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

// Forward Declarations
class AGun;

UCLASS()
class FPSPROTOSANDBOX_API APickup : public AActor
{
	GENERATED_BODY()

public:
    // Data
    UPROPERTY(EditDefaultsOnly, Category=Mesh)
    USkeletalMeshComponent* Mesh;

    UPROPERTY(EditDefaultsOnly, Category=Physics)
    UPhysicalMaterial *PhysicsMaterial;

    UPROPERTY(EditDefaultsOnly, Category=Collision)
    UBoxComponent *CollisionBounds;

    UPROPERTY(EditDefaultsOnly, Category=Gameplay)
    TSubclassOf<AGun> Weapon;

public:	
	// Sets default values for this actor's properties
	APickup();
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
    virtual void BeginPlay(void) override;

    void OnHit(AActor* self, AActor* other, FVector normal, const FHitResult &hitResult);

    AGun* GetGun();

    void SetPreviousOwner(APawn* owner, float time = 1.0f);
    bool CanPickup(APawn* pawn);

protected:

    AGun *GunInstance;

    APawn *PreviousOwner;       // timer so pawns don't pick it up right after dropping it
    float PrevOwnerTimeout;
};
