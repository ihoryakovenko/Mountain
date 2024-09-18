#include "DialogueAsset.h"
#include "UObject/ObjectSaveContext.h"

void UDialogueAsset::PreSave(FObjectPreSaveContext saveContext)
{ 
    if (_onPreSaveListener) {
        _onPreSaveListener();
    }
}
