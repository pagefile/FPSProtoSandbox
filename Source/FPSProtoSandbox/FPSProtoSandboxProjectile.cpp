// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "FPSProtoSandbox.h"
#include "FPSProtoSandboxProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

AFPSProtoSandboxProjectile::AFPSProtoSandboxProjectile() 
{
    static ConstructorHelpers::FObjectFinder<UParticleSystem> SmokeTrailObj(TEXT("ParticleSystem'/Game/Particles/P_RibbonSmoke.P_RibbonSmoke'"));
    UParticleSystemComponent* SmokeTrailPs = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SmokeTrailObj"));
    

    if(SmokeTrailObj.Succeeded())
    {
        SmokeTrailPs->Template = SmokeTrailObj.Object;
        SmokeTrailPs->bAutoActivate = true;
    }

	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(1.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &AFPSProtoSandboxProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;
    CollisionComp->SetVisibility(false);

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 100000.0f;
	ProjectileMovement->MaxSpeed = 100000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void AFPSProtoSandboxProjectile::OnHit(AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		Destroy();
	}
}