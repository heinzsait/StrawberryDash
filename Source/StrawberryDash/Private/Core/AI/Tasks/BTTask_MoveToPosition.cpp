// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/BTTask_MoveToPosition.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToPosition::UBTTask_MoveToPosition()
{
	NodeName = TEXT("Move to position");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_MoveToPosition, BlackboardKey));
	
}

EBTNodeResult::Type UBTTask_MoveToPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	AIPawn = AIController->GetPawn();

	FVector pos = AIController->GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);

	//FAIMoveRequest move = FAIMoveRequest(pos);
	//AIController->MoveTo(move);

	auto moveResult = AIController->MoveToLocation(pos, -1, false, false);
	if (moveResult == EPathFollowingRequestResult::Failed)
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, TEXT("Failed"));
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_MoveToPosition::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}