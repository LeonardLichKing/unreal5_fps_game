// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BuptAttributeComponent.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GamePlayStatics.h"

// Sets default values
ABuptMagicProjectile::ABuptMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetCollisionProfileName("Projectile");
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABuptMagicProjectile::OnActorOverlap);
	RootComponent = SphereComp;

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComp");
	EffectComp->SetupAttachment(SphereComp);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("MovementComp");
	MovementComp->InitialSpeed = 1000.0f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;

	LoopedAudioComp=CreateDefaultSubobject<UAudioComponent>("LoopedAudioComp");
	ImpactAudioComp=CreateDefaultSubobject<UAudioComponent>("ImpactAudioComp");
	//it seems like the audio will autoplay when AudioComponent construct.
}

void ABuptMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor&&OtherActor!=GetInstigator())
	{
		LoopedAudioComp->Stop();

		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactAudioComp->GetSound(),GetActorLocation(),GetActorRotation());
		
		UBuptAttributeComponent* AttributeComp = Cast<UBuptAttributeComponent>(OtherActor->GetComponentByClass(UBuptAttributeComponent::StaticClass()));
		if (AttributeComp)
		{
			AttributeComp->ApplyHealthChange(-20.0f);
			Destroy();
		}
	}
}

// Called when the game starts or when spawned
void ABuptMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	LoopedAudioComp->Play();
}

// Called every frame
void ABuptMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LoopedAudioComp->SetWorldLocation(GetActorLocation());
}

