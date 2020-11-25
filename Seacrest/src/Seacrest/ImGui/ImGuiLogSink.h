#include "spdlog/sinks/base_sink.h"
#include "ImGuiConsole.h"

namespace Seacrest {

    template<class Mutex>
    class ImGuiLogSink : public spdlog::sinks::base_sink<std::mutex>,
        public ImGuiConsole
    {
    public:
        explicit ImGuiLogSink() {}
        ImGuiLogSink( const ImGuiLogSink& ) = delete;
        ImGuiLogSink& operator=( const ImGuiLogSink& ) = delete;
        virtual ~ImGuiLogSink() = default;
    protected:
        void sink_it_( const spdlog::details::log_msg& msg ) override
        {
            spdlog::memory_buf_t formatted;
            // fmt::memory_buffer formatted;
            formatter_->format( msg, formatted );

            AddMessage( std::make_shared<Message>( fmt::to_string( formatted ), GetMessageLevel( msg.level ) ) );
        }

        void flush_() override
        {
        }
    private:
        static Message::Level GetMessageLevel( const spdlog::level::level_enum level )
        {
            switch (level)
            {
                case spdlog::level::level_enum::trace:    return Message::Level::Trace;
                case spdlog::level::level_enum::debug:    return Message::Level::Debug;
                case spdlog::level::level_enum::info:     return Message::Level::Info;
                case spdlog::level::level_enum::warn:     return Message::Level::Warn;
                case spdlog::level::level_enum::err:      return Message::Level::Error;
                case spdlog::level::level_enum::critical: return Message::Level::Critical;
                case spdlog::level::level_enum::off:      return Message::Level::Off;
            }
            return Message::Level::Invalid;
        }
    };

}

#include "spdlog/details/null_mutex.h"
#include <mutex>
namespace Seacrest {
    using ImGuiLogSink_mt = ImGuiLogSink<std::mutex>;                  // multi-threaded
    using ImGuiLogSink_st = ImGuiLogSink<spdlog::details::null_mutex>; // single threaded
}