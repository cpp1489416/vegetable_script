#include "./ll1_statement_analyzer.h"
#include <initializer_list>
#include <memory>
#include <deque>
#include "./lexer.h"
#include "../ast/core_asts.h"
#include "./parsing_exception.h"
#include "./ll1_definition_analyzer.h"

namespace vegetable_script {

bool Ll1StatementAnalyzer::Parse(
    Lexer* lexer,
    Statement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  return ParseStatement(lexer, statement, exception);
}

bool Ll1StatementAnalyzer::ParseStatement(
    Lexer* lexer,
    Statement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.MatchType({ Token::Type::kBracketBig,
      Token::Type::kBracketLeft })) {
    BlockStatement::Ptr ans_statement;
    if (!ParseBlockStatement(lexer, &ans_statement, exception)) {
      return false;
    }
    *statement = ans_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordIf) {
    IfStatement::Ptr if_statement;
    if (!ParseIfStatement(lexer, &if_statement, exception)) {
      return false;
    }
    *statement = if_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordWhile) {
    WhileStatement::Ptr while_statement;
    if (!ParseWhileStatement(lexer, &while_statement, exception)) {
      return false;
    }
    *statement = while_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordFor) {
    ForStatement::Ptr for_statement;
    if (!ParseForStatement(lexer, &for_statement, exception)) {
      return false;
    }
    *statement = for_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordContinue) {
    ContinueStatement::Ptr continue_statement;
    if (!ParseContinueStatement(lexer, &continue_statement, exception)) {
      return false;
    }
    *statement = continue_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordBreak) {
    BreakStatement::Ptr break_statement;
    if (!ParseBreakStatement(lexer, &break_statement, exception)) {
      return false;
    }
    *statement = break_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordReturn) {
    ReturnStatement::Ptr return_statement;
    if (!ParseReturnStatement(lexer, &return_statement, exception)) {
      return false;
    }
    *statement = return_statement;
    return true;
  } else if (token.type == Token::Type::kKeywordFunc || token.type == Token::Type::kKeywordVar) {
    Definition::Ptr definition;
    if (!ParseDefinitionStatement(lexer, &definition, exception)) {
      return false;
    }
    *statement = definition;
    return true;
  } else if (token.type == Token::Type::kSemicolon) {
    *statement = std::make_shared<EmptyStatement>();
    lexer->MoveNext();
    return true;
  } else {
    ExpressionStatement::Ptr expression_statement;
    if (!ParseExpressionStatement(lexer, &expression_statement, exception)) {
      return false;
    }
    *statement = expression_statement;
    return true;
  }
}

bool Ll1StatementAnalyzer::ParseExpressionStatement(
    Lexer* lexer,
    ExpressionStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  *statement = std::make_shared<ExpressionStatement>();
  if (!expression_analyzer_.Parse(
        lexer,
        &(*statement)->expression,
        exception)) {
    return false;
  }
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kSemicolon) {
    *exception = {
      "this statement must be ended with a \";\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  return true;
}

bool Ll1StatementAnalyzer::ParseDefinitionStatement(
    Lexer* lexer,
    Definition::Ptr* definition,
    Ll1StatementAnalyzer::Exception* exception) {
  return Ll1DefinitionAnalyzer().Parse(lexer, definition, exception);
}

bool Ll1StatementAnalyzer::ParseBlockStatement(
    Lexer* lexer,
    BlockStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketBig,
      Token::Type::kBracketLeft })) {
    *exception = {
      "block statement must begin with a \"{\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  auto block_statement = std::make_shared<BlockStatement>();
  while (true) {
    if (!lexer->LookCurrentWithoutComments(&token, exception)) {
      return false;
    }
    if (token.MatchType({ Token::Type::kBracketBig,
        Token::Type::kBracketRight})) {
      lexer->MoveNext();
      *statement = block_statement;
      return true;
    }
    Statement::Ptr inside_statement;
    if (!ParseStatement(lexer, &inside_statement, exception)) {
      return false;
    }
    block_statement->statements.push_back(inside_statement);
  }
}

bool Ll1StatementAnalyzer::ParseIfStatement(
    Lexer* lexer,
    IfStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordIf) {
    *exception = {
      "if statement must begin with \"if\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketLeft })) {
    *exception = {
      "missing \"(\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  auto if_statement = std::make_shared<IfStatement>();
  if (!expression_analyzer_.Parse(lexer,
      &if_statement->condition_expression, exception)) {
    return false;
  }
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketRight })) {
    *exception = {
      "missing \")\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!ParseStatement(lexer, &if_statement->body_statement, exception)) {
    return false;
  }
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.type == Token::Type::kKeywordElse) {
    lexer->MoveNext();
    if (!ParseStatement(lexer, &if_statement->else_statement, exception)) {
      return false;
    }
  }
  *statement = if_statement;
  return true;
}

bool Ll1StatementAnalyzer::ParseWhileStatement(
    Lexer* lexer,
    WhileStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordWhile) {
    *exception = {
      "while statement must begin with \"while\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  auto while_statement = std::make_shared<WhileStatement>();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketLeft })) {
    *exception = {
      "missing \"(\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!expression_analyzer_.Parse(lexer,
      &while_statement->condition_expression, exception)) {
    return false;
  }
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketRight })) {
    *exception = {
      "missing \")\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!ParseStatement(lexer, &while_statement->body_statement, exception)) {
    return false;
  }
  *statement = while_statement;
  return true;
}

bool Ll1StatementAnalyzer::ParseForStatement(
    Lexer* lexer,
    ForStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordFor) {
    *exception = {
      "for statement must begin with \"for\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    *exception = {
      "missing \"(\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  auto for_statement = std::make_shared<ForStatement>();
  if (!ParseStatement(lexer, &for_statement->begin_statement, exception)) {
    return false;
  }
  if (!expression_analyzer_.Parse(lexer,
      &for_statement->condition_expression, exception)) {
    return false;
  }
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    *exception = {
      "missing \";\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!expression_analyzer_.Parse(lexer,
      &for_statement->after_expression, exception)) {
    return false;
  }
  if (!lexer->LookCurrentWithoutComments(&token, exception)) {
    return false;
  }
  if (!token.MatchType({ Token::Type::kBracketSmall,
      Token::Type::kBracketRight })) {
    *exception = {
      "missing \")\"",
      token.row,
      token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!ParseStatement(lexer, &for_statement->body_statement, exception)) {
    return false;
  }
  *statement = for_statement;
  return true;
}

bool Ll1StatementAnalyzer::ParseContinueStatement(
    Lexer* lexer,
    ContinueStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordContinue) {
    *exception = {
      "missing \"continue\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kSemicolon) {
    *exception = {
      "missing \";\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  *statement = std::make_shared<ContinueStatement>();
  return true;
}

bool Ll1StatementAnalyzer::ParseBreakStatement(
    Lexer* lexer,
    BreakStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordBreak) {
    *exception = {
      "missing \"break\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kSemicolon) {
    *exception = {
      "missing \";\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  *statement = std::make_shared<BreakStatement>();
  return true;
}

bool Ll1StatementAnalyzer::ParseReturnStatement(
    Lexer* lexer,
    ReturnStatement::Ptr* statement,
    Ll1StatementAnalyzer::Exception* exception) {
  Token token;
  auto return_statement = std::make_shared<ReturnStatement>();
  *statement = return_statement;
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  if (token.type != Token::Type::kKeywordReturn) {
    *exception = {
      "missing \"return\"", token.row, token.column
    };
    return false;
  }
  lexer->MoveNext();
  if (!lexer->LookCurrent(&token, exception)) {
    return false;
  }
  lexer->MoveNext();
  if (token.type == Token::Type::kSemicolon) {
    return true;
  } else {
    if (!expression_analyzer_.Parse(lexer, &return_statement->expression, exception)) {
      return false;
    }
  }
}

}  // namespace vegetable_script
