// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "WeapBallProjectile.h"
#include "FPSProtoSandboxCharacter.h"

// Sets default values
AWeapBallProjectile::AWeapBallProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // TODO: Maybe this should be a mesh component instead of static mesh?
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjMesh"));

    CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
    CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
    CollisionComp->CanCharacterStepUpOn = ECB_No;
    CollisionComp->SetVisibility(false);

    RootComponent = CollisionComp;

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    
    ParticleTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ProjectileTrail"));
    ParticleTrail->AttachTo(RootComponent);

    InitialLifeSpan = 3.0f;
    Damage = 0.0f;

    Mesh->AttachTo(RootComponent);
}

void AWeapBallProjectile::PostActorCreated()
{
    CollisionComp->IgnoreActorWhenMoving(Instigator, true);   
}

// Called when the game starts or when spawned
void AWeapBallProjectile::BeginPlay()
{
	Super::BeginPlay();
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AWeapBallProjectile::OnImpact);
}

// Called every frame
void AWeapBallProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AWeapBallProjectile::OnImpact(const FHitResult &hitResult)
{
    // TODO: For some god forsaken reason the rotation isn't getting to the spawned actor. Figure it out later
    if(!bExploded)
    {
        FTransform spawnTrans = FTransform(hitResult.ImpactNormal.Rotation(), hitResult.ImpactPoint + hitResult.ImpactNormal);
        AExplosion* fx = Cast<AExplosion>(GetWorld()->SpawnActor((UClass *)Explosion, &spawnTrans, FActorSpawnParameters()));
        bExploded = true;
        // TODO: Pass information for instigator (Controller), causer (Pawn) and Damage Event
        FPointDamageEvent devent;
        devent.Damage = Damage;
        devent.HitInfo = hitResult;
        devent.ShotDirection = GetVelocity();       // Need to verify this isn't just going to return (0, 0, 0)
        if(hitResult.Actor != nullptr)
        {
            hitResult.Actor->TakeDamage(Damage, devent, nullptr, nullptr);
        }
        this->Destroy();
    }
}
