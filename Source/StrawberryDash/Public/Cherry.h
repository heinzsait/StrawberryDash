// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "Cherry.generated.h"

UCLASS()
class STRAWBERRYDASH_API ACherry : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACherry();
	void FireTarget();
	void ProjectileUpdate();

	FVector targetLocation;
	FVector midPoint;
	FVector curvedPoint;
	float projectileSpeed;
	float minCurveRadius;
	float maxCurveRadius;
	float minCurvePointDistance;
	float maxCurvePointDistance;
	float minAngle;
	float maxAngle;

	bool pointsCalculated;

	UStaticMeshComponent* mesh;

	UFUNCTION()
		void OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AActor* strawberry;

	//UPROPERTY(VisibleAnywhere, Category = "Trigger Box")
	//	UBoxComponent* Trigger;

	//// declare overlap begin function
	//UFUNCTION()
	//	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//// declare overlap end function
	//UFUNCTION()
	//	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
