// Fill out your copyright notice in the Description page of Project Settings.

#include "MeshMergeFunctionLibrary.h"
#include "SkeletalMeshMerge.h"
#include "Engine/SkeletalMesh.h"
#include "Animation/Skeleton.h"
#include "Customization/CustomizationElementaryAsset.h"


TArray<UCustomizationElementaryAsset*> FCustomizationElementaryAssetsSelectionSet::SelectBestAsset(
	const FGameplayTagContainer& CosmeticTags) const
{
	for (const FCustomizationElementaryAssetsSelectionEntry& Rule : CeaRules)
	{
		if ((!Rule.Assets.IsEmpty()) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Assets;
		}
	}

	return DefaultAssets;
}

TArray<UCustomizationMaterialAsset*> FCustomizationMaterialAssetsSelectionSet::SelectBestAsset(
	const FGameplayTagContainer& CosmeticTags) const
{
	for (const FCustomizationMaterialAssetsSelectionEntry& Rule : CmaRules)
	{
		if ((!Rule.Assets.IsEmpty()) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Assets;
		}
	}

	return DefaultAssets;
}

TArray<UCustomizationAttachmentAsset*> FCustomizationAttachmentAssetsSelectionSet::SelectBestAsset(
	const FGameplayTagContainer& CosmeticTags) const
{
	for (const FCustomizationAttachmentAssetsSelectionEntry& Rule : CaaRules)
	{
		if ((!Rule.Assets.IsEmpty()) && CosmeticTags.HasAll(Rule.RequiredTags))
		{
			return Rule.Assets;
		}
	}

	return DefaultAssets;
}

USkeletalMesh* UMeshMergeFunctionLibrary::MergeMeshes(const FSkeletalMeshMergeParams& Params)
{
	TArray<USkeletalMesh*> MeshesToMergeCopy = Params.MeshesToMerge;
	MeshesToMergeCopy.RemoveAll([](const USkeletalMesh* InMesh)
	{
		return InMesh == nullptr;
	});
	if (MeshesToMergeCopy.Num() <= 1)
	{
		UE_LOG(LogTemp, Warning, TEXT("Must provide multiple valid Skeletal Meshes in order to perform a merge."));
		return nullptr;
	}
	const EMeshBufferAccess BufferAccess = Params.bNeedsCpuAccess
		                                       ? EMeshBufferAccess::ForceCPUAndGPU
		                                       : EMeshBufferAccess::Default;
	const TArray<FSkelMeshMergeSectionMapping> SectionMappings;

	USkeletalMesh* BaseMesh = NewObject<USkeletalMesh>();
	if (Params.Skeleton && Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
	}

	FSkeletalMeshMerge Merger(BaseMesh, MeshesToMergeCopy, SectionMappings, Params.StripTopLODS, BufferAccess);
	if (!Merger.DoMerge())
	{
		UE_LOG(LogTemp, Warning, TEXT("Merge failed!"));
		return nullptr;
	}
	if (Params.Skeleton && !Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
	}
	return BaseMesh;
}

TArray<UCustomizationElementaryAsset*> UMeshMergeFunctionLibrary::MergeCustomizationElementaryAssets(
	TArray<UCustomizationElementaryAsset*> Assets1, TArray<UCustomizationElementaryAsset*> Assets2)
{
	TMap<FName, UCustomizationElementaryAsset*> ResultMap;
	TArray<UCustomizationElementaryAsset*> ResultArray;

	for (UCustomizationElementaryAsset* Asset : Assets1)
	{
		if (!Asset)
		{
			continue;
		}
		ResultMap.Add(Asset->ModuleName, Asset);
	}

	for (UCustomizationElementaryAsset* Asset : Assets2)
	{
		if (!Asset)
		{
			continue;
		}
		ResultMap.Add(Asset->ModuleName, Asset);
	}

	ResultMap.GenerateValueArray(ResultArray);

	return ResultArray;
}

TArray<UCustomizationMaterialAsset*> UMeshMergeFunctionLibrary::MergeCustomizationMaterialAssets(
	TArray<UCustomizationMaterialAsset*> Materials1, TArray<UCustomizationMaterialAsset*> Materials2)
{
	TMap<FName, UCustomizationMaterialAsset*> ResultMap;
	TArray<UCustomizationMaterialAsset*> ResultArray;

	for (UCustomizationMaterialAsset* Material : Materials1)
	{
		if (!Material)
		{
			continue;
		}
		ResultMap.Add(Material->MaterialNamespace, Material);
	}

	for (UCustomizationMaterialAsset* Material : Materials2)
	{
		if (!Material)
		{
			continue;
		}
		ResultMap.Add(Material->MaterialNamespace, Material);
	}

	ResultMap.GenerateValueArray(ResultArray);

	return ResultArray;
}

TArray<UCustomizationElementaryAsset*> UMeshMergeFunctionLibrary::SelectBestCeaArray(
	const FCustomizationElementaryAssetsSelectionSet Set, FGameplayTagContainer CosmeticTags)
{
	return Set.SelectBestAsset(CosmeticTags);
}

TArray<UCustomizationMaterialAsset*> UMeshMergeFunctionLibrary::SelectBestCmaArray(
	const FCustomizationMaterialAssetsSelectionSet Set, FGameplayTagContainer CosmeticTags)
{
	return Set.SelectBestAsset(CosmeticTags);
}

TArray<UCustomizationAttachmentAsset*> UMeshMergeFunctionLibrary::SelectBestCaaArray(
	const FCustomizationAttachmentAssetsSelectionSet Set, FGameplayTagContainer CosmeticTags)
{
	return Set.SelectBestAsset(CosmeticTags);
}
