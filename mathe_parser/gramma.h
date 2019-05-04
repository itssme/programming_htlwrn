#ifndef GRAMMA_H
#define GRAMMA_H

#include <string>
#include <vector>
#include <math.h>
#include <QDebug>
#include <exception>

inline bool is_number(std::string num) {
    try {
        std::stod(num);
        return true;
    } catch (...) {
        return false;
    }
}

class Expression {
public:
    virtual double evaluate() = 0;
    virtual ~Expression() {}
    virtual void setLeft(Expression *left) = 0;
    virtual void setRight(Expression *right) = 0;
};
class Terminal : public Expression {
public:
    Terminal(double _num) : num{_num} {}
    virtual double evaluate() {
        return this->num;
    }
    virtual void setLeft(Expression *left) {}
    virtual void setRight(Expression *right) {}
private:
    double num;
};
class Addition : public Expression {
public:
    Addition(Expression *_left = nullptr, Expression *_right = nullptr) : left{_left}, right{_right} {}
    virtual double evaluate() {
      return left->evaluate() + right->evaluate();
    }
    virtual void setLeft(Expression *_left) {
        left = _left;
    }
    virtual void setRight(Expression *_right) {
        right = _right;
    }
private:
    Expression *left;
    Expression *right;
};
class Substraction : public Expression {
public:
    Substraction(Expression *left = nullptr, Expression *right = nullptr) {
        this->left = left;
        this->right = right;
    }
    virtual double evaluate() {
      return left->evaluate() - right->evaluate();
    }
    virtual void setLeft(Expression *_left) {
        left = _left;
    }
    virtual void setRight(Expression *_right) {
        right = _right;
    }
private:
    Expression *left;
    Expression *right;
};
class Multiplication : public Expression {
public:
    Multiplication(Expression *left = nullptr, Expression *right = nullptr) {
        this->left = left;
        this->right = right;
    }
    virtual double evaluate() {
      return left->evaluate() * right->evaluate();
    }
    virtual void setLeft(Expression *_left) {
        left = _left;
    }
    virtual void setRight(Expression *_right) {
        right = _right;
    }
private:
    Expression *left;
    Expression *right;
};
class Division : public Expression {
public:
    Division(Expression *left = nullptr, Expression *right = nullptr) {
        this->left = left;
        this->right = right;
    }
    virtual double evaluate() {
      return left->evaluate() / right->evaluate();
    }
    virtual void setLeft(Expression *_left) {
        left = _left;
    }
    virtual void setRight(Expression *_right) {
        right = _right;
    }
private:
    Expression *left;
    Expression *right;
};
class Power : public Expression {
public:
    Power(Expression *left = nullptr, Expression *right = nullptr) {
        this->left = left;
        this->right = right;
    }
    virtual double evaluate() {
      return std::pow(left->evaluate(),right->evaluate());
    }
    virtual void setLeft(Expression *_left) {
        left = _left;
    }
    virtual void setRight(Expression *_right) {
        right = _right;
    }
private:
    Expression *left;
    Expression *right;
};


class Parser {
public:
    unsigned long int index{0};
    std::string next_token;
    std::vector<std::string> tokens;
    Parser(std::vector<std::string> _tokens) : tokens{_tokens} {
        this->next_token = this->tokens.at(this->index);
    }
    void consume() {
        this->index++;
        this->next_token = this->tokens.at(this->index);
    }
    Expression* get_expression(std::string token) {
        qDebug() << "reading " << QString(token.c_str());
        if (token == "+") {
            return new Addition();
        } else if (token == "-") {
            return new Substraction();
        } else if (token == "*") {
            return new Multiplication();
        } else if (token == "/") {
            return new Division();
        } else if (token == "^") {
            return new Power();
        } else {
            throw std::logic_error("Invalid symbol -> " + token);
        }
    }
    void check_next(std::string token) {
        if (token != this->next_token) {
            throw std::logic_error("Invalid symbol in check -> " + token);
        } else {
            this->consume();
        }
    }
    Expression* init_node(Expression* operation, Expression* left_value, Expression* right_value) {
        operation->setLeft(left_value);
        operation->setRight(right_value);
        return operation;
    }

    // G:
    //     E --> T {( "+" | "-" ) T}
    //     T --> F {( "*" | "/" ) F}
    //     F --> P ["^" F]
    //     P --> v | "(" E ")" | "-" P

    double parse() {
        auto value = E();
        if (this->next_token != "stop") {
               throw std::logic_error("Stopped parsing but not at end -> " + this->next_token);
        }
        return value->evaluate();
    }
    Expression* E() {
        Expression* t = T();
        while (this->next_token == "+" || this->next_token == "-") {
            Expression* operation = this->get_expression(this->next_token);
            this->consume();
            auto t1 = T();
            t = this->init_node(operation, t, t1);
        }
        return t;
    }
    Expression* T() {
        Expression* t = F();
        while (this->next_token == "*" || this->next_token == "/") {
            Expression* operation = this->get_expression(this->next_token);
            this->consume();
            auto t1 = F();
            t = this->init_node(operation, t, t1);
        }
        return t;
    }
    Expression* F() {
        Expression* t = P();
        if (this->next_token == "^") {
            Expression* operation = this->get_expression("^");
            this->consume();
            auto t1 = F();
            return this->init_node(operation, t, t1);
        }
        return t;
    }
    Expression* P() {
        Expression* t = nullptr;
        if (is_number(this->next_token)) {
            t = new Terminal(stod(this->next_token));
            this->consume();
            return t;
        } else if (this->next_token == "(") {
            this->consume();
            t = E();
            this->check_next(")");
            return t;
        } else if (this->next_token == "-") {
            this->consume();
            t = P();
            return init_node(get_expression("-"), new Terminal{0}, t);
        } else {
            throw std::logic_error("invalid symbol at p -> " + this->next_token);
        }
    }
};

#endif // GRAMMA_H
