#ifndef FALCON_ASM_AST_HPP
#define FALCON_ASM_AST_HPP

#include <algorithm>
#include <string>
#include <vector>

#include <cstdint>

#include "../vm/Register.hpp"

namespace Falcon
{
    namespace Assembler
    {
        class ASTNode
        {
            public:
                uint64_t Line;
                uint64_t Character;

                virtual ~ASTNode() = default;
        };

        class AtomNode : public ASTNode
        {
            public:
                enum class AtomType
                {
                    IDENTIFIER,
                    REGISTER,
                    CHAR,
                    UINT,
                    INT,
                    FLOAT
                } Type;

                union
                {
                    char     Char;
                    uint64_t Uint;
                    int64_t  Int;
                    double   Float;
                };
                std::string Str;

                AtomNode(const std::string & str);
                AtomNode(char data);
                AtomNode(uint64_t data);
                AtomNode(int64_t data);
                AtomNode(double data);
        };

        class InstructionNode : public ASTNode
        {
            public:
                std::string             Inst;
                std::vector<AtomNode>   Args;

                InstructionNode(const std::string & inst);
        };

        class LabelNode : public ASTNode
        {
            public:
                std::string                  Name;
                std::vector<InstructionNode> Instructions;

                LabelNode(const std::string & name);
        };

        class RoutineNode : public ASTNode
        {
            public:
                std::string            Name;
                std::vector<LabelNode> Labels;

                RoutineNode(const std::string & name);
        };

        class CodeSectionNode : public ASTNode
        {
            public:
                std::vector<RoutineNode> Routines;
        };
    }
}

#endif
