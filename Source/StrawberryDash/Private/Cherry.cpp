// Fill out your copyright notice in the Description page of Project Settings.


#include "Cherry.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Math/Vector.h"
#include "Components/BoxComponent.h"
#include "Enemy.h"

// Sets default values
ACherry::ACherry()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	projectileSpeed = 5000;
	minCurveRadius = 100;
	maxCurveRadius = 500;
	minCurvePointDistance = 0.2;
	maxCurvePointDistance = 0.8;
	minAngle = 0;
	maxAngle = 360;
	pointsCalculated = false;

	/*Trigger = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	Trigger->InitBoxExtent(FVector(20.0f, 20.0f, 20.0f));
	Trigger->SetCollisionProfileName(TEXT("Trigger"));
	Trigger->SetupAttachment(RootComponent);

	Trigger->OnComponentBeginOverlap.AddDynamic(this, &ACherry::OnOverlapBegin);
	Trigger->OnComponentEndOverlap.AddDynamic(this, &ACherry::OnOverlapEnd);*/

	
	//

}

// Called when the game starts or when spawned
void ACherry::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(3);
	
	mesh = Cast<UStaticMeshComponent>(GetComponentByClass(UStaticMeshComponent::StaticClass()));
	if (mesh)
	{
		mesh->OnComponentHit.AddDynamic(this, &ACherry::OnCompHit);
	}
}

// Called every frame
void ACherry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (pointsCalculated)
	{
		ProjectileUpdate();
	}	
}

void ACherry::FireTarget()
{
	FVector x;
	FVector y;
	FVector z;
	midPoint = ((targetLocation - GetActorLocation()) * (FMath::RandRange(minCurvePointDistance, maxCurvePointDistance))) + GetActorLocation();
	UKismetMathLibrary::BreakRotIntoAxes(UKismetMathLibrary::FindLookAtRotation(midPoint, targetLocation), x, y, z);
	FVector inVect = (FMath::RandRange(minCurveRadius, maxCurveRadius) * y);
	curvedPoint = midPoint + inVect.RotateAngleAxis(FMath::RandRange(minAngle, maxAngle), x);
	pointsCalculated = true;
}

void ACherry::ProjectileUpdate()
{
	if (GetActorLocation() != FVector().ZeroVector && targetLocation != FVector().ZeroVector)
	{
		float delta = GetWorld()->GetDeltaSeconds();
		curvedPoint = FMath::VInterpConstantTo(curvedPoint, targetLocation, delta, projectileSpeed);
		FVector pos = FMath::VInterpConstantTo(GetActorLocation(), curvedPoint, delta, projectileSpeed);
		SetActorLocation(pos, true);
	}
}

void ACherry::OnCompHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && (OtherActor != strawberry))
	{
		if (OtherActor->IsA(AEnemy::StaticClass()))
		{
			AEnemy* enemy = Cast<AEnemy>(OtherActor);
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("I Hit: %s"), *enemy->GetName()));
			enemy->TakeDamage(20);
		}
		Destroy();
	}
}

//void ACherry::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	if (OtherActor && (OtherActor != this) && OtherComp)
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Overlapped :" + OtherActor->GetFName().ToString());
//	}
//}
//
//void ACherry::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
//{
//	/*if (OtherActor && (OtherActor != this) && OtherComp && (OtherActor->GetClass() != TSubclassOf<AStrawberry>()))
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Overlap End"));
//	}*/
//}

