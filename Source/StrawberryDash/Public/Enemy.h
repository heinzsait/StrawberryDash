// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"


UCLASS()
class STRAWBERRYDASH_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float HP;

	AActor* player;

	UAnimInstance* AnimInst;
	FBoolProperty* gotHit;
	FBoolProperty* deadAnim;
	FBoolProperty* punchAnim;

	//AAIController* aiController;

	bool isDead;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void TakeDamage(float dmg);

	void Attack();
	void Follow();
};
