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
            // log_msg is a struct containing the log entry info like level, timestamp, thread id etc.
            // msg.raw contains pre formatted log

            // If needed (very likely but not mandatory), the sink formats the message before sending it to its final destination:
            spdlog::memory_buf_t formatted;
            // fmt::memory_buffer formatted;
            formatter_->format( msg, formatted );
            std::cout << fmt::to_string( formatted );

            AddMessage( { fmt::to_string( formatted ).c_str(), Message::Level::Info } );
        }

        void flush_() override
        {
            std::cout << std::flush;
        }
    };

}

#include "spdlog/details/null_mutex.h"
#include <mutex>
namespace Seacrest {
    using ImGuiLogSink_mt = ImGuiLogSink<std::mutex>;                  // multi-threaded
    using ImGuiLogSink_st = ImGuiLogSink<spdlog::details::null_mutex>; // single threaded
}