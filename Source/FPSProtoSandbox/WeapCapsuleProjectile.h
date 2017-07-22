// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "WeapProjectile.h"
#include "WeapCapsuleProjectile.generated.h"

UCLASS()
class FPSPROTOSANDBOX_API AWeapCapsuleProjectile : public AWeapProjectile
{
	GENERATED_BODY()
	
    // UProperties
    UPROPERTY(EditDefaultsOnly, Category = Mesh)
    UStaticMeshComponent *Mesh;

    UPROPERTY(EditDefaultsOnly, Category = Effects)
    UParticleSystemComponent *ParticleTrail;
	
    UPROPERTY(EditDefaultsOnly, Category = Collision)
    UCapsuleComponent *CollisionComp;

public:	
	// Sets default values for this actor's properties
	AWeapCapsuleProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;


    virtual void PostActorCreated() override;
};
