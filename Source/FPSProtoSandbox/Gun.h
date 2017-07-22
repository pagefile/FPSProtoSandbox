// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeapProjectile.h"
#include "Gun.generated.h"

class AFPSProtoSandboxCharacter;

UCLASS()
class FPSPROTOSANDBOX_API AGun : public AActor
{
	GENERATED_BODY()
	
public:

    // Properties
    UPROPERTY(EditDefaultsOnly, Category = Mesh)
    USkeletalMeshComponent* Mesh1P;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    FVector GunOffset;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    TSubclassOf<AWeapProjectile> FireProjectile;

    // TODO: Use a sound queue. Sound queues allow for crossfade by distance, which would make
    // better gun sounds depending on how far away the shot is
    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    USoundBase *FireSound;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    bool IsAutomatic;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float CycleRate;

public:
	// Sets default values for this actor's properties
	AGun();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    // Gameplay functions
    void EquipTo(AFPSProtoSandboxCharacter *pawn);
    void Unequip();

    void OnFire();

    void TriggerDown();

    void TriggerUp();

public:
    // Getters and setters
    
    FTransform GetMuzzleTransform();

    FVector GetMuzzleDir();

    FVector GetMuzzlePoint();
protected:
    // owner
    AFPSProtoSandboxCharacter *MyPawn;

    bool IsTriggerDown;

    // Cycle timing
    bool ReadyToFire;
    float CurrentCycleTime;
};
