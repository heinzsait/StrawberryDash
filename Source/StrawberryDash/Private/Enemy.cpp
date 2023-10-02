// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Core/AI/DemonAIController.h"
#include "StrawberryDash/Strawberry.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	HP = 100;

	if (USkeletalMeshComponent* _mesh = FindComponentByClass<USkeletalMeshComponent>())
	{
		AnimInst = _mesh->GetAnimInstance();

		if (AnimInst)
		{
			gotHit = FindFProperty<FBoolProperty>(AnimInst->GetClass(), TEXT("GotHit"));
			deadAnim = FindFProperty<FBoolProperty>(AnimInst->GetClass(), TEXT("Dead"));
			punchAnim = FindFProperty<FBoolProperty>(AnimInst->GetClass(), TEXT("Punch"));
		}
	}

	player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Follow();
	Attack();
}

void AEnemy::TakeDamage(float dmg)
{
	if (isDead) return;

	HP -= dmg;
	gotHit->SetPropertyValue_InContainer(AnimInst, true);
	if (HP <= 0)
	{
		isDead = true;
		deadAnim->SetPropertyValue_InContainer(AnimInst, true);
		AAIController* aiController = CastChecked<AAIController>(GetController());
		ADemonAIController* demonAI = Cast<ADemonAIController>(aiController);
		if (demonAI)
		{
			demonAI->StopAI();
		}

		if (USkeletalMeshComponent* _mesh = FindComponentByClass<USkeletalMeshComponent>())
		{
			_mesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		}
		SetLifeSpan(3);
	}
}

void AEnemy::Follow()
{
	//MoveToLocation(player->GetActorLocation(), 100);
}

void AEnemy::Attack()
{
	if (isDead) return;

	if (player)
	{
		float dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());
		if (dist < 250)
		{
			punchAnim->SetPropertyValue_InContainer(AnimInst, true);
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("Attacking player...")));

			FVector Direction = player->GetActorLocation() - GetActorLocation();
			FRotator NewControlRotation = Direction.Rotation();

			NewControlRotation.Yaw = FRotator::ClampAxis(NewControlRotation.Yaw);

			FaceRotation(NewControlRotation, GetWorld()->DeltaRealTimeSeconds);

			AStrawberry* strawberry = Cast<AStrawberry>(player);
			if (strawberry)
			{
				strawberry->TakeDamage(10);
			}

		}
		else
		{
			punchAnim->SetPropertyValue_InContainer(AnimInst, false);
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("Not Attacking player...")));
		}
	}
}

