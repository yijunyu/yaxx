/* Yijun Yu */
%{
#include "types.h"
#include "symtbl.h"
%}
%name-prefix="yy" /* according to Sebastien Fricker */
%union {
	struct {
		struct SymbolTableNode *symptr;
		struct type_info_t type_info;
	} info;
}
%token <info> IDENTIFIER CONSTANT STRING_LITERAL SIZEOF
%token <info> PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token <info> AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token <info> SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token <info> XOR_ASSIGN OR_ASSIGN TYPE_NAME
%token <info> TYPEDEF EXTERN STATIC AUTO REGISTER
%token <info> CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE CONST VOLATILE VOID
%token <info> STRUCT UNION ENUM ELIPSIS RANGE
%token <info> CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN
%token <info> PUNCT_COLON PUNCT_SEMICOLON PUNCT_COMMA PUNCT_DOT PUNCT_EQUAL
%token <info> PUNCT_AMBLE PUNCT_STAR PUNCT_PLUS PUNCT_MINUS PUNCT_TILDE PUNCT_EXCLAIM
%token <info> PUNCT_QUESTION PUNCT_OR
%token <info> PUNCT_DIV PUNCT_PERCENT PUNCT_LT PUNCT_GT PUNCT_CARET
%token <info> PUNCT_LPAR PUNCT_RPAR PUNCT_LBRACKET PUNCT_RBRACKET PUNCT_LBRACE PUNCT_RBRACE

%type <info> primary_expr
%type <info> postfix_expr
%type <info> argument_expr_list
%type <info> unary_expr
%type <info> unary_operator
%type <info> cast_expr
%type <info> multiplicative_expr
%type <info> additive_expr
%type <info> shift_expr
%type <info> relational_expr
%type <info> equality_expr
%type <info> and_expr
%type <info> exclusive_or_expr
%type <info> inclusive_or_expr
%type <info> logical_and_expr
%type <info> logical_or_expr
%type <info> conditional_expr
%type <info> assignment_expr
%type <info> assignment_operator
%type <info> expr
%type <info> constant_expr
%type <info> declaration
%type <info> declaration_specifiers
%type <info> init_declarator_list
%type <info> init_declarator
%type <info> storage_class_specifier
%type <info> type_specifier
%type <info> struct_or_union_specifier
%type <info> struct_or_union
%type <info> struct_declaration_list
%type <info> struct_declaration
%type <info> struct_declarator_list
%type <info> struct_declarator
%type <info> enum_specifier
%type <info> enumerator_list
%type <info> enumerator
%type <info> declarator
%type <info> declarator2
%type <info> pointer
%type <info> type_specifier_list
%type <info> parameter_identifier_list
%type <info> identifier_list
%type <info> parameter_type_list
%type <info> parameter_list
%type <info> parameter_declaration
%type <info> type_name
%type <info> abstract_declarator
%type <info> abstract_declarator2
%type <info> initializer
%type <info> initializer_list
%type <info> statement
%type <info> labeled_statement
%type <info> compound_statement
%type <info> declaration_list
%type <info> statement_list
%type <info> expression_statement
%type <info> selection_statement
%type <info> iteration_statement
%type <info> jump_statement
%type <info> file
%type <info> external_definition
%type <info> function_definition
%type <info> function_body
%type <info> identifier

%start file
%%

primary_expr
    : identifier
    | CONSTANT
    | STRING_LITERAL
    | PUNCT_LPAR expr PUNCT_RPAR
    ;

postfix_expr
    : primary_expr
    | postfix_expr PUNCT_LBRACKET expr PUNCT_RBRACKET
    | postfix_expr PUNCT_LPAR PUNCT_RPAR
    | postfix_expr PUNCT_LPAR argument_expr_list PUNCT_RPAR
    | postfix_expr PUNCT_DOT identifier
    | postfix_expr PTR_OP identifier
    | postfix_expr INC_OP
    | postfix_expr DEC_OP
    ;

argument_expr_list
    : assignment_expr
    | argument_expr_list PUNCT_COMMA assignment_expr
    ;

unary_expr
    : postfix_expr
    | INC_OP unary_expr
    | DEC_OP unary_expr
    | unary_operator cast_expr
    | SIZEOF unary_expr
    | SIZEOF PUNCT_LPAR type_name PUNCT_RPAR
    ;

unary_operator
    : PUNCT_AMBLE
    | PUNCT_STAR
    | PUNCT_PLUS
    | PUNCT_MINUS
    | PUNCT_TILDE
    | PUNCT_EXCLAIM
    ;

cast_expr
    : unary_expr
    | PUNCT_LPAR type_name PUNCT_RPAR cast_expr
    ;

multiplicative_expr
    : cast_expr
    | multiplicative_expr PUNCT_STAR cast_expr
    | multiplicative_expr PUNCT_DIV cast_expr
    | multiplicative_expr PUNCT_PERCENT cast_expr
    ;

additive_expr
    : multiplicative_expr
    | additive_expr PUNCT_PLUS multiplicative_expr
    | additive_expr PUNCT_MINUS multiplicative_expr
    ;

shift_expr
    : additive_expr
    | shift_expr LEFT_OP additive_expr
    | shift_expr RIGHT_OP additive_expr
    ;

relational_expr
    : shift_expr
    | relational_expr PUNCT_LT shift_expr
    | relational_expr PUNCT_GT shift_expr
    | relational_expr LE_OP shift_expr
    | relational_expr GE_OP shift_expr
    ;

equality_expr
    : relational_expr
    | equality_expr EQ_OP relational_expr
    | equality_expr NE_OP relational_expr
    ;

and_expr
    : equality_expr
    | and_expr PUNCT_AMBLE equality_expr
    ;

exclusive_or_expr
    : and_expr
    | exclusive_or_expr PUNCT_CARET and_expr
    ;

inclusive_or_expr
    : exclusive_or_expr
    | inclusive_or_expr PUNCT_OR exclusive_or_expr
    ;

logical_and_expr
    : inclusive_or_expr
    | logical_and_expr AND_OP inclusive_or_expr
    ;

logical_or_expr
    : logical_and_expr
    | logical_or_expr OR_OP logical_and_expr
    ;

conditional_expr
    : logical_or_expr
    | logical_or_expr PUNCT_QUESTION logical_or_expr PUNCT_COLON conditional_expr
    ;

assignment_expr
    : conditional_expr
    | unary_expr assignment_operator assignment_expr
    ;

assignment_operator
    : PUNCT_EQUAL
    | MUL_ASSIGN
    | DIV_ASSIGN
    | MOD_ASSIGN
    | ADD_ASSIGN
    | SUB_ASSIGN
    | LEFT_ASSIGN
    | RIGHT_ASSIGN
    | AND_ASSIGN
    | XOR_ASSIGN
    | OR_ASSIGN
    ;

expr
    : assignment_expr
    | expr PUNCT_COMMA assignment_expr
    ;

constant_expr
    : conditional_expr
    ;

declaration
    : declaration_specifiers PUNCT_SEMICOLON
    | declaration_specifiers init_declarator_list PUNCT_SEMICOLON
    {
		if ($1.type_info.storage_class == TYPEDEF_C) {
			$2.symptr->symtype = TYPEDEF_T;
		}
    }
    ;

declaration_specifiers
    : storage_class_specifier
    {
		$$.type_info.storage_class = $1.type_info.storage_class;
    }
    | storage_class_specifier declaration_specifiers
    {
		$$.type_info.storage_class = $1.type_info.storage_class;
    }
    | type_specifier
    | type_specifier declaration_specifiers
    ;

init_declarator_list
    : init_declarator
    {
		$$.symptr = $1.symptr;
    }
    | init_declarator_list PUNCT_COMMA init_declarator
    ;

init_declarator
    : declarator
    {
		$$.symptr = $1.symptr;
    }
    | declarator PUNCT_EQUAL initializer
    ;

storage_class_specifier
    : TYPEDEF
    {
		$$.type_info.storage_class = TYPEDEF_C;
    }
    | EXTERN
    | STATIC
    | AUTO
    | REGISTER
    ;

type_specifier
    : CHAR
    | SHORT
    | INT
    | LONG
    | SIGNED
    | UNSIGNED
    | FLOAT
    | DOUBLE
    | CONST
    | VOLATILE
    | VOID
    | struct_or_union_specifier
    | enum_specifier
    | TYPE_NAME
    ;

struct_or_union_specifier
    : struct_or_union identifier PUNCT_LBRACE struct_declaration_list PUNCT_RBRACE
    | struct_or_union PUNCT_LBRACE struct_declaration_list PUNCT_RBRACE
    | struct_or_union identifier
    ;

struct_or_union
    : STRUCT
    | UNION
    ;

struct_declaration_list
    : struct_declaration
    | struct_declaration_list struct_declaration
    ;

struct_declaration
    : type_specifier_list struct_declarator_list PUNCT_SEMICOLON
    ;

struct_declarator_list
    : struct_declarator
    | struct_declarator_list PUNCT_COMMA struct_declarator
    ;

struct_declarator
    : declarator
    | PUNCT_COLON constant_expr
    | declarator PUNCT_COLON constant_expr
    ;

enum_specifier
    : ENUM PUNCT_LBRACE enumerator_list PUNCT_RBRACE
    | ENUM identifier PUNCT_LBRACE enumerator_list PUNCT_RBRACE
    | ENUM identifier
    ;

enumerator_list
    : enumerator
    | enumerator_list PUNCT_COMMA enumerator
    ;

enumerator
    : identifier
    | identifier PUNCT_EQUAL constant_expr
    ;

declarator
    : declarator2
    {
		$$.symptr = $1.symptr;
    }
    | pointer declarator2
    {
		$$.symptr = $1.symptr;
    }
    ;

declarator2
    : identifier
    {
		$$.symptr = $1.symptr;
    }
    | PUNCT_LPAR declarator PUNCT_RPAR
    {
		$$.symptr = $2.symptr;
    }
    | declarator2 PUNCT_LBRACKET PUNCT_RBRACKET
    {
		$$.symptr = $1.symptr;
    }
    | declarator2 PUNCT_LBRACKET constant_expr PUNCT_RBRACKET
    {
		$$.symptr = $1.symptr;
    }
    | declarator2 PUNCT_LPAR PUNCT_RPAR
    {
		$$.symptr = $1.symptr;
    }
    | declarator2 PUNCT_LPAR parameter_type_list PUNCT_RPAR
    {
		$$.symptr = $1.symptr;
    }
    | declarator2 PUNCT_LPAR parameter_identifier_list PUNCT_RPAR
    {
		$$.symptr = $1.symptr;
    }
    ;

pointer
    : PUNCT_STAR
    | PUNCT_STAR type_specifier_list
    | PUNCT_STAR pointer
    | PUNCT_STAR type_specifier_list pointer
    ;

type_specifier_list
    : type_specifier
    | type_specifier_list type_specifier
    ;

parameter_identifier_list
    : identifier_list
    | identifier_list PUNCT_COMMA ELIPSIS
    ;

identifier_list
    : identifier
    | identifier_list PUNCT_COMMA identifier
    ;

parameter_type_list
    : parameter_list
    | parameter_list PUNCT_COMMA ELIPSIS
    ;

parameter_list
    : parameter_declaration
    | parameter_list PUNCT_COMMA parameter_declaration
    ;

parameter_declaration
    : type_specifier_list declarator
    | type_name
    ;

type_name
    : type_specifier_list
    | type_specifier_list abstract_declarator 
    ;

abstract_declarator
    : pointer
    | abstract_declarator2
    {
		$$.symptr = $1.symptr;
    }
    | pointer abstract_declarator2
    {
		$$.symptr = $1.symptr;
    }
    ;

abstract_declarator2
    : PUNCT_LPAR abstract_declarator PUNCT_RPAR
    | PUNCT_LBRACKET PUNCT_RBRACKET
    | PUNCT_LBRACKET constant_expr PUNCT_RBRACKET
    | abstract_declarator2 PUNCT_LBRACKET PUNCT_RBRACKET
    | abstract_declarator2 PUNCT_LBRACKET constant_expr PUNCT_RBRACKET
    | PUNCT_LPAR PUNCT_RPAR
    | PUNCT_LPAR parameter_type_list PUNCT_RPAR
    | abstract_declarator2 PUNCT_LPAR PUNCT_RPAR
    | abstract_declarator2 PUNCT_LPAR parameter_type_list PUNCT_RPAR
    ;

initializer
    : assignment_expr
    | PUNCT_LBRACE initializer_list PUNCT_RBRACE
    | PUNCT_LBRACE initializer_list PUNCT_COMMA PUNCT_RBRACE
    ;

initializer_list
    : initializer
    | initializer_list PUNCT_COMMA initializer
    ;

statement
    : labeled_statement
    | compound_statement
    | expression_statement
    | selection_statement
    | iteration_statement
    | jump_statement
    ;

labeled_statement
    : identifier PUNCT_COLON statement
    | CASE constant_expr PUNCT_COLON statement
    | DEFAULT PUNCT_COLON statement
    ;

compound_statement
    : PUNCT_LBRACE PUNCT_RBRACE
    | PUNCT_LBRACE statement_list PUNCT_RBRACE
    | PUNCT_LBRACE declaration_list PUNCT_RBRACE
    | PUNCT_LBRACE declaration_list statement_list PUNCT_RBRACE
    ;

declaration_list
    : declaration
    | declaration_list declaration
    ;

statement_list
    : statement
    | statement_list statement
    ;

expression_statement
    : PUNCT_SEMICOLON
    | expr PUNCT_SEMICOLON
    ;

selection_statement
    : IF PUNCT_LPAR expr PUNCT_RPAR statement
    | IF PUNCT_LPAR expr PUNCT_RPAR statement ELSE statement
    | SWITCH PUNCT_LPAR expr PUNCT_RPAR statement
    ;

iteration_statement
    : WHILE PUNCT_LPAR expr PUNCT_RPAR statement
    | DO statement WHILE PUNCT_LPAR expr PUNCT_RPAR PUNCT_SEMICOLON
    | FOR PUNCT_LPAR PUNCT_SEMICOLON PUNCT_SEMICOLON PUNCT_RPAR statement
    | FOR PUNCT_LPAR PUNCT_SEMICOLON PUNCT_SEMICOLON expr PUNCT_RPAR statement
    | FOR PUNCT_LPAR PUNCT_SEMICOLON expr PUNCT_SEMICOLON PUNCT_RPAR statement
    | FOR PUNCT_LPAR PUNCT_SEMICOLON expr PUNCT_SEMICOLON expr PUNCT_RPAR statement
    | FOR PUNCT_LPAR expr PUNCT_SEMICOLON PUNCT_SEMICOLON PUNCT_RPAR statement
    | FOR PUNCT_LPAR expr PUNCT_SEMICOLON PUNCT_SEMICOLON expr PUNCT_RPAR statement
    | FOR PUNCT_LPAR expr PUNCT_SEMICOLON expr PUNCT_SEMICOLON PUNCT_RPAR statement
    | FOR PUNCT_LPAR expr PUNCT_SEMICOLON expr PUNCT_SEMICOLON expr PUNCT_RPAR statement
    ;

jump_statement
    : GOTO identifier PUNCT_SEMICOLON
    | CONTINUE PUNCT_SEMICOLON
    | BREAK PUNCT_SEMICOLON
    | RETURN PUNCT_SEMICOLON
    | RETURN expr PUNCT_SEMICOLON
    ;

file
    : external_definition
    | file external_definition
    ;

external_definition
    : function_definition
    | declaration
    ;

function_definition
    : declarator function_body
    | declaration_specifiers declarator function_body
    ;

function_body
    : compound_statement
    | declaration_list compound_statement
    ;

identifier
    : IDENTIFIER
    ;
%%

#include <stdio.h>

extern int column;

yyerror(s)
char *s;
{
    fflush(stdout);
    printf("\n%*s\n%*s\n", column, "^", column, s);
}
