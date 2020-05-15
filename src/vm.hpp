#ifndef FALCON_VM_HPP
#define FALCON_VM_HPP

#include <iostream>
#include <unordered_map>
#include <vector>

#include <cassert>
#include <cstdint>
#include <cstring>

namespace Falcon
{
    enum InstructionType : uint8_t
    {
        INSTRUCTION_ADD,
        INSTRUCTION_SUB,
        INSTRUCTION_MUL,
        INSTRUCTION_DIV,
        INSTRUCTION_MOD,
        INSTRUCTION_INC,
        INSTRUCTION_DEC,

        INSTRUCTION_LSHFT,
        INSTRUCTION_RSHFT,
        INSTRUCTION_AND,
        INSTRUCTION_OR,
        INSTRUCTION_XOR,
        INSTRUCTION_CMPL,

        INSTRUCTION_IF,
        INSTRUCTION_ELSE,
        INSTRUCTION_GRT0,
        INSTRUCTION_GRT1,
        INSTRUCTION_GREQ0,
        INSTRUCTION_GREQ1,
        INSTRUCTION_LESS0,
        INSTRUCTION_LESS1,
        INSTRUCTION_LSEQ0,
        INSTRUCTION_LSEQ1,
        INSTRUCTION_ISEQ0,
        INSTRUCTION_ISEQ1,
        INSTRUCTION_NEQ0,
        INSTRUCTION_NEQ1,
        INSTRUCTION_NOT0,
        INSTRUCTION_NOT1,
        INSTRUCTION_CAND,
        INSTRUCTION_COR,

        INSTRUCTION_PUSHC,
        INSTRUCTION_PUSHU,
        INSTRUCTION_PUSHL,
        INSTRUCTION_PUSHF,
        INSTRUCTION_POP,
        INSTRUCTION_MOVC,
        INSTRUCTION_MOVU,
        INSTRUCTION_MOVL,
        INSTRUCTION_MOVF,
        INSTRUCTION_CALL,
        INSTRUCTION_JMP,
        INSTRUCTION_SYMBOL,
        INSTRUCTION_EXTERN,
        INSTRUCTION_START,
        INSTRUCTION_LABEL,
        INSTRUCTION_END
    };

    enum RegisterType : uint8_t
    {
        REGISTER_C0,
        REGISTER_C1,
        REGISTER_C2,
        REGISTER_C3,
        REGISTER_U0,
        REGISTER_U1,
        REGISTER_U2,
        REGISTER_U3,
        REGISTER_L0,
        REGISTER_L1,
        REGISTER_L2,
        REGISTER_L3,
        REGISTER_F0,
        REGISTER_F1,
        REGISTER_F2,
        REGISTER_F3,
        REGISTER_USP,
        REGISTER_LSP,
        REGISTER_FSP,
        REGISTER_NULL
    };

    namespace Internal
    {
        struct Instruction
        {
            InstructionType type        : 6;
            RegisterType    arg1        : 5;
            uint8_t         arg1_offset    ;
            RegisterType    arg2        : 5;
            union
            {
                uint8_t     arg2_offset;
                uint64_t    value;
            } extra;
            std::string     symbol;
        };

        struct Register
        {
            union Value
            {
                char        c;
                uint64_t    u;
                int64_t     l;
                double    f;
            };
            
            RegisterType    type;
            Value           value;
        };
    }

    class VM
    {
        private:
            /*
             * Flags
             */
            bool    advanceOnly;
            bool    jmpStart;
            bool    jmpEnd;
            bool    cmpResult[2];

            /*
             * Bytecode related
             */
            std::vector<Internal::Instruction>  instructions;
            Internal::Instruction               currenInstruction;
            uint64_t                            instructionPtr;

            /*
             * Registers
             */
            Internal::Register registers[20];

            /*
             * Stack
             */
            uint8_t     stack[128];
            uint64_t    stack_pointer;

            /*
             * Operators
             */
            void( *     operators[47])(VM &);

            /*
             * Symbols:
             *
             * uint64_t     -> id
             * std::string  -> name
             *
             *
             * Function map:
             *
             * std::string  -> name
             * bool         -> external flag
             * uint64_t     -> bytecode address or function pointer
             */
            std::unordered_map<uint64_t, std::string> symbols;
            std::unordered_map<std::string, std::pair<bool, uint64_t>> functions;

            void compile(std::string __bytecode);
            void _push64(uint64_t data);

            inline void advance()
            {
                if (this->instructionPtr + 1 > this->instructions.size())
                {
                    this->instructionPtr = this->instructions.size();
                    Internal::Instruction i;
                    this->currenInstruction = i;
                    return;
                }
                this->currenInstruction = this->instructions[this->instructionPtr++];
            }

        public:
            VM(const std::string & __bytecode);
       
            /*
             * Flag getters and setters
             */
            bool        getCmpResult(uint8_t id);
            void        setCmpResult(uint8_t id, bool res);
            void        setJmp(bool start);

            uint64_t                    getInstructionPtr();
            Internal::Instruction &     getCurrentInstruction();

            /*
             * Register getters
             */
            Internal::Register & getRegister(RegisterType type);
            
            /*
             * Function adding functions
             */
            std::string getSymbol(uint16_t id);
            void        registerSymbol(uint16_t id, std::string name);
            void        registerFunction(std::string name, uint64_t location);
            void        externalFunction(std::string name, void( * function)(VM & vm));

            /*
             * Stack manipulation
             */
            void        pushChar(char data);
            void        pushUint(uint64_t data);
            void        pushInt(int64_t data);
            void        pushFloat(double data);
            char        popChar();
            uint64_t    popUint();
            int64_t     popInt();
            double    popFloat();

            /*
             * Call a function
             */
            void        run(std::string function);
    };
}

#endif
