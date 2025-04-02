// DialogueParser.cpp
#include "Dialogue System/DialogueParser.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"

FDialogueParser::FDialogueParser()
{
}

TMap<int32, FDialogueNode> FDialogueParser::ParseDialogueJson(const FString &JsonString, bool &Success)
{
    TMap<int32, FDialogueNode> Dialogue;
    Success = false;

    TSharedPtr<FJsonValue> JsonValue;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);

    if (FJsonSerializer::Deserialize(Reader, JsonValue) && JsonValue.IsValid())
    {
        TArray<TSharedPtr<FJsonValue>> JsonArray = JsonValue->AsArray();

        for (const TSharedPtr<FJsonValue> &ElementValue : JsonArray)
        {
            TSharedPtr<FJsonObject> ElementObject = ElementValue->AsObject();
            if (!ElementObject.IsValid())
            {
                continue;
            }

            FDialogueNode Element;

            // Parse basic properties
            Element.ID = ElementObject->GetIntegerField(TEXT("id"));
            Element.Next_Node_ID = ElementObject->GetIntegerField(TEXT("nextNodeId"));
            Element.Node_Type = ElementObject->GetIntegerField(TEXT("nodeType"));
            Element.Text = ElementObject->GetStringField(TEXT("text"));

            // Parse responses array
            TArray<TSharedPtr<FJsonValue>> ResponsesArray = ElementObject->GetArrayField(TEXT("responses"));
            for (const TSharedPtr<FJsonValue> &ResponseValue : ResponsesArray)
            {
                Element.Responses.Add(ResponseValue->AsNumber());
            }

            // Parse callbacks array
            TArray<TSharedPtr<FJsonValue>> CallbacksArray = ElementObject->GetArrayField(TEXT("selected_callbacks"));
            for (const TSharedPtr<FJsonValue> &CallbackValue : CallbacksArray)
            {
                Element.Selected_Callbacks.Add(CallbackValue->AsString());
            }
            
            Dialogue.Add(Element.ID, Element);
        }

        Success = true;
    }

    return Dialogue;
}

TMap<int32, FDialogueNode> FDialogueParser::LoadDialogueFromFile(const FString &FilePath, bool &Success)
{
    FString JsonString;
    Success = false;

    if (FFileHelper::LoadFileToString(JsonString, *FilePath))
    {
        return ParseDialogueJson(JsonString, Success);
    }

    return TMap<int32, FDialogueNode>();
}