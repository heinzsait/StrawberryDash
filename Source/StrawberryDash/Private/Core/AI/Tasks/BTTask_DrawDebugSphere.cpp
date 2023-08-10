// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/AI/Tasks/BTTask_DrawDebugSphere.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DrawDebugSphere::UBTTask_DrawDebugSphere()
{
	NodeName = TEXT("Draw Debug Sphere");

	BlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_DrawDebugSphere, BlackboardKey));
}

EBTNodeResult::Type UBTTask_DrawDebugSphere::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();

	FVector pos = AIController->GetBlackboardComponent()->GetValueAsVector(BlackboardKey.SelectedKeyName);

	DrawDebugSphere(GetWorld(), pos, SphereRadius, 12, FColor::Blue, false, 3, 0U, 3);
		
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

FString UBTTask_DrawDebugSphere::GetStaticDescription() const
{
	return FString::Printf(TEXT("Vector : %s"), *BlackboardKey.SelectedKeyName.ToString());
}