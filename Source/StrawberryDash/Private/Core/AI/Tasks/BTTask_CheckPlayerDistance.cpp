// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/BTTask_CheckPlayerDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_CheckPlayerDistance::UBTTask_CheckPlayerDistance()
{
	NodeName = TEXT("Check Player Distance");
	//player = GetWorld()->GetFirstPlayerController()->GetPawn();
}

EBTNodeResult::Type UBTTask_CheckPlayerDistance::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AIController = OwnerComp.GetAIOwner();
	AIPawn = AIController->GetPawn();

	if (player == NULL)
		player = GetWorld()->GetFirstPlayerController()->GetPawn();
	bool isClose = false;
	if (player)
	{
		float dist = FVector::Distance(AIPawn->GetActorLocation(), player->GetActorLocation());

		if (dist < SearchRadius)
		{
			//if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1, FColor::Red, "Movin twords player");
			AIController->GetBlackboardComponent()->SetValueAsObject(BlackboardKey.SelectedKeyName, player);
			isClose = true;
		}
	}

	if (isClose)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return EBTNodeResult::Failed;
	}
}

FString UBTTask_CheckPlayerDistance::GetStaticDescription() const
{
	return FString::Printf(TEXT("Check Player Distance"));
}
