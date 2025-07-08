#include <iostream>
#include <string>
#include <array>
#include <fstream>

void defineType(std::ofstream& outputFile, const std::string& baseClassName, 
                const std::string& derivedClassName, const std::string& derivedClassMembers)
{
    outputFile << "template<typename T>\n";
    outputFile << "class " + derivedClassName + ": public " + baseClassName + "<T>\n    {\n";
    outputFile << "\tpublic:\n";
    outputFile << "\t\t" + derivedClassName + "(" + derivedClassMembers + ");\n";
    outputFile << "\t\tT accept(Visitor<T>& visitor) override;\n";
    outputFile << "};\n\n";
}

void defineVisitor(std::ofstream& outputFile, const std::string& baseClassName, const std::array<std::string, 4>& types)
{
    outputFile << "template<typename T>\n";
    outputFile << "class Visitor\n{\n";
    outputFile << "\tpublic:\n";

    for (std::string type : types)
    {
        std::size_t colonPosition = type.find(":");
        std::string derivedClassName = type.substr(0, colonPosition - 1);
        outputFile << "\t\tvirtual T visit" + derivedClassName + baseClassName + "(" + derivedClassName + "<T>& expr) = 0;\n";
    }
    outputFile << "};\n\n";
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
        outputFile << "\tpublic:\n";
        outputFile << "\t\tvirtual T accept(Visitor<T>& visitor) = 0;\n";
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

    return 0;
}