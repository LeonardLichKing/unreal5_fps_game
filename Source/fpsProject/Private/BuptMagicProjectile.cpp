// Fill out your copyright notice in the Description page of Project Settings.


#include "BuptMagicProjectile.h"

#include "BuptActionrComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "BuptAttributeComponent.h"
#include "BuptGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GamePlayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABuptMagicProjectile::ABuptMagicProjectile()
{
	SphereComp->SetSphereRadius(20.0f);
	InitialLifeSpan = 10.0f;
	Damage = -20.0f;
	//it seems like the audio will autoplay when AudioComponent construct.
}

void ABuptMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABuptMagicProjectile::OnActorOverlap);
}

void ABuptMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor&&OtherActor!=GetInstigator())
	{
		LoopedAudioComp->Stop();

		UGameplayStatics::PlaySoundAtLocation(GetWorld(),ImpactAudioComp->GetSound(),GetActorLocation(),GetActorRotation());

		UBuptActionrComponent* ActionComp=Cast<UBuptActionrComponent>(OtherActor->GetComponentByClass(UBuptActionrComponent::StaticClass()));

		if(ActionComp&&ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			MovementComp->Velocity=-MovementComp->Velocity;

			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		// UBuptAttributeComponent* AttributeComp = Cast<UBuptAttributeComponent>(OtherActor->GetComponentByClass(UBuptAttributeComponent::StaticClass()));
		// if (AttributeComp)
		// {
		// 	AttributeComp->ApplyHealthChange(GetInstigator(),Damage);
		// 	Destroy();
		// 	// APlayerController* PCController=GetWorld()->GetFirstPlayerController();
		// 	// PCController->ClientStartCameraShake(CameraShakeComp);
		// }
		if(UBuptGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(),OtherActor,Damage,SweepResult))
		{
			Destroy();
		}
	}
}




