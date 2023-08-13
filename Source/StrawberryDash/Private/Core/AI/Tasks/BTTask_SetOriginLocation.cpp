// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/BTTask_SetOriginLocation.h"

#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_SetOriginLocation::UBTTask_SetOriginLocation()
{
	NodeName = TEXT("Set Origin Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SetOriginLocation, BlackboardKey));
}

EBTNodeResult::Type UBTTask_SetOriginLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	const APawn* AIPawn = AIController->GetPawn();

	FVector Origin = AIPawn->GetActorLocation();

	AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Origin);

	//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Origin = " + Origin.ToString());

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_SetOriginLocation::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}