#include "ll1_definition_analyzer.h"

namespace vegetable_script {

bool Ll1DefinitionAnalyzer::Parse(Lexer* lexer, Definition::Ptr* definition, Exception* exception) {
  return false;
}

bool Ll1DefinitionAnalyzer::ParseDefinition(Lexer* lexer, Definition::Ptr* statement, Exception* exception) {
  return false;
}

bool Ll1DefinitionAnalyzer::ParseFunctionDefinition(Lexer* lexer, ExpressionStatement::Ptr* statement, Exception* exception) {
  return false;
}

bool Ll1DefinitionAnalyzer::ParseVariableDefinition(Lexer* lexer, BlockStatement::Ptr* statement, Exception* exception)
{
  return false;
}

}

