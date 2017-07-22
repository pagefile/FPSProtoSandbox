// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSProtoSandbox.h"
#include "Explosion.h"


// Sets default values
AExplosion::AExplosion()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    ExplosionParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionEmitter"));
    ExplosionParticle->AttachTo(RootComponent);
    Damage = 0.0f;
    Falloff = 0.0f;
    ExplosionKnockback = 0.0f;
    //InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void AExplosion::BeginPlay()
{
	Super::BeginPlay();

    FRotator ImpactRotator = SurfaceHit.ImpactNormal.Rotation();
    SetActorRotation(ImpactRotator);

    if(ExplosionSound != nullptr)
    {
        UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
    }
}

// Called every frame
void AExplosion::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

