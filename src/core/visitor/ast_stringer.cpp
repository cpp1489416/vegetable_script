#include "ast_stringer.h"

namespace vegetable_script {

void AstStringer::Visit(FloatExpression* node) {
  stream_ << node->value;
}

void AstStringer::Visit(IntegerExpression* node) {
  stream_ << node->value;
}

void AstStringer::Visit(StringExpression* node) {
  stream_ << "\"" << node->value << "\"";
}

void AstStringer::Visit(IdentifierExpression* node) {
  stream_ << node->value;
}

void AstStringer::Visit(UnaryExpression* node) {
  stream_ << node->operatorr.ToString();
  std::string c;
  if (node->operatorr == UnaryExpression::Operator::kNegative) {
    c = "-";
  } else {
    c = "+";
  }
  stream_ << " " << c << " ";
  node->operand_expression->Accept(this);
}

void AstStringer::Visit(BinaryExpression* node) {
  stream_ << "(";
  node->left_expression->Accept(this);

  std::string ope;
  if (node->operatorr == BinaryExpression::Operator::kPlus) {
    ope = "+";
  } else if (node->operatorr == BinaryExpression::Operator::kMinus) {
    ope = "-";
  } else if (node->operatorr == BinaryExpression::Operator::kMultiply) {
    ope = "*";
  } else if (node->operatorr == BinaryExpression::Operator::kDivide) {
    ope = "/";
  } else {
    ope = node->operatorr.ToString();
  }

  stream_ << " " << ope << " ";

  node->right_expression->Accept(this);
  stream_ << ")";
}

/*
void ASTStringer::Visit(NumberExpression* node) {
    mStream << node->mValue;
}

void ASTStringer::Visit(EmptyExpression* node) {
}

void ASTStringer::Visit(VariableExpression* node) {
    mStream << node->mName;
}

void ASTStringer::Visit(CalculateExpression* node) {
    mStream << "(";
    node->mLeftExpression->Accept(this);
    switch (node->mOperator) {
    case CalculateExpression::Operator::Plus:
        mStream << " + ";
        break;
    case CalculateExpression::Operator::Minus:
        mStream << " - ";
        break;
    case CalculateExpression::Operator::Multiply:
        mStream << " * ";
        break;
    case CalculateExpression::Operator::Divide:
        mStream << " / ";
        break;
    default:
        break;
    }
    node->mRightExpression->Accept(this);
    mStream << ")";
}

void ASTStringer::Visit(CompareExpression* node) {
    mStream << "(";
    node->mLeftExpression->Accept(this);
    switch (node->mOperator) {
    case CompareExpression::Operator::Greater:
        mStream << " > ";
        break;
    case CompareExpression::Operator::Lesser:
        mStream << " > ";
        break;
    default:
        break;
    }
    node->mRightExpression->Accept(this);
    mStream << ")";
}

void ASTStringer::Visit(AssignExpression* node) {
    mStream << "(";
    node->mLeftExpression->Accept(this);
    mStream << " = ";
    node->mRightExpression->Accept(this);
    mStream << ")";
}

void ASTStringer::Visit(ExpressionStatement* node) {
    node->mExpression->Accept(this);
    mStream << ";\n";
}

void ASTStringer::Visit(BlockStatement* node) {
    mStream << "{\n";
    for (auto itr : node->mStatements) {
        itr->Accept(this);
    }
    mStream << "}\n";
}

void ASTStringer::Visit(IfStatement* node) {
    mStream << "if (";
    node->mConditionExpression->Accept(this);
    mStream << ")\n";
    node->mMainStatement->Accept(this);
    mStream << "else\n";
    node->mElseStatement->Accept(this);
}

void ASTStringer::Visit(WhileStatement* node) {
    mStream << "while (";
    node->mConditionExpression->Accept(this);
    mStream << ")\n";
    node->mBodyStatement->Accept(this);
}

void ASTStringer::Visit(ForStatement* node) {
    mStream << "for (";
    node->mBeginStatement->Accept(this);
    node->mConditionExpression->Accept(this);
    node->mAfterExpression->Accept(this);
    mStream << ")\n";
    node->mBodyStatement->Accept(this);
}

void ASTStringer::Visit(FunctionExpression* node) {
    mStream << node->mName;
    mStream << "(";
    if (node->mArgumentList.GetSize() > 0)
    {
        node->mArgumentList[0]->Accept(this);
        for (int i = 1; i < node->mArgumentList.GetSize(); ++i)
        {
            mStream << ", ";
            node->mArgumentList[i]->Accept(this);
        }
    }
    mStream << ")";
}

void ASTStringer::Visit(VariableDefinition* node) {
    mStream << "var " << node->mName << " : ";
    node->mValueExpression->Accept(this);
    mStream << ";\n";
}

void ASTStringer::Visit(FunctionDefinition* node) {
    mStream << "function " << node->mName << "(";
    if (node->mParemeterList.GetSize() > 0) {
        node->mParemeterList[0]->Accept(this);
        for (int i = 1; i < node->mParemeterList.GetSize(); ++i) {
            mStream << ", ";
            node->mParemeterList[i]->Accept(this);
        }
    }
    mStream << ")\n";
    node->mBlockStatement->Accept(this);
}

void ASTStringer::Visit(Program* node) {
    for (auto itr : node->mASTs) {
        itr->Accept(this);
    }
}

void ASTStringer::Visit(FunctionSymbol *node) {
}

*/
}  // namespace vegetable_script
