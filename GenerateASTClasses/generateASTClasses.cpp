#include <iostream>
#include <string>
#include <array>
#include <fstream>
#include <vector>

using VariablePair = std::pair<std::string, std::string>;

static const int number_Of_Class_Types = 4;
static const int max_Class_Member_Amount = 3;

void addDerivedDataIntoVector(const std::array<std::string, number_Of_Class_Types>& types,
                             std::array<std::string, number_Of_Class_Types>& derivedClassNames,
                             std::vector<std::vector<VariablePair>>& derivedClassMembers)
{
    for (size_t i = 0; i < types.size(); i++)
    {
        size_t colonPosition = types[i].find(":");
        derivedClassNames[i] = types[i].substr(0, colonPosition - 1);
        std::string derivedClassMembersString = types[i].substr(colonPosition + 2, types[i].length() - colonPosition);
        std::string memberString = "";

        for (size_t k = 0; k < derivedClassMembersString.length(); k++)
        {
            if (k == derivedClassMembersString.length() - 1)
            {
                memberString.push_back(derivedClassMembersString[k]);
            }

            if (derivedClassMembersString[k] == ',' || k == derivedClassMembersString.length() - 1)
            {
                int spaceIndex = memberString.find(' ');
                std::string variableType = memberString.substr(0, spaceIndex);
                std::string variableName = memberString.substr(spaceIndex + 1, memberString.length() - spaceIndex);
                VariablePair variablePair(variableType, variableName);
                derivedClassMembers[i].push_back(variablePair);
                memberString = "";
                k++;
            }
            else
            {
                memberString.push_back(derivedClassMembersString[k]);
            }
        }
    }
}

std::string derivedClassConstructorParameters(const std::vector<VariablePair>& derivedClassMembers)
{
    std::string constructorParameters = "";
    
    for (VariablePair varPair : derivedClassMembers)
    {
        constructorParameters.append(varPair.first + "& " + varPair.second + ", ");
    }

    constructorParameters = constructorParameters.substr(0, constructorParameters.length() - 2);

    return constructorParameters;
}

void defineType(std::ofstream& outputFile, const std::string& baseClassName,
                const std::string& derivedClassName, const std::vector<VariablePair>& derivedClassMembers)
{
    outputFile << "class " + derivedClassName + " : public " + baseClassName + "\n{\n";
    outputFile << "    public:\n";
    
    for (VariablePair varPair : derivedClassMembers)
    {
        if (varPair.first == "InterpreterObject")
            outputFile << "        " + varPair.first + " m_" + varPair.second + ";\n";
        else
            outputFile << "        " + varPair.first + "* m_" + varPair.second + ";\n";
    }

    outputFile << "\n    public:\n";
    outputFile << "        " + derivedClassName + "(" + derivedClassConstructorParameters(derivedClassMembers) + ");\n";
    outputFile << "        InterpreterObject accept(Visitor& visitor) override;\n";
    outputFile << "};\n\n";
}

void defineVisitor(std::ofstream& outputFile, const std::string& baseClassName, 
                    const std::array<std::string, number_Of_Class_Types>& derivedClassNames)
{
    outputFile << "class Visitor\n{\n";
    outputFile << "    public:\n";

    for (std::string derivedClassName : derivedClassNames)
    {
        outputFile << "        virtual InterpreterObject visit" + derivedClassName + baseClassName + "(" + derivedClassName + "& expr) = 0;\n";
    }

    outputFile << "};\n\n";
}

void defineDerivedFunctions(std::ofstream& outputFile, const std::string& baseClassName, 
                            const std::string& derivedClassName, const std::vector<VariablePair>& derivedClassMembers)
{
    outputFile << derivedClassName + "::" + derivedClassName + "(" + derivedClassConstructorParameters(derivedClassMembers) + ")\n{\n";
    
    for (VariablePair varPair : derivedClassMembers)
    {
        if (varPair.first == "InterpreterObject")
            outputFile << "    m_" + varPair.second + " = " + varPair.second + ";\n";
        else
            outputFile << "    m_" + varPair.second + " = &" + varPair.second + ";\n";
    }

    outputFile << "}\n\n";
    outputFile << "InterpreterObject " + derivedClassName + "::accept" + +"(Visitor& visitor)\n{\n";
    outputFile << "    return visitor.visit" + derivedClassName + baseClassName + "(*this);\n}\n\n";
}

void defineAstHeader(const std::string& outputDir,
                     const std::string& baseClassName,
                     const std::array<std::string, number_Of_Class_Types>& derivedClassNames,
                     const std::vector<std::vector<VariablePair>>& derivedClassMembers)
{
    std::string headerPath = outputDir + "/ast.h";
    std::ofstream outputFile;
    outputFile.open(headerPath);

    if (outputFile.is_open())
    {
        outputFile << "#ifndef AST\n#define AST\n\n";
        outputFile << "#include \"scanner.h\"\n";
        outputFile << "#include \"interpreterObject.h\"\n\n";

        //forward declare derived classes
        for (std::string derivedClassName : derivedClassNames)
        {
            outputFile << "class " + derivedClassName + ";\n";
        }

        outputFile << "\n";
        defineVisitor(outputFile, baseClassName, derivedClassNames);

        //define base class
        outputFile << "class " + baseClassName + "\n{\n";
        outputFile << "    public:\n";
        outputFile << "        virtual InterpreterObject accept(Visitor& visitor) = 0;\n";
        outputFile << "};\n\n";

        //define derived classes
        for (size_t i = 0; i < number_Of_Class_Types; i++)
        {
            defineType(outputFile, baseClassName, derivedClassNames[i], derivedClassMembers[i]);
        }

        outputFile << "#endif";
    }
    else
    {
        std::cerr << "Error could not open file!" << std::endl;
    }

    outputFile.close();
}

void defineAstCpp(const std::string& outputDir,
                  const std::string& baseClassName,
                  const std::array<std::string, number_Of_Class_Types>& derivedClassNames,
                  const std::vector<std::vector<VariablePair>> derivedClassMembers)
{
    std::string cppPath = outputDir + "/ast.cpp";
    std::ofstream outputFile;
    outputFile.open(cppPath);

    if (outputFile.is_open())
    {
        outputFile << "#include \"ast.h\"\n\n";

        //define derived member functions and constructors
        for (size_t i = 0; i < number_Of_Class_Types; i++)
        {
            defineDerivedFunctions(outputFile, baseClassName, derivedClassNames[i], derivedClassMembers[i]);
        }
    }
    else
    {
        std::cerr << "Error could not open file!" << std::endl;
    }

    outputFile.close();
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: generate_ast <output directory>";
        exit(1);
    }

    std::string outputDir = argv[1];
    std::cout << "dir: " + outputDir;
    std::array<std::string, number_Of_Class_Types> types = {"Binary : Expr left, Token oper, Expr right",
                                                         "Grouping : Expr expression",
                                                         "Literal : InterpreterObject value",
                                                         "Unary : Token oper, Expr right"};

    std::array<std::string, number_Of_Class_Types> derivedClassNames;
    std::vector<std::vector<VariablePair>> derivedClassMembers;
    derivedClassMembers.reserve(number_Of_Class_Types);

    for (int i = 0; i < number_Of_Class_Types; i++)
    {
        derivedClassMembers.emplace_back();
        derivedClassMembers[i].reserve(max_Class_Member_Amount);
    }

    addDerivedDataIntoVector(types, derivedClassNames, derivedClassMembers);

    defineAstHeader(outputDir, "Expr", derivedClassNames, derivedClassMembers);
    defineAstCpp(outputDir, "Expr", derivedClassNames, derivedClassMembers);

    return 0;
}