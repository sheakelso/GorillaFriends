#include <string>
#include <vector>

#include "WebVerified.hpp"

#include "custom-types/shared/macros.hpp"
#include "UnityEngine/MonoBehaviour.hpp"
#include "UnityEngine/UI/Text.hpp"
#include "UnityEngine/Material.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/MeshRenderer.hpp"
#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"

DECLARE_CLASS_CODEGEN(GorillaFriends, FriendButton, UnityEngine::MonoBehaviour,
    DECLARE_INSTANCE_FIELD_DEFAULT(GlobalNamespace::GorillaPlayerScoreboardLine*, parentLine, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, isOn, false);
    DECLARE_INSTANCE_FIELD(Il2CppString*, offText);
    DECLARE_INSTANCE_FIELD(Il2CppString*, onText);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::UI::Text*, myText, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(float, touchTime, 0.0f);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, offMaterial);
    DECLARE_INSTANCE_FIELD(UnityEngine::Material*, onMaterial);
    DECLARE_INSTANCE_FIELD_DEFAULT(UnityEngine::MeshRenderer*, meshRenderer, nullptr);
    DECLARE_INSTANCE_FIELD_DEFAULT(bool, initialized, false);
    DECLARE_INSTANCE_FIELD_DEFAULT(float, nextUpdate, 0.0f);
    
    DECLARE_METHOD(void, Start);
    DECLARE_METHOD(void, Update);
    DECLARE_METHOD(void, UpdateColor);
    DECLARE_METHOD(void, OnTriggerEnter, UnityEngine::Collider* collider);

    REGISTER_FUNCTION(
        REGISTER_FIELD(parentLine);
        REGISTER_FIELD(isOn);
        REGISTER_FIELD(offText);
        REGISTER_FIELD(onText);
        REGISTER_FIELD(myText);
        REGISTER_FIELD(touchTime);
        REGISTER_FIELD(offMaterial);
        REGISTER_FIELD(onMaterial);
        REGISTER_FIELD(meshRenderer);
        REGISTER_FIELD(initialized);
        REGISTER_FIELD(nextUpdate);

        REGISTER_METHOD(Start);
        REGISTER_METHOD(Update);
        REGISTER_METHOD(UpdateColor);
        REGISTER_METHOD(OnTriggerEnter);
    )
)