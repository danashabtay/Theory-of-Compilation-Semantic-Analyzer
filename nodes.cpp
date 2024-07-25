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

//Program:
Program::Program() {}


//Statements:
Statements::Statements(Statement *statement){}

Statements::Statements(Statements *statements, Statement *statement){}


//Statement:

// Statement -> CONTINUE SC / BREAK SC
Statement::Statement(const Node *node)
{
    if(node->val == "continue")
    {
        if(!stacks.is_loop())
        {
            output::errorUnexpectedContinue(yylineno);
            exit(0);
        }
    }
    else if(node->val == "break")
    {
        if(!stacks.is_loop())
        {
            output::errorUnexpectedBreak(yylineno);
            exit(0);
        }
    }
}


// Statement -> TYPE ID SC
Statement::Statement(const Type *type, const Node *node)
{
    if(stacks.doesSymbolExists(node->val))
    {
        output::errorDef(yylineno, node->val);
        exit(0);
    }

    stacks.insertSymbol(node->val, type->type, false);
    this->val = type->val; //?? - statement and type have no val 
}

/// Statement -> TYPE ID ASSIGN EXP SC
Statement::Statement(Type *type, Node *node, Exp *exp)
{
    if(stacks.doesSymbolExists(node->val))
    {
        output::errorDef(yylineno, node->val);
        exit(0);
    }
    //checking if types match
    if(type->type != exp->type){
        if (type->type == "string" || exp->type == "string" || exp->type == "void"){
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (type->type == "byte" &&  exp->type == "int"){
            output::errorMismatch(yylineno);
            exit(0);
        }
    }
    else{
        stacks.insertSymbol(node->val, type->type, false);
    }
    /// TODO: need to check for cases of null?
}
/// Statement -> ID ASSIGN EXP SC
Statement::Statement(Node *node, Exp *exp)
{
    //check if symbol wasn't defined 
    if(!stacks.doesSymbolExists(node->val))
    {
        output::errorUndef(yylineno, node->val);
        exit(0);
    }
    //types check
    symTableEntry* symbol = stacks.getSymbol(node->val);
    if(symbol->isFunc){
        output::errorMismatch(yylineno);
        exit(0);
    }
    else if(symbol->type != exp->type){
        if (symbol->type == "string" || exp->type == "string" || exp->type == "void"){
            output::errorMismatch(yylineno);
            exit(0);
        }
        if (symbol->type == "byte" &&  exp->type == "int"){
            output::errorMismatch(yylineno);
            exit(0);
        }
    }
}

/// Statement -> Call SC
Statement::Statement(Call *call)
{
    if(!stacks.doesSymbolExists(call->val))
    {
        output::errorUndefFunc(yylineno, call->val);
        exit(0);
    }
    symTableEntry* symbol = stacks.getSymbol(call->val);
    if(!symbol->isFunc)
    {
        output::errorUndefFunc(yylineno, val);
        exit(0);
    }
}
///  Statement -> IF LPAREN Exp RPAREN Statement / ELSE Statement / WHILE LPAREN Exp RPAREN  Statement
Statement::Statement(Exp *exp)
{
//checkinh if condition given is of type bool
    if(exp->type!="bool"){
        output::errorMismatch(yylineno);
        exit(0);
    }
}

// Call:

Call::Call(const Node *node, const Exp *exp)
{
    std::string name = node->val;

    
    //look in global scope for the function:
    if(!stacks.doesSymbolExists(name))
    {
        output::errorUndefFunc(yylineno, val);
        exit(0);
    }

    // found a matching function symbol! check if it is a function:
    symTableEntry* symbol = stacks.getSymbol(name);
    if(!symbol->isFunc)
    {
        output::errorUndefFunc(yylineno, val);
        exit(0);
    }
    // it is a function! check argument type:
    else if(!(symbol->params == exp->val) && !(symbol->params == "int" && exp->val == "byte"))
    {
        output::errorPrototypeMismatch(yylineno, name, symbol->params);
        exit(0);
    }

    // we found the right function!
    this->val = name;
    this->returnType = symbol->type;
   
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
        if(!stacks.doesSymbolExists(id->getValue()))
        {
            output::errorUndef(yylineno,id->getValue());
            exit(0);
        }
        symTableEntry *symbol = stacks.getSymbol(id->getValue());
        this->val = symbol->name;
        this->type = symbol->type;
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
    if (operand && operand->type == "bool" && opType == "logic") 
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


void check_bool(Node *node) {
    Exp *exp = dynamic_cast<Exp *>(node);
    if (exp->type != "bool") {
        output::errorMismatch(yylineno);
        exit(0);
    }
}
// -----