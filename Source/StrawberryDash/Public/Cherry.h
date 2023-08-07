// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Cherry.generated.h"

UCLASS()
class STRAWBERRYDASH_API ACherry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACherry();
	AActor* FindTargets();
	void LockOn();

	UProjectileMovementComponent* projectile;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
