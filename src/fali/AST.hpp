#ifndef FALCON_FALI_AST_HPP
#define FALCON_FALI_AST_HPP

#include "common/Common.hpp"

#include "fali/Types.hpp"

namespace Falcon
{
    namespace FALI
    {
        struct ASTNode
        {
            uint64_t LineNumber;

            virtual ~ASTNode() = default;
        };
        
        struct LiteralNode : public ASTNode
        {
            std::optional<bool>     Boolean;
            std::optional<uint64_t> Whole;
            std::optional<int64_t>  Integer;
            std::optional<double>   Real;

            LiteralNode(bool b, uint64_t line = 0);
            LiteralNode(uint64_t u64, uint64_t line = 0);
            LiteralNode(int64_t i64, uint64_t line = 0);
            LiteralNode(double f64, uint64_t line = 0);
        };

        struct VariableNode : public ASTNode
        {
            std::string Name;
            DataType    Type;

            VariableNode() = default;
            VariableNode(const std::string & name, const DataType & type, uint64_t line = 0);
            VariableNode(const std::string & name, const std::string & type, const TypeFlag & flags = 0, uint64_t line = 0);
        };

        struct OperatorNode;

        struct ValueNode : public ASTNode
        {
            std::optional<LiteralNode> Literal;
            std::optional<VariableNode> Variable;
            std::optional<OperatorNode *> Operator;

            ValueNode() = default;
            explicit ValueNode(LiteralNode literal, uint64_t line = 0);
            explicit ValueNode(VariableNode variable, uint64_t line = 0);
            explicit ValueNode(OperatorNode * op, uint64_t line = 0);
        };

        struct OperatorNode : public ASTNode
        {
            bool Binary;

            char Type;
            
            std::optional<ValueNode> Left;
            std::optional<ValueNode> Right;

            explicit OperatorNode(char type, uint64_t line = 0);
            OperatorNode(char type, ValueNode Left, uint64_t line = 0);
            OperatorNode(char type, ValueNode Left, ValueNode Right, uint64_t line = 0);
        };

        struct StatementNode;

        struct IfNode : public ASTNode
        {
            OperatorNode Condition;
            std::vector<StatementNode> Statements;

            explicit IfNode(OperatorNode condition, uint64_t line = 0);
        };

        struct ElseNode : public ASTNode
        {
            std::vector<StatementNode> Statements;
        };

        struct WhileNode : public ASTNode
        {
            OperatorNode Condition;
            std::vector<StatementNode> Statements;

            explicit WhileNode(OperatorNode condition, uint64_t line = 0);
        };

        struct StatementNode : public ASTNode
        {
            std::optional<OperatorNode> Operator;
            std::optional<IfNode>       IfBlock;
            std::optional<ElseNode>     ElseBlock;
            std::optional<WhileNode>    WhileBlock;

            explicit StatementNode(OperatorNode operatorNode, uint64_t line = 0);
            explicit StatementNode(IfNode ifBlock, uint64_t line = 0);
            explicit StatementNode(ElseNode elseBlock, uint64_t line = 0);
            explicit StatementNode(WhileNode whileBlock, uint64_t line = 0);
        };

        struct FunctionNode : public ASTNode
        {
            DataType    ReturnType;
            std::string Name;
            std::vector<std::pair<DataType, std::string>> Params;
            
            std::map<std::string, VariableNode> LocalVariables;
            std::vector<StatementNode> Statements;

            FunctionNode(const DataType & retType, const std::string & name, const std::vector<std::pair<DataType, std::string>> & params, uint64_t line = 0);
            FunctionNode(const std::string & retType, const std::string & name, const std::vector<std::pair<DataType, std::string>> & params, const TypeFlag & retTypeFlags = 0, uint64_t line = 0);

            std::string GetSignature();
        };

        struct ClassNode : public ASTNode
        {
            std::string Name;

            std::vector<VariableNode> PublicMemberVariables;
            std::vector<VariableNode> ProtectedMemberVariables;
            std::vector<VariableNode> PrivateMemberVariables;

            std::vector<FunctionNode> PublicMemberFunctions;
            std::vector<FunctionNode> ProtectedMemberFunctions;
            std::vector<FunctionNode> PrivateMemberFunctions;

            ClassNode(const std::string & name, uint64_t line = 0);
        };

        struct ModuleNode : public ASTNode
        {
            std::vector<ClassNode>      Classes;
            std::vector<FunctionNode>   Functions;
        };
    }
}

#endif
