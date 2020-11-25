#include "scpch.h"
#include "ImGuiConsole.h"

#include "imgui.h"

#include "examples/imgui_impl_win32.h"
#include "examples/imgui_impl_dx11.h"

namespace Seacrest {

    uint16_t ImGuiConsole::s_MessageBufferCapacity = 200;
    std::vector<std::shared_ptr<ImGuiConsole::Message>> ImGuiConsole::s_MessageBuffer( s_MessageBufferCapacity );
    uint16_t ImGuiConsole::s_MessageBufferIndex = 0;

    std::unordered_map<ImGuiConsole::Message::Level, ImGuiConsole::Color> ImGuiConsole::s_RenderColors = {
        {ImGuiConsole::Message::Level::Trace   , {1.0f, 1.0f, 1.0f, 1.0f}}, // White
        {ImGuiConsole::Message::Level::Info    , {0.0f, 1.0f, 0.0f, 1.0f}}, // Green
        {ImGuiConsole::Message::Level::Debug   , {0.0f, 1.0f, 1.0f, 1.0f}}, // Cyan
        {ImGuiConsole::Message::Level::Warn    , {1.0f, 1.0f, 0.0f, 1.0f}}, // Yellow
        {ImGuiConsole::Message::Level::Error   , {1.0f, 0.0f, 0.0f, 1.0f}}, // Red
        {ImGuiConsole::Message::Level::Critical, {1.0f, 0.0f, 1.0f, 1.0f}}  // Magenta
    };

    std::shared_ptr<ImGuiConsole> ImGuiConsole::GetConsole()
    {
        return std::dynamic_pointer_cast<ImGuiConsole>( Log::GetSinks()[1] );
    }

    void ImGuiConsole::AddMessage( std::shared_ptr<Message> message )
    {
        if (message->m_Level == Message::Level::Invalid)
            return;

        *( s_MessageBuffer.begin() + s_MessageBufferIndex ) = message;
        if (++s_MessageBufferIndex == s_MessageBufferCapacity)
            s_MessageBufferIndex = 0;
    }

    void ImGuiConsole::OnImGuiRender( bool* show )
    {
        ImGui::SetNextWindowSize( ImVec2( 640, 480 ), ImGuiCond_FirstUseEver );
        ImGui::Begin( "Console", show );
        {
            auto messageStart = s_MessageBuffer.begin() + s_MessageBufferIndex;
            if (*messageStart) // If contains old message here
                for (auto message = messageStart; message != s_MessageBuffer.end(); message++)
                    ( *message )->OnImGuiRender();
            if (s_MessageBufferIndex != 0) // Skipped first messages in vector
                for (auto message = s_MessageBuffer.begin(); message != messageStart; message++)
                    ( *message )->OnImGuiRender();
        }
        ImGui::End();
    }

    ImGuiConsole::Message::Message( const std::string message, Level level )
        : m_Message( message ), m_Level( level )
    {
    }

    void ImGuiConsole::Message::OnImGuiRender()
    {
        if (!Valid()) return;

        Color color = s_RenderColors[m_Level];
        ImGui::PushStyleColor( ImGuiCol_Text, { color.r, color.g, color.b, color.a } );
        ImGui::TextUnformatted( m_Message.c_str() );
        ImGui::PopStyleColor();
    }
}