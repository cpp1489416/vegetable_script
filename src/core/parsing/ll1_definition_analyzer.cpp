#include "./ll1_definition_analyzer.h"
#include "./ll1_expression_analyzer.h"
#include "./ll1_statement_analyzer.h"

namespace vegetable_script {

bool Ll1DefinitionAnalyzer::Parse(Lexer* lexer, Definition::Ptr* definition, Exception* exception) {
  return ParseDefinition(lexer, definition, exception);
}

bool Ll1DefinitionAnalyzer::ParseDefinition(Lexer* lexer, Definition::Ptr* definition, Exception* exception) {
  Token token;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type == Token::Type::kKeywordFunc) {
    auto function_definition = std::make_shared<FunctionDefinition>();
    if (!ParseFunctionDefinition(lexer, &function_definition, exception)) {
      return false;
    }
    *definition = function_definition;
    return true;
  } else if (token.type == Token::Type::kKeywordVar) {
    // return ParseVariableDefinition(lexer, definition, exception);
    return false;
  } else {
    *exception = {
      "it is not a begin of definition", token.row, token.column
    };
    return false;
  }
  return false;
}

bool Ll1DefinitionAnalyzer::ParseFunctionDefinition(
    Lexer* lexer, FunctionDefinition::Ptr*
    function_definition,
    Exception* exception) {
  Token token;
  *function_definition = std::make_shared<FunctionDefinition>();
  Ll1StatementAnalyzer statement_analyzer;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordFunc) {
    *exception = {
      "function definition must begin with \"func\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kIdentifier) {
    *exception = {
      "function name is incorrect, maybe a keyword", token.row, token.column
    };
    return false;
  }
  (*function_definition)->name = token.string;
  lexer->MoveNext();
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall, Token::Type::kBracketLeft })) {
    *exception = {
      "missing \"(\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  while (true) {
    if (!lexer->LookCurrent(&token, exception)) {
      return false;
    }
    if (token.type != Token::Type::kIdentifier) {
      *exception = {
        "missing identifier of a parameter", token.row, token.column
      };
      return false;
    }
    auto parameter = std::make_shared<VariableDefinition>();
    parameter->string = token.string;
    (*function_definition)->parameter_list.push_back(parameter);
    lexer->MoveNext();
    if (!lexer->LookCurrent(&token, exception)) {
      return false;
    }
    if (token.type == Token::Type::kOperatorComma) {
      lexer->MoveNext();
      continue;
    } else if (token.MatchType({ Token::Type::kBracketRight, Token::Type::kBracketSmall })) {
      lexer->MoveNext();
      break;
    } else {
      *exception = {
        "missing \")\" or \",\"", token.row, token.column
      };
      return false;
    }
  }
  if (!statement_analyzer.ParseBlockStatement(lexer, &(*function_definition)->block_statement, exception)) {
    return false;
  }
  return true;
}

bool Ll1DefinitionAnalyzer::ParseVariableDefinition(Lexer* lexer, VariableDefinition::Ptr* statement, Exception* exception)
{
  return false;
}

}

