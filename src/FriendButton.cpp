#include "UnityEngine/MeshRenderer.hpp"
#include "UnityEngine/Time.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/GameObject.hpp"

#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "Photon/Realtime/Player.hpp"

#include "FriendButton.hpp"
#include "WebVerified.hpp"

DEFINE_TYPE(GorillaFriends::FriendButton);

UnityEngine::Color m_clrFriendColor = UnityEngine::Color(0.8f, 0.5f, 0.9f, 1.0f);
UnityEngine::Color m_clrVerifiedColor = UnityEngine::Color(0.5f, 1.0f, 0.5f, 1.0f);

static bool IsVerified(std::string userID)
{
    il2cpp_utils::getLogger().info("Checking if player is verified");
    int listsize = GorillaFriends::WebVerified::verifiedUserIds.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::verifiedUserIds[i] == userID) return true;
    }
    il2cpp_utils::getLogger().info("Player was not verified");
    return false;
}

static bool IsFriend(std::string userID)
{
    return (UnityEngine::PlayerPrefs::GetInt(il2cpp_utils::createcsstr(userID + "_friend"), 0) != 0);
}

static bool IsInFriendList(std::string userID)
{
    int listsize = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[i] == userID) return true;
    }
    return false;
}


void GorillaFriends::FriendButton::Start()
{
    meshRenderer = get_gameObject()->GetComponent<UnityEngine::MeshRenderer*>();
}

void GorillaFriends::FriendButton::Update()
{
    il2cpp_utils::getLogger().info("Checking whether to run or not");
    if(nextUpdate > UnityEngine::Time::get_time() || parentLine->playerVRRig == nullptr || parentLine->linePlayer == nullptr) return;
    nextUpdate = UnityEngine::Time::get_time() + 0.5f;

    if(!initialized)
    {
        il2cpp_utils::getLogger().info("Wasn't initialized");
        initialized = true;
        if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
        {
            il2cpp_utils::getLogger().info("Was verified");
            parentLine->playerName->set_color(m_clrVerifiedColor);
            parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
        }

        if(parentLine->linePlayer->IsLocal) get_gameObject()->SetActive(false);
        else
        {
            il2cpp_utils::getLogger().info("Checking if player is a freind");
            if(IsFriend(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
            {
                il2cpp_utils::getLogger().info("Was friend");
                if(!IsInFriendList(to_utf8(csstrtostr(parentLine->linePlayer->UserId)))) GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(to_utf8(csstrtostr(parentLine->linePlayer->UserId)));
                parentLine->playerName->set_color(m_clrFriendColor);
                isOn = true;
                //myText->set_text(onText);
                UpdateColor();
            }
            else
            {
                il2cpp_utils::getLogger().info("Wasn't friend");
            }
        }
        return;
    }

    if(!parentLine->linePlayer->IsLocal && isOn != IsInFriendList(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
    {
        isOn = !isOn;
        UpdateColor();
        if(!isOn)
        {
            if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
            {
                parentLine->playerName->set_color(m_clrVerifiedColor);
                parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
            }
            else
            {
                parentLine->playerName->set_color(UnityEngine::Color::get_white());
                parentLine->playerVRRig->playerText->set_color(UnityEngine::Color::get_white());
            }
            //myText->set_text(offText);
        }
        else
        {
            parentLine->playerName->set_color(m_clrFriendColor);
            parentLine->playerVRRig->playerText->set_color(m_clrFriendColor);
            //myText->set_text(onText);
        }
    }
}

void GorillaFriends::FriendButton::OnTriggerEnter(UnityEngine::Collider* collider)
{
    if(touchTime > UnityEngine::Time::get_time()) return;
    touchTime = UnityEngine::Time::get_time() + 0.25;
    GlobalNamespace::GorillaTriggerColliderHandIndicator* component = collider->GetComponent<GlobalNamespace::GorillaTriggerColliderHandIndicator*>();
    if(component == nullptr) return;

    isOn = !isOn;
    UpdateColor();
    GlobalNamespace::GorillaTagger::get_Instance()->StartVibration(component->isLeftHand, GlobalNamespace::GorillaTagger::get_Instance()->tapHapticStrength / 2.0f, GlobalNamespace::GorillaTagger::get_Instance()->tapHapticDuration);

    if(isOn)
    {
        GorillaFriends::WebVerified::m_listCurrentSessionFriends.push_back(to_utf8(csstrtostr(parentLine->linePlayer->UserId)));
        UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(to_utf8(csstrtostr(parentLine->linePlayer->UserId)) + "_friend"), 1);
        parentLine->playerName->set_color(m_clrFriendColor);
        //myText->set_text(onText);
        return;
    }

    /* Remove from list */
    auto it = GorillaFriends::WebVerified::m_listCurrentSessionFriends.begin();
    auto end = GorillaFriends::WebVerified::m_listCurrentSessionFriends.end();
    while( it != end )
    {
        if(*it == to_utf8(csstrtostr(parentLine->linePlayer->UserId)))
        {
            GorillaFriends::WebVerified::m_listCurrentSessionFriends.erase(it);
            break;
        }
        ++it;
    }
    /*int listsize = GorillaFriends::WebVerified::m_listCurrentSessionFriends.size();
    for(int i = 0; i < listsize; ++i)
    {
        if(GorillaFriends::WebVerified::m_listCurrentSessionFriends[i] == to_utf8(csstrtostr(parentLine->linePlayer->UserId)))
        {
            GorillaFriends::WebVerified::m_listCurrentSessionFriends.erase(GorillaFriends::WebVerified::m_listCurrentSessionFriends.begin() + i - 1);
        }
    }*/
    /* Remove from list */

    UnityEngine::PlayerPrefs::SetInt(il2cpp_utils::createcsstr(to_utf8(csstrtostr(parentLine->linePlayer->UserId)) + "_friend"), 0);
    //myText->set_text(offText);

    if(IsVerified(to_utf8(csstrtostr(parentLine->linePlayer->UserId))))
    {
        parentLine->playerName->set_color(m_clrVerifiedColor);
        parentLine->playerVRRig->playerText->set_color(m_clrVerifiedColor);
    }
    else
    {
        parentLine->playerName->set_color(UnityEngine::Color::get_white());
        parentLine->playerVRRig->playerText->set_color(UnityEngine::Color::get_white());
    }
}

void GorillaFriends::FriendButton::UpdateColor()
{
    if(isOn)
    {
        if(meshRenderer != nullptr) meshRenderer->set_material(onMaterial);
        myText->set_text(onText);
    }
    else
    {
        if(meshRenderer != nullptr) meshRenderer->set_material(offMaterial);
        myText->set_text(offText);
    }
}