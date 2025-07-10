#include <iostream>
#include <string>
#include <array>
#include <fstream>

void defineType(std::ofstream& outputFile, const std::string& baseClassName, 
                const std::string& derivedClassName, const std::string& derivedClassMembers)
{
    outputFile << "template<typename T>\n";
    outputFile << "class " + derivedClassName + " : public " + baseClassName + "<T>\n{\n";
    outputFile << "    private:\n";
    std::string tempString = "";

    //TODO: try to refactor this into a function
    for (int i = 0; i < int(derivedClassMembers.length()); i++)
    {
        if (derivedClassMembers[i] == ',')
        {
            int spaceIndex = tempString.find(' ');
            std::string type = tempString.substr(0, spaceIndex);
            std::string variableName = tempString.substr(spaceIndex + 1, tempString.length() - spaceIndex);

            outputFile << "        const " + type + " m_" + variableName + ";\n";
            tempString = "";
            i++;
        }
        else
        {
            tempString.push_back(derivedClassMembers[i]);
        }
    }

    int spaceIndex = tempString.find(' ');
    std::string type = tempString.substr(0, spaceIndex);
    std::string variableName = tempString.substr(spaceIndex + 1, tempString.length() - spaceIndex);

    outputFile << "        const " + type + " m_" + variableName + ";\n";
    outputFile << "\n    public:\n";
    outputFile << "        " + derivedClassName + "(" + derivedClassMembers + ");\n";
    outputFile << "        T accept(Visitor<T>& visitor) override;\n";
    outputFile << "};\n\n";
}

void defineVisitor(std::ofstream& outputFile, const std::string& baseClassName, const std::array<std::string, 4>& types)
{
    outputFile << "template<typename T>\n";
    outputFile << "class Visitor\n{\n";
    outputFile << "    public:\n";

    for (std::string type : types)
    {
        std::size_t colonPosition = type.find(":");
        std::string derivedClassName = type.substr(0, colonPosition - 1);
        outputFile << "        virtual T visit" + derivedClassName + baseClassName + "(" + derivedClassName + "<T>& expr) = 0;\n";
    }

    outputFile << "};\n\n";
}

void defineDerivedFunctions(std::ofstream& outputFile, const std::string& baseClassName, const std::string& derivedClassName, const std::string& derivedClassMembers)
{
    outputFile << "template<typename T>\n";
    outputFile << derivedClassName + "<T>::" + derivedClassName + "(" + derivedClassMembers + ")\n{\n";
    std::string tempString = "";

    for (int i = 0; i < int(derivedClassMembers.length()); i++)
    {
        if (derivedClassMembers[i] == ',')
        {
            int spaceIndex = tempString.find(' ');
            std::string variableName = tempString.substr(spaceIndex + 1, tempString.length() - spaceIndex);

            outputFile << "    m_" + variableName + " = " + variableName + ";\n";
            tempString = "";
            i++;
        }
        else
        {
            tempString.push_back(derivedClassMembers[i]);
        }
    }

    int spaceIndex = tempString.find(' ');
    std::string variableName = tempString.substr(spaceIndex + 1, tempString.length() - spaceIndex);
    outputFile << "    m_" + variableName + " = " + variableName + ";\n";

    outputFile << "}\n\n";
    outputFile << "template<typename T>\n";
    outputFile << "T " + derivedClassName + "<T>::accept" +  + "(Visitor<T>& visitor)\n{\n";
    outputFile << "    return visitor.visit" + derivedClassName + baseClassName + "(this);\n}\n\n";
}

void defineAstHeader(const std::string& outputDir, 
               const std::string& baseClassName, 
               const std::array<std::string, 4>& types)
{
    std::string headerPath = outputDir + "/ast.h";
    std::ofstream outputFile;
    outputFile.open(headerPath);

    if (outputFile.is_open())
    {
        outputFile << "#ifndef AST\n#define AST\n\n";
        outputFile << "#include \"scanner.h\"\n\n";

        //forward declare derived classes
        for (std::string type : types)
        {
            std::size_t colonPosition = type.find(":");
            std::string derivedClassName = type.substr(0, colonPosition - 1);
            outputFile << "template<typename T>\n";
            outputFile << "class " + derivedClassName + ";\n\n";
        }

        defineVisitor(outputFile, baseClassName, types);

        //define base class
        outputFile << "template<typename T>\n";
        outputFile << "class " + baseClassName + "\n{\n";
        outputFile << "    public:\n";
        outputFile << "        virtual T accept(Visitor<T>& visitor) = 0;\n";
        outputFile << "};\n\n";

        //define derived classes
        for (std::string type : types)
        {
            std::size_t colonPosition = type.find(":");
            std::string derivedClassName = type.substr(0, colonPosition - 1);
            std::string derivedClassMembers = type.substr(colonPosition + 2, type.length() - colonPosition);
            defineType(outputFile, baseClassName, derivedClassName, derivedClassMembers);
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
               const std::array<std::string, 4>& types)
{
    std::string cppPath = outputDir + "/ast.cpp";
    std::ofstream outputFile;
    outputFile.open(cppPath);

    if (outputFile.is_open())
    {
        outputFile << "#include \"ast.h\"\n\n";
        
        //define derived member functions and constructors
        for (std::string type : types)
        {
            std::size_t colonPosition = type.find(":");
            std::string derivedClassName = type.substr(0, colonPosition - 1);
            std::string derivedClassMembers = type.substr(colonPosition + 2, type.length() - colonPosition);
            defineDerivedFunctions(outputFile, baseClassName, derivedClassName, derivedClassMembers);
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
    std::array<std::string, 4> types = {"Binary : Expr<T> left, Token oper, Expr<T> right",
                                        "Grouping : Expr<T> expression",
                                        "Literal : T value",
                                        "Unary : Token oper, Expr<T> right"};
    defineAstHeader(outputDir, "Expr", types);
    defineAstCpp(outputDir, "Expr", types);

    return 0;
}