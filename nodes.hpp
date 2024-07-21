#ifndef _236360_3_NODES_H
#define _236360_3_NODES_H

#include <string>
#include <vector>
#include <iostream>

class Node {

public:
    std::string val;

    Node() : val("") {};
    Node(const std::string &val): val(val){};
    //Node(const Node &node): val(node.val){};
    virtual ~Node() = default;
};


class Type : public Node {

public:
    Type(const Node* typeNode): Node(typeNode->val) {};
    virtual ~Type() = default;
};


class Exp : public Node {

public:
    std::string type;
    
    Exp(const Node* expNode, std::string str);

    virtual ~Exp() = default;
};


class BooleanExp : public Exp {

public:
    bool boolean;
    
    virtual ~BooleanExp() = default;
};

#endif //_236360_3_NODES_H

