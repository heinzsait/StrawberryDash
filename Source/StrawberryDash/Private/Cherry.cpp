// Fill out your copyright notice in the Description page of Project Settings.


#include "Cherry.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ACherry::ACherry()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACherry::BeginPlay()
{
	Super::BeginPlay();
	LockOn();
}

// Called every frame
void ACherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AActor* ACherry::FindTargets()
{
	TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), TSubclassOf<AActor>(), FName("Enemy"), actors);

	float minDist = 9999;
	AActor* targetActor = NULL;
	for (AActor* actor : actors)
	{
		FVector targetPos = actor->GetActorLocation();
		float dist = (targetPos - GetActorLocation()).Length();

		if (minDist > dist)
		{
			minDist = dist;
			targetActor = actor;
		}
	}

	return targetActor;
}

void ACherry::LockOn()
{
	//->HomingTargetComponent = FindTargets();	
	//projectile->HomingTargetComponent = Cast<USceneComponent>(FindTargets()->GetComponentByClass(TSubclassOf<UActorComponent>()));
	
	//projectile->bIsHomingProjectile = true;
}

