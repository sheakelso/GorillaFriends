#include <string>
#include <vector>
#include <sstream>

#include "WebVerified.hpp"
#include "System/Collections/IEnumerator.hpp"
#include "custom-types/shared/coroutine.hpp"

#include "UnityEngine/Networking/UnityWebRequest.hpp"
#include "UnityEngine/Networking/DownloadHandler.hpp"

#include "System/Text/Encoding.hpp"

#include "beatsaber-hook/shared/utils/il2cpp-utils.hpp"

DEFINE_TYPE(GorillaFriends::WebVerified);

void GorillaFriends::WebVerified::Start()
{
    il2cpp_utils::getLogger().info("Started coroutine...");
    this->StartCoroutine(reinterpret_cast<System::Collections::IEnumerator*>(custom_types::Helpers::CoroutineHelper::New(GetVerifiedModders())));
}

custom_types::Helpers::Coroutine GorillaFriends::WebVerified::GetVerifiedModders()
{
    auto webRequest = UnityEngine::Networking::UnityWebRequest::Get(il2cpp_utils::createcsstr("https://github.com/RusJJ/GorillaFriends/raw/main/gorillas.verified"));
    co_yield reinterpret_cast<System::Collections::IEnumerator*>(webRequest->SendWebRequest());
    auto data = webRequest->get_downloadHandler()->get_data();
    auto responsecs = System::Text::Encoding::get_Default()->GetString(data);
    auto response = to_utf8(csstrtostr(responsecs));
    il2cpp_utils::getLogger().info(response);

    std::istringstream instring(response);
    std::string line;
    while(std::getline(instring, line))
    {
        verifiedUserIds.push_back(line);
    }

    il2cpp_utils::getLogger().info("Ended coroutine.");
    co_return;
}