#include "nodes.hpp"

using namespace std;

// Node:

Node::Node() : val("") {};

Node::Node(const Node *other)
{
    if(other)
    {
        this->val = other->val;
    }
}

std::string Node::getValue() const
{
    return this->val;
}




// Type:

Type::Type(std::string type) : type(type) {}

std::string Type::getType() const
{
    return type;
}

bool Type::isNum() const
{
    return this->type == "int" || this->type == "byte";
}


// Exp:

// Exp -> LPAREN Exp RPAREN
Exp::Exp(const Exp *other)
{
    if (other)
    {
        this->type = other->type;
    }
}

// EXP -> ID
Exp::Exp(const Node* id)
{
    if(id)
    {
        string varName = id->getValue();
        /*
        TODO: Find id by varName and if exists get its type and save in this->expType 
        If does not exist, handle error
        */
    }
}

// EXP -> CALL
Exp::Exp(const Call* call)
{
    if (call)
    {
        this->type = call->returnType;
    }
}

// EXP -> NUM, STRING, TRUE, FALSE
Exp::Exp(std::string type) : type(type) {}

//Exp -> NUM B
Exp::Exp(std::string type, const Node *node)
{
    if (type == "byte" && node && stoi(node->getValue()) < 255)
    {
        this->type = type;
    }
    else
    {
        /* TODO: Handle Error*/
    }
}

// EXP -> NOT EXP
Exp::Exp(const Exp *operand, std::string opType)
{
    if (operand && operand->type == opType && opType == "bool")
    {
        this->type = "bool";
    }
    else
    {
        /* TODO: Handle Error*/
    }
}


// EXP -> EXP OP EXP
Exp::Exp(const Exp *operand1, const Exp *operand2, std::string opType)
{
    if (operand1 && operand2)
    {
    
        if(opType == "logic")
        {
             if (operand1->type == operand2->type && operand2->type == "bool")
            {
                this->type = "bool";
                return;
            }
        }
        else if(opType == "relop")
        {
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                this->type = "bool";
                return;
            }
        }
        else if(opType == "arithmetic")
        {
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                if (operand1->type == "int" || operand2->type == "int")
                {
                    this->type = "int";
                }
                else
                {
                    this->type = "byte";
                }
                this->type = "bool";
                return;
            }
        }
    }
    /* TODO: Handle Error*/
}

// EXP -> LPAREN Type RPAREN Exp
Exp::Exp(const Exp *operand, const Type *type)
{
    if (operand && operand->isNumExp() && type && type->isNum())
    {
        this->type = type->getType();
    }
    else
    {
        /* TODO: Handle Error*/
    }
}

bool Exp::isNumExp() const
{
    return this->type == "int" || this->type == "byte";
}

// -----