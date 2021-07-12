#include <string>
#include <vector>

#include "main.hpp"
#include "WebVerified.hpp"
#include "FriendButton.hpp"

#include "monkecomputer/shared/GorillaUI.hpp"
#include "custom-types/shared/register.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "GorillaLocomotion/Player.hpp"
#include "GlobalNamespace/GorillaTagger.hpp"
#include "GlobalNamespace/GorillaScoreBoard.hpp"
#include "GlobalNamespace/GorillaPlayerLineButton.hpp"
#include "GlobalNamespace/GorillaPlayerScoreboardLine.hpp"
#include "GlobalNamespace/GorillaTriggerColliderHandIndicator.hpp"
#include "GlobalNamespace/VRRig.hpp"

#include "UnityEngine/Transform.hpp"
#include "UnityEngine/GameObject.hpp"
#include "UnityEngine/Collider.hpp"
#include "UnityEngine/Vector3.hpp"
#include "UnityEngine/PlayerPrefs.hpp"
#include "UnityEngine/Color.hpp"
#include "UnityEngine/UI/Text.hpp"

#include "Photon/Realtime/Player.hpp"
#include "Photon/Pun/PhotonNetwork.hpp"

static ModInfo modInfo; // Stores the ID and version of our mod, and is sent to the modloader upon startup

// Loads the config from disk using our modInfo, then returns it for use
Configuration& getConfig() {
    static Configuration config(modInfo);
    config.Load();
    return config;
}

// Returns a logger, useful for printing debug messages
Logger& getLogger() {
    static Logger* logger = new Logger(modInfo);
    return *logger;
}

// Called at the early stages of game loading
extern "C" void setup(ModInfo& info) {
    info.id = ID;
    info.version = VERSION;
    modInfo = info;
	
    getConfig().Load(); // Load the config file
    getLogger().info("Completed setup!");
}

GorillaFriends::WebVerified* webVerified;

MAKE_HOOK_OFFSETLESS(Player_start, void, GorillaLocomotion::Player* self)
{
    //GRAB LIST OF VERIFIED USER IDS
    getLogger().info("Making WebVerified Object");
    auto go = UnityEngine::GameObject::New_ctor(il2cpp_utils::createcsstr("WebVerified"));
    webVerified = go->AddComponent<GorillaFriends::WebVerified*>();
    webVerified->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(webVerified->GetVerifiedModders())));
    Player_start(self);
}

bool friendsInitialised = false;
UnityEngine::GameObject* m_pScoreboardFriendBtn = nullptr;
GorillaFriends::FriendButton* m_pFriendButtonController;


MAKE_HOOK_OFFSETLESS(GSB_Awake, void, GlobalNamespace::GorillaScoreBoard* self)
{
    if(m_pScoreboardFriendBtn != nullptr)
    {
		// Friend Button prefab has been created, skip our code
        GSB_Awake(self);
        return;
    }
	// I think we dont need it here:
    // GorillaFriends::WebVerified::m_listCurrentSessionFriends.clear();
    getLogger().info("GSB_AWAKE");
    auto transform = self->scoreBoardLinePrefab->get_transform();
    int transformNum = transform->get_childCount();
    for(int i = 0; i < transformNum; ++i)
    {
        getLogger().info("ooga");
        auto t = transform->GetChild(i);
        if(to_utf8(csstrtostr(t->get_name())) == "Mute Button")
        {
            m_pScoreboardFriendBtn = UnityEngine::GameObject::Instantiate(t->get_gameObject());
            if(m_pScoreboardFriendBtn != nullptr)
            {
                t->set_localPosition(UnityEngine::Vector3(3.8f, 0.0f, 0.0f));
                m_pScoreboardFriendBtn->get_transform()->set_parent(self->scoreBoardLinePrefab->get_transform());
                m_pScoreboardFriendBtn->get_transform()->set_name(il2cpp_utils::createcsstr("FriendButton"));
                m_pScoreboardFriendBtn->get_transform()->set_localPosition(UnityEngine::Vector3(17.5f, 0.0f, 0.0f));
                auto controller = m_pScoreboardFriendBtn->GetComponent<GlobalNamespace::GorillaPlayerLineButton*>();
                if(controller != nullptr)
                {
                    m_pFriendButtonController = m_pScoreboardFriendBtn->AddComponent<GorillaFriends::FriendButton*>();
                    m_pFriendButtonController->parentLine = controller->parentLine;
                    m_pFriendButtonController->offText = il2cpp_utils::createcsstr("ADD\nFRIEND");
                    m_pFriendButtonController->onText = il2cpp_utils::createcsstr("FRIEND!");
                    m_pFriendButtonController->myText = controller->myText;
                    m_pFriendButtonController->myText->set_text(m_pFriendButtonController->offText);
                    m_pFriendButtonController->offMaterial = controller->offMaterial;
					/* Maybe we need to instantiate onMaterial, please check */
                    m_pFriendButtonController->onMaterial = controller->onMaterial;
                    m_pFriendButtonController->onMaterial->set_color(UnityEngine::Color(0.8f, 0.5f, 0.9f, 1.0f));

                    UnityEngine::GameObject::Destroy(controller);
                }
            }
			// We're added a Friend Button! Break our "for loop"
			break;
        }
    }
	// Call original function
    GSB_Awake(self);
}

MAKE_HOOK_OFFSETLESS(PN_Disconnect, void, Photon::Pun::PhotonNetwork* self)
{
    GorillaFriends::WebVerified::m_listCurrentSessionFriends.clear();
    PN_Disconnect(self);
}

// Called later on in the game loading - a good time to install function hooks
extern "C" void load() {
    il2cpp_functions::Init();
    GorillaUI::Innit(); //bruv

    custom_types::Register::RegisterType<GorillaFriends::WebVerified>();
    custom_types::Register::RegisterType<GorillaFriends::FriendButton>();

    getLogger().info("Installing hookeroos");
    INSTALL_HOOK_OFFSETLESS(getLogger(), Player_start, il2cpp_utils::FindMethodUnsafe("GorillaLocomotion", "Player", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), GSB_Awake, il2cpp_utils::FindMethodUnsafe("", "GorillaScoreBoard", "Awake", 0));
    INSTALL_HOOK_OFFSETLESS(getLogger(), PN_Disconnect, il2cpp_utils::FindMethodUnsafe("Photon.Pun", "PhotonNetwork", "Disconnect", 0));
    getLogger().info("Installed all hooks!");
}