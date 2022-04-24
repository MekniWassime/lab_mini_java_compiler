#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>
#include <assert.h> 

using namespace std;
const vector<string> VAR_TYPES = {"int", "int[]", "bool", "bool[]", "String", "String[]"};
const vector<string> METHOD_TYPES = {"int", "int[]", "bool", "bool[]", "String", "String[]", "void"};

struct Node{
    string name;
    bool isUsed = false;
    int line;
};

struct Variable:Node{
    bool isInitialized = false;
};

struct Method:Node{
    vector<Variable> parameters;
    vector<Variable> localVariables;
    int blockStart;
};

struct Class{
    string name;
    vector<Variable> attributes;
    vector<Method> methods;
    int blockStart;
    int line;

    void print(){
        cout << "class " << name << " line: " << line << "\n";
        cout << "| attributes:" << "\n";
        for (size_t i = 0; i < attributes.size(); i++)
            cout << "| | " << attributes[i].name << " line: " << attributes[i].line << "\n";
        cout << "| +\n";
        cout << "| methods:" << "\n"; 
        for (size_t i = 0; i < methods.size(); i++){
            cout << "| | " << methods[i].name << "(";
            for (size_t j = 0; j < methods[i].parameters.size(); j++)
                cout << " " << methods[i].parameters[j].name<< " line: " << methods[i].parameters[j].line;
            cout << " ) line: " << methods[i].line << "\n";
            for (size_t j = 0; j < methods[i].localVariables.size(); j++)
                cout << "| | | " << methods[i].localVariables[j].name << " line: " << methods[i].localVariables[j].line << "\n";
            cout << "| | +\n";
            cout << "| +\n"; 
        }
        cout << "+\n";
    }
};

bool isNumber(string token){
    return regex_match(token, regex(R"(^-?\d+$)"));
}

struct TokenIterator{
    int currentIndex = 0;
    vector<vector<string>> tokens;

    TokenIterator(vector<vector<string>> tokens): tokens(tokens){}

    int getLineNumber(){
        int index = currentIndex;
        for (size_t line = 0; line < tokens.size(); line++){
            for (size_t column = 0; column < tokens[line].size(); column++){
                if(index == 0)
                    return line + 1;
                index--;
            }
        }
        return tokens.size();
    }
    
    bool hasToken(){
        int index = currentIndex;
        for (size_t line = 0; line < tokens.size(); line++){
            for (size_t column = 0; column < tokens[line].size(); column++){
                if(index == 0)
                    return true;
                index--;
            }
        }
        return false;
    }

    int saveCheckPoint(){
        return currentIndex;
    }

    void loadCheckPoint(int checkPoint){
        currentIndex = checkPoint;
    }

    string getCurrentToken(){
        int index = currentIndex;
        for (size_t line = 0; line < tokens.size(); line++){
            for (size_t column = 0; column < tokens[line].size(); column++){
                if(index == 0){
                    return tokens[line][column];
                }
                index--;
            }
        }
        return "";
    }

    string getNextToken(){
        int index = currentIndex;
        for (size_t line = 0; line < tokens.size(); line++){
            for (size_t column = 0; column < tokens[line].size(); column++){
                if(index == 0){
                    currentIndex++;
                    return tokens[line][column];
                }
                index--;
            }
        }
        return "";
    }
};

std::string ltrim(const std::string &s) {
    return std::regex_replace(s, std::regex(R"(^[^\S\r\n]+)"), std::string(""));
}
 
std::string rtrim(const std::string &s) {
    return std::regex_replace(s, std::regex(R"([^\S\r\n]+$)"), std::string(""));
}
 
std::string trim(const std::string &s) {
    return ltrim(rtrim(s));
}

void identifierAlreadyUsedError(string identifier, int line){
    cout << "Error: identifier '" << identifier << "' already used, line " << line << "\n";
}

void findAndReplaceAll(std::string & data, std::string toSearch, std::string replaceStr){
    // Get the first occurrence
    size_t pos = data.find(toSearch);
    // Repeat till end is reached
    while( pos != std::string::npos){
        // Replace this occurrence of Sub String
        data.replace(pos, toSearch.size(), replaceStr);
        // Get the next occurrence from the current position
        pos =data.find(toSearch, pos + replaceStr.size());
    }
}

void deleteFirstOccurance(string & data, string toDelete){
    size_t pos = data.find(toDelete);
    data.replace(pos, toDelete.size(), "");
}

bool charInList(char c, vector<char> chars){
    for (size_t i = 0; i < chars.size(); i++){
        if(c == chars[i])
            return true;
    }
    return false;
}

bool stringInList(string s, vector<string> strings){
    for (size_t i = 0; i < strings.size(); i++){
        if(s.compare(strings[i])==0)
            return true;
    }
    return false;
    
}

string getNextToken(string code){
    size_t start_index = 0, end_index = 1;
    if(charInList(code[start_index], vector<char>{'(','{','}',')','.', ';',',','<','>','=','&','|','*','-','+','/','\n'})){
        string result;
        result.push_back(code[start_index]);
        if(end_index<code.size() && charInList(code[start_index], vector<char>{'<','>'}) && code[end_index]=='=')
            result.push_back(code[end_index]);
        if(end_index<code.size() && code[start_index]=='&' && code[end_index]=='&')
            result.push_back(code[end_index]);
        if(end_index<code.size() && code[start_index]=='|' && code[end_index]=='|')
            result.push_back(code[end_index]);
        return result;
    }
    while(!charInList(code[end_index], vector<char>{' ', '(','{','}',')','.', ';',',','<','>','=','&','|','*','-','+','/', '\n'}))
        end_index++;
    return code.substr(start_index, end_index-start_index);
}

string codeCleanUp(string code){
    //remove single line comments
    regex commentLineRegEx(R"(//.*)");
    code = regex_replace(code, commentLineRegEx, "");
    //remove block comments while keeping line counts
    regex commentBlockRegEx(R"(\/\*(.|\n)*?\*\/)");
    smatch match;
    while(regex_search(code, match, commentBlockRegEx)){
        string matchStr = match.str(0);
        size_t n = count(matchStr.begin(), matchStr.end(), '\n');
        string replacement = "";
        for (size_t i = 0; i < n; i++)
            replacement+="\n";
        findAndReplaceAll(code, match.str(0), replacement);
    }
    //remove System.out.println
    findAndReplaceAll(code, "System.out.println", "");
    return code;
}

bool nameAlreadyUsed(string name, vector<Class> alreadyDefined){
    for (size_t i = 0; i < alreadyDefined.size(); i++){
        if(name.compare(alreadyDefined[i].name)==0)
            return true;
    }
    return false;
}

bool nameAlreadyUsed(string name, vector<Node> alreadyDefined){
    for (size_t i = 0; i < alreadyDefined.size(); i++){
        if(name.compare(alreadyDefined[i].name)==0)
            return true;
    }
    return false;
}

bool nameAlreadyUsed(string name, vector<Variable> alreadyDefined){
    for (size_t i = 0; i < alreadyDefined.size(); i++){
        if(name.compare(alreadyDefined[i].name)==0)
            return true;
    }
    return false;
}

vector<Variable> parseVarDeclarations(TokenIterator & tokens, vector<Variable> alreadyDefined = vector<Variable>()){
    vector<Variable> result;
    while(tokens.getCurrentToken().compare("}") != 0){
        int loopStartCP = tokens.saveCheckPoint();
        while(stringInList(tokens.getCurrentToken(), {"public", "static"}))
            tokens.getNextToken();
        if(stringInList(tokens.getNextToken(), VAR_TYPES)){
            int line = tokens.getLineNumber();
            string identifier = tokens.getNextToken();
            if(tokens.getCurrentToken().compare("(")==0){   //method declaration
                tokens.loadCheckPoint(loopStartCP);
                return result;
            }else{
                Variable newVar;
                newVar.name = identifier;
                newVar.line = line;
                while(tokens.getCurrentToken().compare(";")!=0)
                    tokens.getNextToken();
                tokens.getNextToken();
                result.push_back(newVar);                       //points at what's after ";"
            }
        }else{                                                  //not var declaration
            tokens.loadCheckPoint(loopStartCP);
            return result;
        }
    }
    return result;
}

vector<Method> parseMethods(TokenIterator & tokens){
    vector<Method> result;
    while(tokens.getCurrentToken().compare("}")!=0){
        int loopStartCP = tokens.saveCheckPoint();
        while(stringInList(tokens.getCurrentToken(), {"public", "static"}))
            tokens.getNextToken();
        assert(stringInList(tokens.getNextToken(), METHOD_TYPES));
        Method newMeth;
        newMeth.line = tokens.getLineNumber();
        newMeth.name = tokens.getNextToken();
        assert(tokens.getNextToken().compare("(")==0);
        if(tokens.getCurrentToken().compare(")")!=0){
            while(tokens.getCurrentToken().compare(")")!=0){
                assert(stringInList(tokens.getNextToken(), VAR_TYPES));
                Variable argument;
                int line = tokens.getLineNumber();
                argument.name = tokens.getNextToken();
                argument.isInitialized = true;
                argument.line = line;
                newMeth.parameters.push_back(argument);
                if(tokens.getCurrentToken().compare(",")==0)
                    tokens.getNextToken();
            }
        }
        tokens.getNextToken();
        assert(tokens.getNextToken().compare("{")==0);
        newMeth.blockStart = tokens.saveCheckPoint();
        newMeth.localVariables = parseVarDeclarations(tokens);
        result.push_back(newMeth);
        //skip until end of method block
        int nbOpenBraces = 0;
        while(nbOpenBraces!=0 || tokens.getCurrentToken().compare("}")!=0){
            string token = tokens.getCurrentToken();
            if(token.compare("{")==0)
                nbOpenBraces ++;
            else if(token.compare("}")==0)
                nbOpenBraces --;
            tokens.getNextToken();
        }
        assert(tokens.getNextToken().compare("}")==0);
    }
    return result;
}


Class parseClass(TokenIterator & tokens, vector<Class> alreadyDefined){
    Class classObject;
    assert(tokens.getNextToken().compare("class")==0);
    classObject.line = tokens.getLineNumber();
    classObject.name = tokens.getNextToken();
    while(tokens.getCurrentToken().compare("{") != 0)
        tokens.getNextToken();
    tokens.getNextToken();
    tokens.getNextToken();//point at what's after "{"
    classObject.blockStart = tokens.saveCheckPoint();
    classObject.attributes = parseVarDeclarations(tokens);
    classObject.methods = parseMethods(tokens);
    assert(tokens.getNextToken().compare("}")==0);
    return classObject;
}

vector<Class> generateSymbolTable(TokenIterator tokens){
    vector<Class> result;
    while (tokens.hasToken()){
        Class classObject = parseClass(tokens, result);
        result.push_back(classObject);
    }
    return result;
}

vector<vector<string>> generateTokenTable(string code){
    vector<vector<string>> result;
    while(!code.empty()){
        code = trim(code);
        string token = getNextToken(code);
        vector<string> row;
        if(token.compare("\n")!=0)
            row.push_back(token);
        deleteFirstOccurance(code, token);
        while(!code.empty() && token.compare("\n")!=0){
            code = trim(code);
            token = getNextToken(code);
            if(token.compare("\n")!=0)
                row.push_back(token);
            deleteFirstOccurance(code, token);
        }
        result.push_back(row);
    }
    return result;
}

void checkIdentifierAlreadyUsed(vector<Class>& symbolTable){
    vector<string> usedClassNames;
    for (size_t i = 0; i < symbolTable.size(); i++){
        Class& currentClass = symbolTable[i];
        if(stringInList(currentClass.name, usedClassNames)){
            identifierAlreadyUsedError(currentClass.name, currentClass.line);
        }else{
            usedClassNames.push_back(currentClass.name);
            vector<string> usedClassLevelNames;
            for (size_t j = 0; j < currentClass.attributes.size(); j++){
                Variable& currentAttribute = currentClass.attributes[j];
                if(stringInList(currentAttribute.name, usedClassLevelNames)){
                    identifierAlreadyUsedError(currentAttribute.name, currentAttribute.line);
                }else{
                    usedClassLevelNames.push_back(currentAttribute.name);
                }
            }
            for (size_t j = 0; j < currentClass.methods.size(); j++){
                Method& currentMethod = currentClass.methods[j];
                if(stringInList(currentMethod.name, usedClassLevelNames)){
                    identifierAlreadyUsedError(currentMethod.name, currentMethod.line);
                }else{
                    usedClassLevelNames.push_back(currentMethod.name);
                    vector<string> usedMethodLevelNames;
                    for (size_t k = 0; k < currentMethod.parameters.size(); k++){
                        Variable& currentParameter = currentMethod.parameters[k];
                        if(stringInList(currentParameter.name, usedMethodLevelNames)){
                            identifierAlreadyUsedError(currentParameter.name, currentParameter.line);
                        }else{
                            usedMethodLevelNames.push_back(currentParameter.name);
                        }
                    }
                    for (size_t k = 0; k < currentMethod.localVariables.size(); k++){
                        Variable& currentVariable = currentMethod.localVariables[k];
                        if(stringInList(currentVariable.name, usedMethodLevelNames)){
                            identifierAlreadyUsedError(currentVariable.name, currentVariable.line);
                        }else{
                            usedMethodLevelNames.push_back(currentVariable.name);
                        }
                    }
                }
            }
        }
    } 
}

void templateCheck(vector<Class> &symbolTable){
    for (size_t i = 0; i < symbolTable.size(); i++){
        Class &currentClass = symbolTable[i];
        for (size_t j = 0; j < currentClass.attributes.size(); j++){
            Variable &currentAttribute = currentClass.attributes[j];
        }
        for (size_t j = 0; j < currentClass.methods.size(); j++){
            Method &currentMethod = currentClass.methods[j];
            for (size_t k = 0; k < currentMethod.parameters.size(); k++){
                Variable &currentParameter = currentMethod.parameters[k];
            }
            for (size_t k = 0; k < currentMethod.localVariables.size(); k++){
                Variable &currentVariable = currentMethod.localVariables[k];
            }
        }
    }
}

bool isDefinedInMemberAttributes(string identifier, Class& currentClass){
    for (size_t i = 0; i < currentClass.attributes.size(); i++){
        Variable& attribute = currentClass.attributes[i];
        if(identifier.compare(attribute.name)==0){
            return true;
        }
    }
    return false;
}

bool isDefinedClass(string identifier, vector<Class> & classes){
    for (size_t i = 0; i < classes.size(); i++){
        Class& tempClass = classes[i];
        if(identifier.compare(tempClass.name)==0){
            return true;
        }
    }
    return false;
}

Class& findClass(string identifier, vector<Class> & classes){
    for (size_t i = 0; i < classes.size(); i++){
        Class& tempClass = classes[i];
        if(identifier.compare(tempClass.name)==0){
            return tempClass;
        }
    }
}

Variable& findMemberAttributeNode(string identifier, Class& currentClass){
    for (size_t i = 0; i < currentClass.attributes.size(); i++){
        Variable& attribute = currentClass.attributes[i];
        if(identifier.compare(attribute.name)==0){
            return attribute;
        }
    }
}

bool isDefinedMemberMethod(string identifier, Class& currentClass){
    for (size_t i = 0; i < currentClass.methods.size(); i++){
        Method& method = currentClass.methods[i];
        if(identifier.compare(method.name)==0){
            return true;
        }
    }
    return false;
}

Method& findMemberMethodNode(string identifier, Class& currentClass){
    for (size_t i = 0; i < currentClass.methods.size(); i++){
        Method& method = currentClass.methods[i];
        if(identifier.compare(method.name)==0){
            return method;
        }
    }
}

bool isDefinedInMethodScope(string identifier, Method& currentMethod){
    for (size_t i = 0; i < currentMethod.parameters.size(); i++){
        Variable& parameter = currentMethod.parameters[i];
        if(identifier.compare(parameter.name)==0){
            return true;
        }
    }
    for (size_t i = 0; i < currentMethod.localVariables.size(); i++){
        Variable& localVariable = currentMethod.localVariables[i];
        if(identifier.compare(localVariable.name)==0){
            return true;
        }
    }
    return false;
}

Variable& findVariableNodeMethodScope(string identifier, Method& currentMethod){
    for (size_t i = 0; i < currentMethod.parameters.size(); i++){
        Variable& parameter = currentMethod.parameters[i];
        if(identifier.compare(parameter.name)==0){
            return parameter;
        }
    }
    for (size_t i = 0; i < currentMethod.localVariables.size(); i++){
        Variable& localVariable = currentMethod.localVariables[i];
        if(identifier.compare(localVariable.name)==0){
            return localVariable;
        }
    }
}

void verifyMethodOrVariableClassLevel(Class& currentClass, TokenIterator& tokens){
    int line = tokens.getLineNumber();
    string identifier = tokens.getNextToken();
    int checkpoint = tokens.saveCheckPoint();
    if (tokens.getCurrentToken().compare("(") == 0){ // method
        int parametersCheckpoint = tokens.saveCheckPoint();
        if (isDefinedMemberMethod(identifier, currentClass)){
            Method &method = findMemberMethodNode(identifier, currentClass);
            int nbParams = method.parameters.size();
            int givenArgs = 0;
            tokens.getNextToken();
            if (!tokens.getCurrentToken().compare(")") == 0){
                givenArgs++;
                while (!tokens.getCurrentToken().compare(")") == 0){
                    if (tokens.getCurrentToken().compare(",") == 0)
                        givenArgs++;
                    tokens.getNextToken();
                }
            }
            method.isUsed = true;
            assert(tokens.getNextToken().compare(")") == 0);
            if (nbParams != givenArgs){
                cout << "Error: method call '" << identifier << "' expects " << nbParams << " arguments but got " << givenArgs << ", line " << line << "\n";
            }
            tokens.loadCheckPoint(parametersCheckpoint);
        }else{
            cout << "Error: method '" << identifier << "' is undefined, line " << line << "\n";
        }
    }else{ // attribute
        if (isDefinedInMemberAttributes(identifier, currentClass)){
            Variable &variable = findMemberAttributeNode(identifier, currentClass);
            variable.isInitialized = true;
            if (!tokens.getNextToken().compare("=") == 0){ // being used
                variable.isUsed = true;
            }
        }else{
            cout << "Error: variable '" << identifier << "' is undefined, line " << line << "\n";
        }
    }
}

void checkIsDefined(vector<Class> &symbolTable, TokenIterator tokens){
    for (size_t i = 0; i < symbolTable.size(); i++){
        Class &currentClass = symbolTable[i];
        for (size_t j = 0; j < currentClass.methods.size(); j++){
            Method &currentMethod = currentClass.methods[j];
            tokens.loadCheckPoint(currentMethod.blockStart);    //points right after "{"
            parseVarDeclarations(tokens);
            int nbOpenBraces = 1;
            while(nbOpenBraces>0){
                while(!stringInList(tokens.getCurrentToken(),{"{", "}"})){
                    while(stringInList(tokens.getCurrentToken(), {"(",")","if","else","return",";","+","-","*","/","<","<=",">",">=", "&&", "||", "true", "false",","}) || isNumber(tokens.getCurrentToken())){
                        tokens.getNextToken();
                    }
                    if(stringInList(tokens.getCurrentToken(),{"{", "}"}))
                        continue;
                    if(tokens.getCurrentToken().compare("new")==0){
                        tokens.getNextToken();
                        int line = tokens.getLineNumber();
                        string identifier = tokens.getNextToken();
                        assert(tokens.getNextToken().compare("(")==0);
                        assert(tokens.getNextToken().compare(")")==0);
                        assert(tokens.getNextToken().compare(".")==0);//points at what's after '.'
                        if(isDefinedClass(identifier, symbolTable)){//class defined
                            Class& typeClass = findClass(identifier, symbolTable);
                            verifyMethodOrVariableClassLevel(typeClass, tokens);
                        }else{
                            cout << "Error: Type '" << identifier << "' is undefined, line " << line;
                        }
                    }else if(tokens.getCurrentToken().compare("this")==0){
                        assert(tokens.getNextToken().compare("this")==0);
                        assert(tokens.getNextToken().compare(".")==0);
                        verifyMethodOrVariableClassLevel(currentClass, tokens);
                    }else{
                        int line = tokens.getLineNumber();
                        string identifier = tokens.getNextToken();
                        int checkpoint = tokens.saveCheckPoint();
                        if(tokens.getCurrentToken().compare("(")==0){//method
                            int parametersCheckpoint = tokens.saveCheckPoint();
                            if(isDefinedMemberMethod(identifier, currentClass)){
                                Method& method = findMemberMethodNode(identifier, currentClass);
                                int nbParams = method.parameters.size();
                                int givenArgs = 0;
                                tokens.getNextToken();
                                if(!tokens.getCurrentToken().compare(")")==0){
                                    givenArgs++;
                                    while(!tokens.getCurrentToken().compare(")")==0){
                                        if(tokens.getCurrentToken().compare(",")==0)
                                            givenArgs++;
                                        tokens.getNextToken();
                                    }
                                }
                                method.isUsed = true;
                                assert(tokens.getNextToken().compare(")")==0);
                                if(nbParams != givenArgs){
                                    cout << "Error: method call '" << identifier << "' expects " << nbParams << " arguments but got " << givenArgs << ", line " << line << "\n";
                                }
                                tokens.loadCheckPoint(parametersCheckpoint);
                            }else{
                                cout << "Error: method '" << identifier << "' is undefined, line " << line << "\n";
                            }
                        }else{//attribute
                            if(isDefinedInMethodScope(identifier, currentMethod)){
                                Variable& variable = findVariableNodeMethodScope(identifier, currentMethod);
                                if(tokens.getNextToken().compare("=")==0){//assignation
                                    variable.isInitialized = true;
                                }else{//being used
                                    if(!variable.isInitialized){
                                        cout << "Error: variable '" << identifier << "' is used before it's initialized, line " << line << "\n";
                                    }else{
                                        variable.isUsed = true;
                                    }
                                }
                            }else{
                                if(isDefinedInMemberAttributes(identifier, currentClass)){
                                    Variable& variable = findMemberAttributeNode(identifier, currentClass);
                                    variable.isInitialized = true;
                                    if(!tokens.getNextToken().compare("=")==0){//being used
                                        variable.isUsed = true;
                                    }
                                }else{
                                    cout << "Error: variable '" << identifier << "' is undefined, line " << line << "\n";
                                }
                            }
                        }
                    }
                }
                string token = tokens.getCurrentToken();
                if(token.compare("{")==0){
                    nbOpenBraces ++;
                }
                else if(token.compare("}")==0){
                    nbOpenBraces --;
                }
                tokens.getNextToken();
            }
            assert(tokens.getNextToken().compare("}")==0);
        }
    }
}

void checkUnused(vector<Class> &symbolTable){
    for (size_t i = 0; i < symbolTable.size(); i++){
        Class &currentClass = symbolTable[i];
        for (size_t j = 0; j < currentClass.attributes.size(); j++){
            Variable &currentAttribute = currentClass.attributes[j];
            if(!currentAttribute.isUsed)
                cout << "Warning: '" << currentAttribute.name << "' is declared but never used, line " << currentAttribute.line << "\n";
        }
        for (size_t j = 0; j < currentClass.methods.size(); j++){
            Method &currentMethod = currentClass.methods[j];
            if(!currentMethod.isUsed)
                cout << "Warning: '" << currentMethod.name << "' is declared but never used, line " << currentMethod.line << "\n";
            for (size_t k = 0; k < currentMethod.parameters.size(); k++){
                Variable &currentParameter = currentMethod.parameters[k];
                if(!currentParameter.isUsed)
                    cout << "Warning: '" << currentParameter.name << "' is declared but never used, line " << currentParameter.line << "\n";
            }
            for (size_t k = 0; k < currentMethod.localVariables.size(); k++){
                Variable &currentVariable = currentMethod.localVariables[k];
                if(!currentVariable.isUsed)
                    cout << "Warning: '" << currentVariable.name << "' is declared but never used, line " << currentVariable.line << "\n";
            }
        }
    }
}

int main(int argc, char *argv[]) {
    fstream textFile;
    textFile.open(argv[1],ios::in);
    string code = "", line;
    while(getline(textFile, line)){
        line = trim(line);
        code += line + "\n";
    }
    textFile.close();
    code = codeCleanUp(code);
    vector<vector<string>> tokenTable = generateTokenTable(code);
    TokenIterator tokenIterator(tokenTable);
    vector<Class> symbolTable = generateSymbolTable(tokenIterator);
    for (size_t i = 0; i < symbolTable.size(); i++){
        symbolTable[i].print();
    }
    checkIdentifierAlreadyUsed(symbolTable);
    checkIsDefined(symbolTable, tokenIterator);
    checkUnused(symbolTable);
    return 0;
}