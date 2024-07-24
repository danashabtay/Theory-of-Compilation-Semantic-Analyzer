#include "symTable.hpp"
#include "hw3_output.hpp"
#include <iostream>

/* symTableEntry */

symTableEntry::symTableEntry(std::string name, std::string type, int offset, bool isFunc, std::string params) : name(name), type(type), offset(offset), isFunc(isFunc), params(params) {}

/* symTable */

symTable::symTable(int curr_offset) : curr_offset(curr_offset) {}

symTable::~symTable()
{
    for (auto it = symbolsTable.begin(); it != symbolsTable.end(); it++)
    {
        delete (*it);
    }
}

bool symTable::doesSymbolExists(const std::string name)
{
    for (auto it = symbolsTable.begin(); it != symbolsTable.end(); it++)
    {
        if ((*it)->name == name)
        {
            return true;
        }
    }
    return false;
}

void symTable::addSymbol(const symTableEntry &symbol)
{
    symbolsTable.push_back(new symTableEntry(symbol));
    if (symbol.offset >= 0)
    {
        curr_offset = symbol.offset;
    }
    else
    {
        cout << "error offset is weird" << endl;
    }
}

/* symTableStack */

symTableStack::symTableStack() : tableStack(), offsetStack()
{
    offsetStack.push_back(0);
    // creating global scope:
    addTable();
    insertSymbol("print", "void", true, "string");
    insertSymbol("printi", "void", true, "int");
    insertSymbol("print", "void", true, "int");
}

symTableStack::~symTableStack()
{
    for (auto it = tableStack.begin(); it != tableStack.end(); it++)
    {
        symTable *curr_table = *it;
        delete curr_table;
    }
}

void symTableStack::addTable()
{
    symTable *newTable = new symTable(offsetStack.back());
    tableStack.push_back(newTable);
    offsetStack.push_back(offsetStack.back());
}

void symTableStack::removeTable()
{
    symTable *curr_table = tableStack.back();
    output::endScope();

    // printing symbols:
    for (auto it = (*curr_table).symbolsTable.begin(); it != (*curr_table).symbolsTable.end(); it++)
    {
        if (!(*it)->isFunc)
        {
            output::printID((*it)->name, (*it)->offset, output::makeFunctionType((*it)->params, (*it)->type));
        }
        else
        {
            output::printID((*it)->name, (*it)->offset, (*it)->type);
        }
    }
    tableStack.pop_back();
    offsetStack.pop_back();
    delete curr_table;
}

void symTableStack::insertSymbol(std::string name, std::string type, bool isFunction, std::string params)
{
    symTable *curr_table = tableStack.back();
    int offset;
    if (isFunction)
    {
        offset = 0;
    }
    else
    {
        offset = offsetStack.back();
        offsetStack.back() = offsetStack.back() + 1;
    }
    symTableEntry symbol = symTableEntry(name, type, offset, isFunction, params);
    curr_table->addSymbol(symbol);
}

bool symTableStack::doesSymbolExists(const std::string name)
{
    for(auto it = tableStack.begin() ; it != tableStack.end() ; it++)
    {
        symTable *curr_table = tableStack.back();
        for(auto it = (*curr_table).symbolsTable.begin(); it != (*curr_table).symbolsTable.end(); it++)
        {
            if((*it)->name == name)
            {
                return true;
            }
        }
    }  
    return false;  
}

symTable *symTableStack::getCurrSymTable()
{
    return tableStack.back();
}

void symTableStack::printScope()
{
}