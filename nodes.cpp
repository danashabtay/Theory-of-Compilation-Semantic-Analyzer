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
    return this->type == "INT" || this->type == "BYTE";
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
    if (type == "BYTE" && node && stoi(node->getValue()) < 255)
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
    if (operand && operand->type == "BOOL" && opType == "logic") ///////////////////////**** */
    {
        this->type = "BOOL";
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
             if (operand1->type == operand2->type && operand2->type == "BOOL")
            {
                this->type = "BOOL";
                return;
            }
        }
        else if(opType == "relop")
        {
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                this->type = "BOOL";
                return;
            }
        }
        else if(opType == "arithmetic")
        {
            if (operand1->isNumExp() && operand2->isNumExp())
            {
                if (operand1->type == "INT" || operand2->type == "INT")
                {
                    this->type = "INT";
                }
                else
                {
                    this->type = "BYTE";
                }
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
    return this->type == "INT" || this->type == "BYTE";
}

// -----