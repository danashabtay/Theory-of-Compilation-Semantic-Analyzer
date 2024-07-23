#ifndef _236360_3_NODES_H
#define _236360_3_NODES_H

#include <string>
#include <vector>
#include <iostream>


class Node {

public:
    std::string val;

    Node();
    Node(const Node *other);
    virtual ~Node() = default;
    std::string getValue() const;
    //Node(const Node &node): val(node.val){};
    virtual ~Node() = default;
};


class Program : public Node
    {
    public:
        Program(/* args */) = default;
        ~Program() = default;
    };

    class Statements : public Node
    {
    public:
        Statements(/* args */) = default;
        ~Statements() = default;
    };

    class Statement : public Node
    {
    public:
        Statement(/* args */) = default;
        ~Statement() = default;
    };

    class Call : public Node
    {
    public:
        std::string returnType;
        
        //Call(/* args */) = default;
        ~Call() = default;
    };


class Type : public Node {

public:
    std::string type;

    Type(const std::string type);
    std::string getType() const;
    bool isNum() const;
    virtual ~Type() = default;
};


class Exp : public Node {

public:
    std::string type;

// Methods:
    Exp(const Exp *other);
    Exp(const Node *id);
    Exp(const Call *call);
    Exp(std::string type);
    Exp(std::string type, const Node *node);
    Exp(const Exp *operand, std::string opType);
    Exp(const Exp *operand1, const Exp *operand2, std::string opType);
    Exp(const Exp *operand, const Type *type);
    virtual ~Exp() = default;
    bool isNumExp() const; 

};


class BooleanExp : public Exp {

public:
    bool boolean;
    
    virtual ~BooleanExp() = default;
};

class NumExp : public Exp
{
public:
    int numVal;
    std::string type;

    ~NumExp() = default;
};




#define YYSTYPE Node

#endif //_236360_3_NODES_H

