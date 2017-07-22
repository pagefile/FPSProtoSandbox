// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "WeapProjectile.h"
#include "WeapBallProjectile.generated.h"

UCLASS()
class FPSPROTOSANDBOX_API AWeapBallProjectile : public AWeapProjectile
{
	GENERATED_BODY()
	
    // UProperties
    UPROPERTY(EditDefaultsOnly, Category = Mesh)
    UStaticMeshComponent *Mesh;

    UPROPERTY(EditDefaultsOnly, Category = Effects)
    UParticleSystemComponent *ParticleTrail;

    UPROPERTY(EditDefaultsOnly, Category = Collision)
    USphereComponent *CollisionComp;

    UPROPERTY(EditDefaultsOnly, Category = Movement)
    UProjectileMovementComponent *ProjectileMovement;

    UPROPERTY(EditDefaultsOnly, Category = Gameplay)
    float Damage;

public:	
	// Sets default values for this actor's properties
	AWeapBallProjectile();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void PostActorCreated() override;
	
    UFUNCTION()
    void OnImpact(const FHitResult &hitResult);
};
