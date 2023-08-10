// Fill out your copyright notice in the Description page of Project Settings.


#include "Strawberry.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Cherry.h"

// Sets default values
AStrawberry::AStrawberry()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStrawberry::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AStrawberry::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	WallDash();
	fireTarget = GetFireTargetPosition(characterMeshComp, cameraComp);

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, "target pos = " + fireTarget.ToString());
		DrawDebugSphere(GetWorld(), fireTarget, 100, 12, FColor::Green, false, 0, 0U, 3);
	}
}

// Called to bind functionality to input
void AStrawberry::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AStrawberry::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AStrawberry::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("TurnRate"), this, &AStrawberry::TurnRate);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"), this, &AStrawberry::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AStrawberry::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AStrawberry::LookUp);

	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AStrawberry::Fire);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AStrawberry::FireReleased);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &AStrawberry::JumpPressed);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &AStrawberry::JumpReleased);

	PlayerInputComponent->BindAction(TEXT("Walk"), IE_Pressed, this, &AStrawberry::WalkPressed);
	PlayerInputComponent->BindAction(TEXT("Walk"), IE_Released, this, &AStrawberry::WalkReleased);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed, this, &AStrawberry::SprintPressed);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released, this, &AStrawberry::SprintReleased);

	PlayerInputComponent->BindAction(TEXT("Dash"), IE_Pressed, this, &AStrawberry::Dash);
}

void AStrawberry::Initialize(USceneComponent* _characterMeshComp, USceneComponent* _cameraComp, USceneComponent* _cherryMeshComp, USceneComponent* _dashFX)
{
	characterMeshComp = _characterMeshComp;
	cameraComp = _cameraComp;
	cherryMeshComp = _cherryMeshComp;
	dashFX = _dashFX;
	playerChar = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	charMovement = GetCharacterMovement();
	defaultWalkSpeed = GetCharacterMovement()->MaxWalkSpeed;
	defaultGravity = GetCharacterMovement()->GravityScale;
}

//Movement
void AStrawberry::MoveForward(float Value)
{
	if (isMovementLocked) return;
	FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);
	AddMovementInput(UKismetMathLibrary::GetForwardVector(rot), Value);
}

void AStrawberry::MoveRight(float Value)
{
	if (isMovementLocked) return;
	FRotator rot = FRotator(0, GetControlRotation().Yaw, 0);
	AddMovementInput(UKismetMathLibrary::GetRightVector(rot), Value);
}


//Turn/Look
void AStrawberry::TurnRate(float Value)
{
	AddControllerYawInput(Value * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStrawberry::LookUpRate(float Value)
{
	AddControllerPitchInput(Value * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStrawberry::Turn(float Value)
{
	AddControllerYawInput(Value * baseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AStrawberry::LookUp(float Value)
{
	AddControllerPitchInput(Value * baseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AStrawberry::JumpPressed()
{	
	if (!inWallSlide)
	{
		if (charMovement != NULL)
		{			
			charMovement->GravityScale = defaultGravity;
			
			Jump();
			
			if (JumpCurrentCount > 0 && CanJump())
			{
				PlayAnimMontage(animMontageDoubleJump, 1.0);
			}				
		}
	}
	else
	{
		canWallDash = true;
	}	
}

void AStrawberry::JumpReleased()
{
	StopJumping();
	canWallDash = false;
}

void AStrawberry::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	inWallSlide = false;
	charMovement->GravityScale = defaultGravity;
	didWallSlideVelo = false;
	didAirDash = false;
}

void AStrawberry::Fire()
{
	FireCherry(cherryMeshComp, fireTarget);
}

void AStrawberry::FireReleased()
{
	cherryFired = false;
}

void AStrawberry::WalkPressed()
{
	charMovement->MaxWalkSpeed = defaultWalkSpeed * 0.5;
}

void AStrawberry::WalkReleased()
{
	charMovement->MaxWalkSpeed = defaultWalkSpeed;
}

void AStrawberry::SprintPressed()
{
	charMovement->MaxWalkSpeed = defaultWalkSpeed * 2.5;
}

void AStrawberry::SprintReleased()
{
	charMovement->MaxWalkSpeed = defaultWalkSpeed;
}

void AStrawberry::Dash()
{
	if (!charMovement->IsFalling())
	{
		if (!didDash)
		{
			didDash = true;
			isDashing = true;
			dashFX->SetActive(true, true);
			charMovement->Velocity = GetActorForwardVector() * 10000;

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]
				{
					dashFX->SetActive(false, false);

					FTimerDelegate TimerDelegate;
					TimerDelegate.BindLambda([&]
						{							
							isDashing = false;
							didDash = false;
						});

					FTimerHandle TimerHandle;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.25, false);

				});

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.25, false);
		}		
	}
	else
	{
		if (!didAirDash)
		{
			didAirDash = true;
			isDashing = true;
			dashFX->SetActive(true, true);
			charMovement->GravityScale = 0.1;
			charMovement->Velocity = GetActorForwardVector() * 4000;

			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]
				{
					isDashing = false;
					dashFX->SetActive(false, false);
					charMovement->GravityScale = defaultGravity;
					charMovement->Velocity = GetActorForwardVector() * 100;
				});

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1, false);
		}
	}
}

void AStrawberry::WallDash()
{
	if (charMovement->IsFalling())
	{
		FHitResult result;
		bool isHit = false;

		isHit = GetWorld()->SweepSingleByChannel
		(
			result,
			GetActorLocation(),
			GetActorLocation(),
			FQuat::Identity,
			ECollisionChannel::ECC_Visibility,
			FCollisionShape::MakeSphere(50)	
		);

		if (isHit)
		{
			if (result.GetActor()->IsA(BlueprintClass_Wall))
			{
				inWallSlide = true;
				if (GetVelocity().Z <= 0)
				{
					if (!didWallSlideVelo)
					{
						didWallSlideVelo = true;
						charMovement->Velocity = FVector().ZeroVector;

						isMovementLocked = true;
						//charMovement->GravityScale = 0;
					}
					
					charMovement->GravityScale = 0.25;

					FTimerDelegate TimerDelegate1;
					TimerDelegate1.BindLambda([&]
					{
						
						isMovementLocked = false;
					});

					FTimerHandle TimerHandle1;
					GetWorld()->GetTimerManager().SetTimer(TimerHandle1, TimerDelegate1, 0.75, false);
					
					if (canWallDash)
					{
						if (!didWallDash)
						{
							didWallDash = true;

							FRotator rotNormal = FRotator(UKismetMathLibrary::MakeRotFromX(result.Normal).Pitch, UKismetMathLibrary::MakeRotFromX(result.Normal).Yaw, UKismetMathLibrary::MakeRotFromX(result.Normal).Roll);
							//SetActorRotation(rotNormal);

							dashFX->SetActive(true, true);
							
							FVector lauchVelo = FVector().UpVector * 1250;
							LaunchCharacter(lauchVelo, false, false);

							FTimerDelegate TimerDelegate;
							TimerDelegate.BindLambda([&]
								{
									FVector lauchVelo2 = (GetActorForwardVector()) * FVector(1000, 1000, 0);
									LaunchCharacter(-lauchVelo2, false, false);

									SetActorRotation((-lauchVelo2).Rotation());

									FTimerDelegate TimerDelegate;
									TimerDelegate.BindLambda([&]
										{											
											dashFX->SetActive(false, false);
											didWallDash = false;
										});

									FTimerHandle TimerHandle;
									GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.5, false);
								});

							FTimerHandle TimerHandle;
							GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1, false);
								
						}
					}
				}
			}
			else
			{
				inWallSlide = false;
				if (!isDashing)
				{
					charMovement->GravityScale = defaultGravity;
				}
			}
		}
		else
		{
			inWallSlide = false;
			if (!isDashing)
			{
				charMovement->GravityScale = defaultGravity;
			}
		}
	}
}

FVector AStrawberry::GetFireTargetPosition(USceneComponent* charMesh, USceneComponent* camera)
{
	FVector _fireTarget = FVector();

	FVector startVector = charMesh->USceneComponent::GetComponentLocation();
	startVector += FVector(0, 0, 200);

	FVector endVector = camera->USceneComponent::GetForwardVector();
	endVector += FVector(0, 0, 0.25);
	endVector = (endVector * 7000) + startVector;

	FHitResult hitresult;
	FCollisionQueryParams queryParams;
	
	queryParams.AddIgnoredActor(this);
	bool ifHit = GetWorld()->LineTraceSingleByChannel
	(
		hitresult, startVector, endVector, ECollisionChannel::ECC_Camera, queryParams
	);

	if (ifHit)
	{
		_fireTarget = hitresult.Location;
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, FString::Printf(TEXT("targeting: %s"), *hitresult.GetActor()->GetName()));
	}
	else
	{
		_fireTarget = hitresult.TraceEnd;
	}

	return _fireTarget;
}

void AStrawberry::FireCherry(USceneComponent* cherry, FVector target)
{
	if (!cherryFired)
	{
		cherryFired = true;
		FActorSpawnParameters spawnParams;
		spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		ACherry* cherrySpawned = Cast<ACherry>(GetWorld()->SpawnActor<AActor>(cherryActor,cherry->GetComponentTransform() , spawnParams));

		cherrySpawned->targetLocation = fireTarget;
		cherrySpawned->FireTarget();
		cherrySpawned->strawberry = this;

		/*
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "firing pos = " + fireTarget.ToString());
		}
		*/		
	}
}



