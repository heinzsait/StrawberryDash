// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"

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
			gotHit = FindField<FBoolProperty>(AnimInst->GetClass(), TEXT("GotHit"));
			deadAnim = FindField<FBoolProperty>(AnimInst->GetClass(), TEXT("Dead"));
			punchAnim = FindField<FBoolProperty>(AnimInst->GetClass(), TEXT("Punch"));
		}
	}

	player = GetWorld()->GetFirstPlayerController()->GetPawn();// UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	//aiController = CastChecked<AAIController>(GetController());
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Follow();
	//Attack();
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
	
	float dist = FVector::Distance(GetActorLocation(), player->GetActorLocation());
	if (dist < 250)
	{
		punchAnim->SetPropertyValue_InContainer(AnimInst, true);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("Attacking player...")));
	}
	else
	{
		punchAnim->SetPropertyValue_InContainer(AnimInst, false);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::Printf(TEXT("Not Attacking player...")));
	}
}

