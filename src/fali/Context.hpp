#ifndef FALCON_FALI_CONTEXT_HPP
#define FALCON_FALI_CONTEXT_HPP

#include "common/Common.hpp"

#include "vm/VM.hpp"
#include "vm/Debugger.hpp"

#include "fali/Object.hpp"

namespace Falcon
{
    namespace FALI
    {
        class Context
        {
        public:
            Context(uint8_t * code);

            template <typename ReturnType, typename... Args>
            void AddExternalFunction(const std::string & name, std::function<ReturnType(Args...)> function)
            {
                auto wrappedFunction = [function](VM & vm)
                {
                    if constexpr (sizeof...(Args))
                    {
                        std::tuple<Args...> args = std::make_tuple(*(Args *)vm.Pop(sizeof(Args))...);

                        if constexpr (!std::is_void_v<ReturnType>)
                        {
                            ReturnType result = std::apply(function, args);

                            vm.Push((uint8_t *)&result, sizeof(ReturnType));
                        }
                        else
                        {
                            std::apply(function, args);
                        }
                    }
                    else
                    {
                        if constexpr (!std::is_void_v<ReturnType>)
                        {
                            ReturnType result = function();

                            vm.Push((uint8_t *)&result, sizeof(ReturnType));
                        }
                        else
                        {
                            function();
                        }
                    }
                };

                m_VM.ExternalFunction(name, wrappedFunction);
            }

            template <typename ReturnType, typename... Args>
            ReturnType Call(const std::string & name, Args... args)
            {
                (m_VM.Push((uint8_t *)&args, sizeof(args)), ...);

                uint64_t argsSize = 0;

                ((argsSize += sizeof(args)), ...);

                m_VM.Run(name, argsSize);

                if constexpr (!std::is_void_v<ReturnType>)
                {
                    return *(ReturnType *)m_VM.Pop(sizeof(ReturnType));
                }
            }

            constexpr VM & GetVM() { return m_VM; }

        private:
            VM m_VM;
        };
    }
}

#endif
