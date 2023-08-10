// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/DemonBTTask_FindRandomPoints.h"

#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UDemonBTTask_FindRandomPoints::UDemonBTTask_FindRandomPoints()
{
	NodeName = TEXT("Find Random Location");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UDemonBTTask_FindRandomPoints, BlackboardKey));
	Origin = FVector().ZeroVector;
}

EBTNodeResult::Type UDemonBTTask_FindRandomPoints::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FNavLocation Location{};
	AIController = OwnerComp.GetAIOwner();
	AIPawn = AIController->GetPawn();

	if(Origin == FVector().ZeroVector)
		Origin = AIPawn->GetActorLocation();

	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (IsValid(NavSystem) && NavSystem->GetRandomPointInNavigableRadius(Origin, SearchRadius, Location))
	{
		AIController->GetBlackboardComponent()->SetValueAsVector(BlackboardKey.SelectedKeyName, Location.Location);
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Movin to:" + Location.Location.ToString());
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UDemonBTTask_FindRandomPoints::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}
