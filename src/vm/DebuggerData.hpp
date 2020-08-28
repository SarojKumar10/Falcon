#ifndef FALCON_DEBUGGER_DATA_HPP
#define FALCON_DEBUGGER_DATA_HPP

#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace Falcon
{
    struct DebuggerData
    {
        struct DebuggerFunction
        {
            std::string Signature;
            uint64_t StartLine;
            uint64_t EndLine;
            std::unordered_map<std::string, std::pair<std::string, uint64_t>> LocalVariables;
        };

        std::map<uint64_t, std::pair<uint64_t, std::string>> LineData;
        std::unordered_map<std::string, DebuggerFunction>    FunctionData;
    };
}

#endif
