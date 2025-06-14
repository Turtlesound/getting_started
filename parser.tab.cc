// A Bison parser, made by GNU Bison 3.8.2.

// Skeleton implementation for Bison LALR(1) parsers in C++

// Copyright (C) 2002-2015, 2018-2021 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

// DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
// especially those whose name start with YY_ or yy_.  They are
// private implementation details that can be changed or removed.





#include "parser.tab.hh"


// Unqualified %code blocks.
#line 18 "parser.yy"

  #define YY_DECL yy::parser::symbol_type yylex()
  YY_DECL;
  
  Node* root;
  extern int yylineno;
  // Add these variables to track line numbers of specific tokens
  int class_line = 0;
  int method_line = 0;

#line 57 "parser.tab.cc"


#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> // FIXME: INFRINGES ON USER NAME SPACE.
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif


// Whether we are compiled with exception support.
#ifndef YY_EXCEPTIONS
# if defined __GNUC__ && !defined __EXCEPTIONS
#  define YY_EXCEPTIONS 0
# else
#  define YY_EXCEPTIONS 1
# endif
#endif



// Enable debugging if requested.
#if YYDEBUG

// A pseudo ostream that takes yydebug_ into account.
# define YYCDEBUG if (yydebug_) (*yycdebug_)

# define YY_SYMBOL_PRINT(Title, Symbol)         \
  do {                                          \
    if (yydebug_)                               \
    {                                           \
      *yycdebug_ << Title << ' ';               \
      yy_print_ (*yycdebug_, Symbol);           \
      *yycdebug_ << '\n';                       \
    }                                           \
  } while (false)

# define YY_REDUCE_PRINT(Rule)          \
  do {                                  \
    if (yydebug_)                       \
      yy_reduce_print_ (Rule);          \
  } while (false)

# define YY_STACK_PRINT()               \
  do {                                  \
    if (yydebug_)                       \
      yy_stack_print_ ();                \
  } while (false)

#else // !YYDEBUG

# define YYCDEBUG if (false) std::cerr
# define YY_SYMBOL_PRINT(Title, Symbol)  YY_USE (Symbol)
# define YY_REDUCE_PRINT(Rule)           static_cast<void> (0)
# define YY_STACK_PRINT()                static_cast<void> (0)

#endif // !YYDEBUG

#define yyerrok         (yyerrstatus_ = 0)
#define yyclearin       (yyla.clear ())

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYRECOVERING()  (!!yyerrstatus_)

namespace yy {
#line 130 "parser.tab.cc"

  /// Build a parser object.
  parser::parser ()
#if YYDEBUG
    : yydebug_ (false),
      yycdebug_ (&std::cerr)
#else

#endif
  {}

  parser::~parser ()
  {}

  parser::syntax_error::~syntax_error () YY_NOEXCEPT YY_NOTHROW
  {}

  /*---------.
  | symbol.  |
  `---------*/



  // by_state.
  parser::by_state::by_state () YY_NOEXCEPT
    : state (empty_state)
  {}

  parser::by_state::by_state (const by_state& that) YY_NOEXCEPT
    : state (that.state)
  {}

  void
  parser::by_state::clear () YY_NOEXCEPT
  {
    state = empty_state;
  }

  void
  parser::by_state::move (by_state& that)
  {
    state = that.state;
    that.clear ();
  }

  parser::by_state::by_state (state_type s) YY_NOEXCEPT
    : state (s)
  {}

  parser::symbol_kind_type
  parser::by_state::kind () const YY_NOEXCEPT
  {
    if (state == empty_state)
      return symbol_kind::S_YYEMPTY;
    else
      return YY_CAST (symbol_kind_type, yystos_[+state]);
  }

  parser::stack_symbol_type::stack_symbol_type ()
  {}

  parser::stack_symbol_type::stack_symbol_type (YY_RVREF (stack_symbol_type) that)
    : super_type (YY_MOVE (that.state))
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_main_class: // main_class
      case symbol_kind::S_class_declaration_list: // class_declaration_list
      case symbol_kind::S_class_declaration: // class_declaration
      case symbol_kind::S_var_declaration_list: // var_declaration_list
      case symbol_kind::S_var_declaration: // var_declaration
      case symbol_kind::S_type: // type
      case symbol_kind::S_base_type: // base_type
      case symbol_kind::S_type_identifier: // type_identifier
      case symbol_kind::S_array_type: // array_type
      case symbol_kind::S_method_declaration_list: // method_declaration_list
      case symbol_kind::S_method_declaration: // method_declaration
      case symbol_kind::S_method_body: // method_body
      case symbol_kind::S_return_statement: // return_statement
      case symbol_kind::S_non_return_statement_list: // non_return_statement_list
      case symbol_kind::S_param_list: // param_list
      case symbol_kind::S_statement_list: // statement_list
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_statement: // simple_statement
      case symbol_kind::S_compound_statement: // compound_statement
      case symbol_kind::S_expression_list: // expression_list
      case symbol_kind::S_non_empty_expression_list: // non_empty_expression_list
      case symbol_kind::S_expression: // expression
        value.YY_MOVE_OR_COPY< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_SUBOP: // SUBOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LESSOP: // LESSOP
      case symbol_kind::S_GREATOP: // GREATOP
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_MOD: // MOD
      case symbol_kind::S_EQUAL: // EQUAL
      case symbol_kind::S_LS: // LS
      case symbol_kind::S_RS: // RS
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSTEMOUTPRINTLN: // SYSTEMOUTPRINTLN
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_INTLITERAL: // INTLITERAL
      case symbol_kind::S_ASSIGN: // ASSIGN
        value.YY_MOVE_OR_COPY< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

#if 201103L <= YY_CPLUSPLUS
    // that is emptied.
    that.state = empty_state;
#endif
  }

  parser::stack_symbol_type::stack_symbol_type (state_type s, YY_MOVE_REF (symbol_type) that)
    : super_type (s)
  {
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_main_class: // main_class
      case symbol_kind::S_class_declaration_list: // class_declaration_list
      case symbol_kind::S_class_declaration: // class_declaration
      case symbol_kind::S_var_declaration_list: // var_declaration_list
      case symbol_kind::S_var_declaration: // var_declaration
      case symbol_kind::S_type: // type
      case symbol_kind::S_base_type: // base_type
      case symbol_kind::S_type_identifier: // type_identifier
      case symbol_kind::S_array_type: // array_type
      case symbol_kind::S_method_declaration_list: // method_declaration_list
      case symbol_kind::S_method_declaration: // method_declaration
      case symbol_kind::S_method_body: // method_body
      case symbol_kind::S_return_statement: // return_statement
      case symbol_kind::S_non_return_statement_list: // non_return_statement_list
      case symbol_kind::S_param_list: // param_list
      case symbol_kind::S_statement_list: // statement_list
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_statement: // simple_statement
      case symbol_kind::S_compound_statement: // compound_statement
      case symbol_kind::S_expression_list: // expression_list
      case symbol_kind::S_non_empty_expression_list: // non_empty_expression_list
      case symbol_kind::S_expression: // expression
        value.move< Node* > (YY_MOVE (that.value));
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_SUBOP: // SUBOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LESSOP: // LESSOP
      case symbol_kind::S_GREATOP: // GREATOP
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_MOD: // MOD
      case symbol_kind::S_EQUAL: // EQUAL
      case symbol_kind::S_LS: // LS
      case symbol_kind::S_RS: // RS
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSTEMOUTPRINTLN: // SYSTEMOUTPRINTLN
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_INTLITERAL: // INTLITERAL
      case symbol_kind::S_ASSIGN: // ASSIGN
        value.move< std::string > (YY_MOVE (that.value));
        break;

      default:
        break;
    }

    // that is emptied.
    that.kind_ = symbol_kind::S_YYEMPTY;
  }

#if YY_CPLUSPLUS < 201103L
  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (const stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_main_class: // main_class
      case symbol_kind::S_class_declaration_list: // class_declaration_list
      case symbol_kind::S_class_declaration: // class_declaration
      case symbol_kind::S_var_declaration_list: // var_declaration_list
      case symbol_kind::S_var_declaration: // var_declaration
      case symbol_kind::S_type: // type
      case symbol_kind::S_base_type: // base_type
      case symbol_kind::S_type_identifier: // type_identifier
      case symbol_kind::S_array_type: // array_type
      case symbol_kind::S_method_declaration_list: // method_declaration_list
      case symbol_kind::S_method_declaration: // method_declaration
      case symbol_kind::S_method_body: // method_body
      case symbol_kind::S_return_statement: // return_statement
      case symbol_kind::S_non_return_statement_list: // non_return_statement_list
      case symbol_kind::S_param_list: // param_list
      case symbol_kind::S_statement_list: // statement_list
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_statement: // simple_statement
      case symbol_kind::S_compound_statement: // compound_statement
      case symbol_kind::S_expression_list: // expression_list
      case symbol_kind::S_non_empty_expression_list: // non_empty_expression_list
      case symbol_kind::S_expression: // expression
        value.copy< Node* > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_SUBOP: // SUBOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LESSOP: // LESSOP
      case symbol_kind::S_GREATOP: // GREATOP
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_MOD: // MOD
      case symbol_kind::S_EQUAL: // EQUAL
      case symbol_kind::S_LS: // LS
      case symbol_kind::S_RS: // RS
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSTEMOUTPRINTLN: // SYSTEMOUTPRINTLN
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_INTLITERAL: // INTLITERAL
      case symbol_kind::S_ASSIGN: // ASSIGN
        value.copy< std::string > (that.value);
        break;

      default:
        break;
    }

    return *this;
  }

  parser::stack_symbol_type&
  parser::stack_symbol_type::operator= (stack_symbol_type& that)
  {
    state = that.state;
    switch (that.kind ())
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_main_class: // main_class
      case symbol_kind::S_class_declaration_list: // class_declaration_list
      case symbol_kind::S_class_declaration: // class_declaration
      case symbol_kind::S_var_declaration_list: // var_declaration_list
      case symbol_kind::S_var_declaration: // var_declaration
      case symbol_kind::S_type: // type
      case symbol_kind::S_base_type: // base_type
      case symbol_kind::S_type_identifier: // type_identifier
      case symbol_kind::S_array_type: // array_type
      case symbol_kind::S_method_declaration_list: // method_declaration_list
      case symbol_kind::S_method_declaration: // method_declaration
      case symbol_kind::S_method_body: // method_body
      case symbol_kind::S_return_statement: // return_statement
      case symbol_kind::S_non_return_statement_list: // non_return_statement_list
      case symbol_kind::S_param_list: // param_list
      case symbol_kind::S_statement_list: // statement_list
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_statement: // simple_statement
      case symbol_kind::S_compound_statement: // compound_statement
      case symbol_kind::S_expression_list: // expression_list
      case symbol_kind::S_non_empty_expression_list: // non_empty_expression_list
      case symbol_kind::S_expression: // expression
        value.move< Node* > (that.value);
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_SUBOP: // SUBOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LESSOP: // LESSOP
      case symbol_kind::S_GREATOP: // GREATOP
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_MOD: // MOD
      case symbol_kind::S_EQUAL: // EQUAL
      case symbol_kind::S_LS: // LS
      case symbol_kind::S_RS: // RS
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSTEMOUTPRINTLN: // SYSTEMOUTPRINTLN
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_INTLITERAL: // INTLITERAL
      case symbol_kind::S_ASSIGN: // ASSIGN
        value.move< std::string > (that.value);
        break;

      default:
        break;
    }

    // that is emptied.
    that.state = empty_state;
    return *this;
  }
#endif

  template <typename Base>
  void
  parser::yy_destroy_ (const char* yymsg, basic_symbol<Base>& yysym) const
  {
    if (yymsg)
      YY_SYMBOL_PRINT (yymsg, yysym);
  }

#if YYDEBUG
  template <typename Base>
  void
  parser::yy_print_ (std::ostream& yyo, const basic_symbol<Base>& yysym) const
  {
    std::ostream& yyoutput = yyo;
    YY_USE (yyoutput);
    if (yysym.empty ())
      yyo << "empty symbol";
    else
      {
        symbol_kind_type yykind = yysym.kind ();
        yyo << (yykind < YYNTOKENS ? "token" : "nterm")
            << ' ' << yysym.name () << " (";
        YY_USE (yykind);
        yyo << ')';
      }
  }
#endif

  void
  parser::yypush_ (const char* m, YY_MOVE_REF (stack_symbol_type) sym)
  {
    if (m)
      YY_SYMBOL_PRINT (m, sym);
    yystack_.push (YY_MOVE (sym));
  }

  void
  parser::yypush_ (const char* m, state_type s, YY_MOVE_REF (symbol_type) sym)
  {
#if 201103L <= YY_CPLUSPLUS
    yypush_ (m, stack_symbol_type (s, std::move (sym)));
#else
    stack_symbol_type ss (s, sym);
    yypush_ (m, ss);
#endif
  }

  void
  parser::yypop_ (int n) YY_NOEXCEPT
  {
    yystack_.pop (n);
  }

#if YYDEBUG
  std::ostream&
  parser::debug_stream () const
  {
    return *yycdebug_;
  }

  void
  parser::set_debug_stream (std::ostream& o)
  {
    yycdebug_ = &o;
  }


  parser::debug_level_type
  parser::debug_level () const
  {
    return yydebug_;
  }

  void
  parser::set_debug_level (debug_level_type l)
  {
    yydebug_ = l;
  }
#endif // YYDEBUG

  parser::state_type
  parser::yy_lr_goto_state_ (state_type yystate, int yysym)
  {
    int yyr = yypgoto_[yysym - YYNTOKENS] + yystate;
    if (0 <= yyr && yyr <= yylast_ && yycheck_[yyr] == yystate)
      return yytable_[yyr];
    else
      return yydefgoto_[yysym - YYNTOKENS];
  }

  bool
  parser::yy_pact_value_is_default_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yypact_ninf_;
  }

  bool
  parser::yy_table_value_is_error_ (int yyvalue) YY_NOEXCEPT
  {
    return yyvalue == yytable_ninf_;
  }

  int
  parser::operator() ()
  {
    return parse ();
  }

  int
  parser::parse ()
  {
    int yyn;
    /// Length of the RHS of the rule being reduced.
    int yylen = 0;

    // Error handling.
    int yynerrs_ = 0;
    int yyerrstatus_ = 0;

    /// The lookahead symbol.
    symbol_type yyla;

    /// The return value of parse ().
    int yyresult;

#if YY_EXCEPTIONS
    try
#endif // YY_EXCEPTIONS
      {
    YYCDEBUG << "Starting parse\n";


    /* Initialize the stack.  The initial state will be set in
       yynewstate, since the latter expects the semantical and the
       location values to have been already stored, initialize these
       stacks with a primary value.  */
    yystack_.clear ();
    yypush_ (YY_NULLPTR, 0, YY_MOVE (yyla));

  /*-----------------------------------------------.
  | yynewstate -- push a new symbol on the stack.  |
  `-----------------------------------------------*/
  yynewstate:
    YYCDEBUG << "Entering state " << int (yystack_[0].state) << '\n';
    YY_STACK_PRINT ();

    // Accept?
    if (yystack_[0].state == yyfinal_)
      YYACCEPT;

    goto yybackup;


  /*-----------.
  | yybackup.  |
  `-----------*/
  yybackup:
    // Try to take a decision without lookahead.
    yyn = yypact_[+yystack_[0].state];
    if (yy_pact_value_is_default_ (yyn))
      goto yydefault;

    // Read a lookahead token.
    if (yyla.empty ())
      {
        YYCDEBUG << "Reading a token\n";
#if YY_EXCEPTIONS
        try
#endif // YY_EXCEPTIONS
          {
            symbol_type yylookahead (yylex ());
            yyla.move (yylookahead);
          }
#if YY_EXCEPTIONS
        catch (const syntax_error& yyexc)
          {
            YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
            error (yyexc);
            goto yyerrlab1;
          }
#endif // YY_EXCEPTIONS
      }
    YY_SYMBOL_PRINT ("Next token is", yyla);

    if (yyla.kind () == symbol_kind::S_YYerror)
    {
      // The scanner already issued an error message, process directly
      // to error recovery.  But do not keep the error token as
      // lookahead, it is too special and may lead us to an endless
      // loop in error recovery. */
      yyla.kind_ = symbol_kind::S_YYUNDEF;
      goto yyerrlab1;
    }

    /* If the proper action on seeing token YYLA.TYPE is to reduce or
       to detect an error, take that action.  */
    yyn += yyla.kind ();
    if (yyn < 0 || yylast_ < yyn || yycheck_[yyn] != yyla.kind ())
      {
        goto yydefault;
      }

    // Reduce or error.
    yyn = yytable_[yyn];
    if (yyn <= 0)
      {
        if (yy_table_value_is_error_ (yyn))
          goto yyerrlab;
        yyn = -yyn;
        goto yyreduce;
      }

    // Count tokens shifted since error; after three, turn off error status.
    if (yyerrstatus_)
      --yyerrstatus_;

    // Shift the lookahead token.
    yypush_ ("Shifting", state_type (yyn), YY_MOVE (yyla));
    goto yynewstate;


  /*-----------------------------------------------------------.
  | yydefault -- do the default action for the current state.  |
  `-----------------------------------------------------------*/
  yydefault:
    yyn = yydefact_[+yystack_[0].state];
    if (yyn == 0)
      goto yyerrlab;
    goto yyreduce;


  /*-----------------------------.
  | yyreduce -- do a reduction.  |
  `-----------------------------*/
  yyreduce:
    yylen = yyr2_[yyn];
    {
      stack_symbol_type yylhs;
      yylhs.state = yy_lr_goto_state_ (yystack_[yylen].state, yyr1_[yyn]);
      /* Variants are always initialized to an empty instance of the
         correct type. The default '$$ = $1' action is NOT applied
         when using variants.  */
      switch (yyr1_[yyn])
    {
      case symbol_kind::S_root: // root
      case symbol_kind::S_goal: // goal
      case symbol_kind::S_main_class: // main_class
      case symbol_kind::S_class_declaration_list: // class_declaration_list
      case symbol_kind::S_class_declaration: // class_declaration
      case symbol_kind::S_var_declaration_list: // var_declaration_list
      case symbol_kind::S_var_declaration: // var_declaration
      case symbol_kind::S_type: // type
      case symbol_kind::S_base_type: // base_type
      case symbol_kind::S_type_identifier: // type_identifier
      case symbol_kind::S_array_type: // array_type
      case symbol_kind::S_method_declaration_list: // method_declaration_list
      case symbol_kind::S_method_declaration: // method_declaration
      case symbol_kind::S_method_body: // method_body
      case symbol_kind::S_return_statement: // return_statement
      case symbol_kind::S_non_return_statement_list: // non_return_statement_list
      case symbol_kind::S_param_list: // param_list
      case symbol_kind::S_statement_list: // statement_list
      case symbol_kind::S_statement: // statement
      case symbol_kind::S_simple_statement: // simple_statement
      case symbol_kind::S_compound_statement: // compound_statement
      case symbol_kind::S_expression_list: // expression_list
      case symbol_kind::S_non_empty_expression_list: // non_empty_expression_list
      case symbol_kind::S_expression: // expression
        yylhs.value.emplace< Node* > ();
        break;

      case symbol_kind::S_PLUSOP: // PLUSOP
      case symbol_kind::S_SUBOP: // SUBOP
      case symbol_kind::S_MULTOP: // MULTOP
      case symbol_kind::S_DIVOP: // DIVOP
      case symbol_kind::S_LP: // LP
      case symbol_kind::S_RP: // RP
      case symbol_kind::S_LESSOP: // LESSOP
      case symbol_kind::S_GREATOP: // GREATOP
      case symbol_kind::S_AND: // AND
      case symbol_kind::S_OR: // OR
      case symbol_kind::S_MOD: // MOD
      case symbol_kind::S_EQUAL: // EQUAL
      case symbol_kind::S_LS: // LS
      case symbol_kind::S_RS: // RS
      case symbol_kind::S_TRUE: // TRUE
      case symbol_kind::S_FALSE: // FALSE
      case symbol_kind::S_DOT: // DOT
      case symbol_kind::S_COMMA: // COMMA
      case symbol_kind::S_THIS: // THIS
      case symbol_kind::S_NEW: // NEW
      case symbol_kind::S_NOT: // NOT
      case symbol_kind::S_PUBLIC: // PUBLIC
      case symbol_kind::S_CLASS: // CLASS
      case symbol_kind::S_STATIC: // STATIC
      case symbol_kind::S_VOID: // VOID
      case symbol_kind::S_MAIN: // MAIN
      case symbol_kind::S_STRING: // STRING
      case symbol_kind::S_BOOLEAN: // BOOLEAN
      case symbol_kind::S_IF: // IF
      case symbol_kind::S_ELSE: // ELSE
      case symbol_kind::S_WHILE: // WHILE
      case symbol_kind::S_SYSTEMOUTPRINTLN: // SYSTEMOUTPRINTLN
      case symbol_kind::S_RETURN: // RETURN
      case symbol_kind::S_LENGTH: // LENGTH
      case symbol_kind::S_LBRACE: // LBRACE
      case symbol_kind::S_RBRACE: // RBRACE
      case symbol_kind::S_SEMICOLON: // SEMICOLON
      case symbol_kind::S_ID: // ID
      case symbol_kind::S_INT: // INT
      case symbol_kind::S_INTLITERAL: // INTLITERAL
      case symbol_kind::S_ASSIGN: // ASSIGN
        yylhs.value.emplace< std::string > ();
        break;

      default:
        break;
    }



      // Perform the reduction.
      YY_REDUCE_PRINT (yyn);
#if YY_EXCEPTIONS
      try
#endif // YY_EXCEPTIONS
        {
          switch (yyn)
            {
  case 2: // root: goal "end of file"
#line 61 "parser.yy"
                     { root = yystack_[1].value.as < Node* > (); }
#line 866 "parser.tab.cc"
    break;

  case 3: // goal: main_class class_declaration_list
#line 64 "parser.yy"
                                        { 
                yylhs.value.as < Node* > ()= new Node("Goal", "", yylineno);
                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
            }
#line 876 "parser.tab.cc"
    break;

  case 4: // $@1: %empty
#line 71 "parser.yy"
    { 
        method_line = yylineno; // Store the line number when PUBLIC token is seen 
    }
#line 884 "parser.tab.cc"
    break;

  case 5: // main_class: PUBLIC $@1 CLASS ID LBRACE PUBLIC STATIC VOID MAIN LP STRING LS RS ID RP LBRACE statement statement_list RBRACE RBRACE
#line 75 "parser.yy"
                                                     { 
        yylhs.value.as < Node* > () = new Node("MainClass", "", method_line); // Use stored line number
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[16].value.as < std::string > (), method_line));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
    }
#line 894 "parser.tab.cc"
    break;

  case 6: // class_declaration_list: class_declaration_list class_declaration
#line 82 "parser.yy"
     { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
     yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 901 "parser.tab.cc"
    break;

  case 7: // class_declaration_list: %empty
#line 84 "parser.yy"
                  { yylhs.value.as < Node* > () = new Node("ClassDeclarationList", "", yylineno); }
#line 907 "parser.tab.cc"
    break;

  case 8: // $@2: %empty
#line 87 "parser.yy"
    { 
        class_line = yylineno; // Store the line number when CLASS token is seen 
    }
#line 915 "parser.tab.cc"
    break;

  case 9: // class_declaration: CLASS $@2 ID LBRACE var_declaration_list method_declaration_list RBRACE
#line 91 "parser.yy"
    { 
        yylhs.value.as < Node* > () = new Node("ClassDeclaration", "", class_line); // Use stored line number
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[4].value.as < std::string > (), class_line));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 926 "parser.tab.cc"
    break;

  case 10: // var_declaration_list: var_declaration_list var_declaration
#line 99 "parser.yy"
    { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 932 "parser.tab.cc"
    break;

  case 11: // var_declaration_list: %empty
#line 100 "parser.yy"
                  { yylhs.value.as < Node* > () = new Node("VarDeclarationList", "", yylineno); }
#line 938 "parser.tab.cc"
    break;

  case 12: // var_declaration: type ID SEMICOLON
#line 103 "parser.yy"
                      {
        yylhs.value.as < Node* > () = new Node("VarDeclaration", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[1].value.as < std::string > (), yylineno));
    }
#line 948 "parser.tab.cc"
    break;

  case 13: // var_declaration: type LS RS ID SEMICOLON
#line 108 "parser.yy"
                              { 
  yylhs.value.as < Node* > () = new Node("VarDeclaration", "", yylineno);
  yylhs.value.as < Node* > ()->children.push_back(new Node("TypeArray", yystack_[4].value.as < Node* > ()->value, yylineno));
  yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[1].value.as < std::string > (), yylineno));
}
#line 958 "parser.tab.cc"
    break;

  case 14: // type: base_type
#line 114 "parser.yy"
                { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 964 "parser.tab.cc"
    break;

  case 15: // type: array_type
#line 115 "parser.yy"
                 { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 970 "parser.tab.cc"
    break;

  case 16: // type: BOOLEAN
#line 116 "parser.yy"
              { yylhs.value.as < Node* > () = new Node("Boolean", "boolean", yylineno); }
#line 976 "parser.tab.cc"
    break;

  case 17: // type: INT
#line 117 "parser.yy"
          { yylhs.value.as < Node* > () = new Node("Int", "int", yylineno); }
#line 982 "parser.tab.cc"
    break;

  case 18: // base_type: STRING
#line 121 "parser.yy"
           { yylhs.value.as < Node* > () = new Node("String", "string", yylineno); }
#line 988 "parser.tab.cc"
    break;

  case 19: // base_type: VOID
#line 122 "parser.yy"
           { yylhs.value.as < Node* > () = new Node("Void", "void", yylineno); }
#line 994 "parser.tab.cc"
    break;

  case 20: // base_type: type_identifier
#line 123 "parser.yy"
                      { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1000 "parser.tab.cc"
    break;

  case 21: // type_identifier: ID
#line 129 "parser.yy"
    { 
        yylhs.value.as < Node* > () = new Node("UserDefinedType", yystack_[0].value.as < std::string > (), yylineno); 
    }
#line 1008 "parser.tab.cc"
    break;

  case 22: // array_type: INT LS RS
#line 135 "parser.yy"
              { yylhs.value.as < Node* > () = new Node("IntArray", "int[]", yylineno); }
#line 1014 "parser.tab.cc"
    break;

  case 23: // array_type: base_type LS RS
#line 136 "parser.yy"
                      { 
        yylhs.value.as < Node* > () = new Node("ArrayType", yystack_[2].value.as < Node* > ()->value + "[]", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    }
#line 1023 "parser.tab.cc"
    break;

  case 24: // method_declaration_list: method_declaration_list method_declaration
#line 142 "parser.yy"
                                                                    { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 1029 "parser.tab.cc"
    break;

  case 25: // method_declaration_list: %empty
#line 143 "parser.yy"
                                      { yylhs.value.as < Node* > () = new Node("MethodDeclarationList", "", yylineno); }
#line 1035 "parser.tab.cc"
    break;

  case 26: // $@3: %empty
#line 146 "parser.yy"
    { 
        method_line = yylineno; // Store the line number when PUBLIC token is seen 
    }
#line 1043 "parser.tab.cc"
    break;

  case 27: // method_declaration: PUBLIC $@3 type ID LP param_list RP LBRACE method_body RBRACE
#line 150 "parser.yy"
    { 
        yylhs.value.as < Node* > () = new Node("MethodDeclaration", "", method_line); // Use stored line number
        yylhs.value.as < Node* > ()->children.push_back(new Node("Public", "public", method_line));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[7].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[6].value.as < std::string > (), method_line));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1056 "parser.tab.cc"
    break;

  case 28: // method_body: non_return_statement_list return_statement
#line 160 "parser.yy"
    {
        yylhs.value.as < Node* > () = new Node("MethodBody", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1066 "parser.tab.cc"
    break;

  case 29: // method_body: return_statement
#line 166 "parser.yy"
    {
        yylhs.value.as < Node* > () = new Node("MethodBody", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1075 "parser.tab.cc"
    break;

  case 30: // return_statement: RETURN expression SEMICOLON
#line 172 "parser.yy"
    {
        yylhs.value.as < Node* > () = new Node("ReturnStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1084 "parser.tab.cc"
    break;

  case 31: // non_return_statement_list: non_return_statement_list statement
#line 178 "parser.yy"
    {
        yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > ();
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1093 "parser.tab.cc"
    break;

  case 32: // non_return_statement_list: %empty
#line 183 "parser.yy"
    {
        yylhs.value.as < Node* > () = new Node("StatementList", "", yylineno);
    }
#line 1101 "parser.tab.cc"
    break;

  case 33: // param_list: type ID
#line 188 "parser.yy"
              {
          Node* param = new Node("Parameter", "", yylineno);
          param->children.push_back(yystack_[1].value.as < Node* > ());
          param->children.push_back(new Node("Identifier", yystack_[0].value.as < std::string > (), yylineno));
          yylhs.value.as < Node* > () = new Node("ParameterList", "", yylineno);
          yylhs.value.as < Node* > ()->children.push_back(param);
      }
#line 1113 "parser.tab.cc"
    break;

  case 34: // param_list: param_list COMMA type ID
#line 195 "parser.yy"
                               {
          Node* param = new Node("Parameter", "", yylineno);
          param->children.push_back(yystack_[1].value.as < Node* > ());
          param->children.push_back(new Node("Identifier", yystack_[0].value.as < std::string > (), yylineno));
          yylhs.value.as < Node* > () = yystack_[3].value.as < Node* > ();
          yylhs.value.as < Node* > ()->children.push_back(param);
      }
#line 1125 "parser.tab.cc"
    break;

  case 35: // param_list: %empty
#line 202 "parser.yy"
                  { yylhs.value.as < Node* > () = new Node("ParameterList", "", yylineno); }
#line 1131 "parser.tab.cc"
    break;

  case 36: // statement_list: statement_list statement
#line 205 "parser.yy"
                                             { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 1137 "parser.tab.cc"
    break;

  case 37: // statement_list: %empty
#line 206 "parser.yy"
                                            { yylhs.value.as < Node* > () = new Node("StatementList", "", yylineno); }
#line 1143 "parser.tab.cc"
    break;

  case 38: // statement: simple_statement
#line 209 "parser.yy"
                                             { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1149 "parser.tab.cc"
    break;

  case 39: // statement: compound_statement
#line 210 "parser.yy"
                                             { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1155 "parser.tab.cc"
    break;

  case 40: // statement: var_declaration
#line 211 "parser.yy"
                                             { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1161 "parser.tab.cc"
    break;

  case 41: // simple_statement: ID ASSIGN expression SEMICOLON
#line 216 "parser.yy"
    { 
        yylhs.value.as < Node* > () = new Node("Assignment", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[3].value.as < std::string > (), yylineno));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1171 "parser.tab.cc"
    break;

  case 42: // simple_statement: expression SEMICOLON
#line 222 "parser.yy"
    {
        yylhs.value.as < Node* > () = new Node("ExpressionStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1180 "parser.tab.cc"
    break;

  case 43: // simple_statement: SYSTEMOUTPRINTLN LP expression RP SEMICOLON
#line 226 "parser.yy"
                                                  {
        yylhs.value.as < Node* > () = new Node("PrintStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
    }
#line 1189 "parser.tab.cc"
    break;

  case 44: // simple_statement: ID LS expression RS ASSIGN expression SEMICOLON
#line 230 "parser.yy"
                                                      {
        yylhs.value.as < Node* > () = new Node("ArrayAssignment", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[6].value.as < std::string > (), yylineno));
        yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1200 "parser.tab.cc"
    break;

  case 45: // compound_statement: IF LP expression RP statement
#line 239 "parser.yy"
                                            { 
        yylhs.value.as < Node* > () = new Node("IfStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1210 "parser.tab.cc"
    break;

  case 46: // compound_statement: IF LP expression RP statement ELSE statement
#line 244 "parser.yy"
                                                   {
        yylhs.value.as < Node* > () = new Node("IfElseStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[4].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1221 "parser.tab.cc"
    break;

  case 47: // compound_statement: WHILE LP expression RP statement
#line 250 "parser.yy"
                                            {
        yylhs.value.as < Node* > () = new Node("WhileStatement", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
        yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
    }
#line 1231 "parser.tab.cc"
    break;

  case 48: // compound_statement: LBRACE statement_list RBRACE
#line 255 "parser.yy"
                                           {
        yylhs.value.as < Node* > () = new Node("Block", "", yylineno);
        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
    }
#line 1240 "parser.tab.cc"
    break;

  case 49: // expression_list: non_empty_expression_list
#line 262 "parser.yy"
    { yylhs.value.as < Node* > () = yystack_[0].value.as < Node* > (); }
#line 1246 "parser.tab.cc"
    break;

  case 50: // expression_list: %empty
#line 263 "parser.yy"
                  { yylhs.value.as < Node* > () = new Node("ExpressionList", "", yylineno); }
#line 1252 "parser.tab.cc"
    break;

  case 51: // non_empty_expression_list: expression
#line 266 "parser.yy"
               { yylhs.value.as < Node* > () = new Node("ExpressionList", "", yylineno); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 1258 "parser.tab.cc"
    break;

  case 52: // non_empty_expression_list: non_empty_expression_list COMMA expression
#line 267 "parser.yy"
                                                 { yylhs.value.as < Node* > () = yystack_[2].value.as < Node* > (); yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ()); }
#line 1264 "parser.tab.cc"
    break;

  case 53: // expression: expression PLUSOP expression
#line 270 "parser.yy"
                                         {      /*
                                                  Create a subtree that corresponds to the AddExpression
                                                  The root of the subtree is AddExpression
                                                  The childdren of the AddExpression subtree are the left hand side (expression accessed through $1) and right hand side of the expression (expression accessed through $3)
                                                */
                            yylhs.value.as < Node* > () = new Node("AddExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                            /* printf("r1 "); */
                          }
#line 1279 "parser.tab.cc"
    break;

  case 54: // expression: expression SUBOP expression
#line 280 "parser.yy"
                                          {
                            yylhs.value.as < Node* > () = new Node("SubExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                            /* printf("r2 "); */
                          }
#line 1290 "parser.tab.cc"
    break;

  case 55: // expression: expression MULTOP expression
#line 286 "parser.yy"
                                           {
                            yylhs.value.as < Node* > () = new Node("MultExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                            /* printf("r3 "); */
                          }
#line 1301 "parser.tab.cc"
    break;

  case 56: // expression: expression DIVOP expression
#line 292 "parser.yy"
                                          {
                            yylhs.value.as < Node* > () = new Node("DivExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          }
#line 1311 "parser.tab.cc"
    break;

  case 57: // expression: expression LESSOP expression
#line 297 "parser.yy"
                                           {
                            yylhs.value.as < Node* > () = new Node("LessThanExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                            /* printf("r6 "); */
                          }
#line 1322 "parser.tab.cc"
    break;

  case 58: // expression: expression GREATOP expression
#line 303 "parser.yy"
                                            {
                            yylhs.value.as < Node* > () = new Node("GreaterThanExpression", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                            /* printf("r7 "); */
                          }
#line 1333 "parser.tab.cc"
    break;

  case 59: // expression: expression AND expression
#line 309 "parser.yy"
                                       {
                          yylhs.value.as < Node* > () = new Node ("AndExpression", "", yylineno);
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          /* printf("r7"); */
                        }
#line 1344 "parser.tab.cc"
    break;

  case 60: // expression: expression OR expression
#line 315 "parser.yy"
                                      {
                          yylhs.value.as < Node* > () = new Node ("OrExpression", "", yylineno);
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          /* printf("r8"); */
                        }
#line 1355 "parser.tab.cc"
    break;

  case 61: // expression: expression EQUAL expression
#line 321 "parser.yy"
                                         {
                          yylhs.value.as < Node* > () = new Node ("EqualExpression", "", yylineno);
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          /* printf("r9"); */
                        }
#line 1366 "parser.tab.cc"
    break;

  case 62: // expression: NOT expression
#line 327 "parser.yy"
                            {
                          yylhs.value.as < Node* > () = new Node ("NotExpression", "", yylineno);
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          /* printf("r10"); */
                        }
#line 1376 "parser.tab.cc"
    break;

  case 63: // expression: expression MOD expression
#line 332 "parser.yy"
                                       {
                          yylhs.value.as < Node* > () = new Node ("ModExpression", "", yylineno);
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                          yylhs.value.as < Node* > ()->children.push_back(yystack_[0].value.as < Node* > ());
                          /* printf("r11"); */
                        }
#line 1387 "parser.tab.cc"
    break;

  case 64: // expression: NEW type LS expression RS
#line 338 "parser.yy"
                                        { yylhs.value.as < Node* > () = new Node("NewArray", "", yylineno);
                                        yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                                      }
#line 1395 "parser.tab.cc"
    break;

  case 65: // expression: expression DOT LENGTH
#line 341 "parser.yy"
                                    { yylhs.value.as < Node* > () = new Node("LengthExpression", "", yylineno);
                                        yylhs.value.as < Node* > ()->children.push_back(yystack_[2].value.as < Node* > ());
                                    }
#line 1403 "parser.tab.cc"
    break;

  case 66: // expression: expression DOT ID LP expression_list RP
#line 344 "parser.yy"
                                                      { yylhs.value.as < Node* > () = new Node("MethodCall", "", yylineno);
                                                            yylhs.value.as < Node* > ()->children.push_back(yystack_[5].value.as < Node* > ());
                                                            yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[3].value.as < std::string > (), yylineno));
                                                            yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                                                        }
#line 1413 "parser.tab.cc"
    break;

  case 67: // expression: expression LS expression RS
#line 350 "parser.yy"
                        {
                            yylhs.value.as < Node* > () = new Node("ArrayAccess", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[3].value.as < Node* > ());
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                        }
#line 1423 "parser.tab.cc"
    break;

  case 68: // expression: ID LS expression RS
#line 356 "parser.yy"
                        {
                            yylhs.value.as < Node* > () = new Node("ArrayAccess", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[3].value.as < std::string > (), yylineno));
                            yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                        }
#line 1433 "parser.tab.cc"
    break;

  case 69: // expression: LP expression RP
#line 361 "parser.yy"
                               { yylhs.value.as < Node* > () = yystack_[1].value.as < Node* > (); /* printf("r6 ");  simply return the expression */}
#line 1439 "parser.tab.cc"
    break;

  case 70: // expression: ID
#line 363 "parser.yy"
            { 
                yylhs.value.as < Node* > () = new Node("Identifier", yystack_[0].value.as < std::string > (), yylineno); 
            }
#line 1447 "parser.tab.cc"
    break;

  case 71: // expression: TRUE
#line 366 "parser.yy"
                           { yylhs.value.as < Node* > () = new Node("True", "", yylineno); /* printf("r8 "); */}
#line 1453 "parser.tab.cc"
    break;

  case 72: // expression: FALSE
#line 367 "parser.yy"
                           { yylhs.value.as < Node* > () = new Node("False", "", yylineno); /* printf("r9 "); */}
#line 1459 "parser.tab.cc"
    break;

  case 73: // expression: THIS
#line 368 "parser.yy"
                           { yylhs.value.as < Node* > () = new Node("This", "", yylineno); /* printf("r10 "); */}
#line 1465 "parser.tab.cc"
    break;

  case 74: // expression: INTLITERAL
#line 369 "parser.yy"
                         { yylhs.value.as < Node* > () = new Node("IntegerLiteral", yystack_[0].value.as < std::string > (), yylineno); /* printf("r12 "); */}
#line 1471 "parser.tab.cc"
    break;

  case 75: // expression: NEW ID LP RP
#line 370 "parser.yy"
                           { yylhs.value.as < Node* > () = new Node("NewObject", "", yylineno);
                            yylhs.value.as < Node* > ()->children.push_back(new Node("Identifier", yystack_[2].value.as < std::string > (), yylineno));
                          }
#line 1479 "parser.tab.cc"
    break;

  case 76: // expression: INT LS expression RS
#line 373 "parser.yy"
                                   { yylhs.value.as < Node* > () = new Node("IntArray", "", yylineno);
                                    yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ());
                                  }
#line 1487 "parser.tab.cc"
    break;

  case 77: // expression: NEW INT LS expression RS
#line 376 "parser.yy"
                                       {
                yylhs.value.as < Node* > () = new Node("NewIntArray", "", yylineno);
                yylhs.value.as < Node* > ()->children.push_back(new Node("New", "new", yylineno));
                yylhs.value.as < Node* > ()->children.push_back(new Node("Type", "int", yylineno));
                yylhs.value.as < Node* > ()->children.push_back(yystack_[1].value.as < Node* > ()); // Storleken på arrayen
            }
#line 1498 "parser.tab.cc"
    break;


#line 1502 "parser.tab.cc"

            default:
              break;
            }
        }
#if YY_EXCEPTIONS
      catch (const syntax_error& yyexc)
        {
          YYCDEBUG << "Caught exception: " << yyexc.what() << '\n';
          error (yyexc);
          YYERROR;
        }
#endif // YY_EXCEPTIONS
      YY_SYMBOL_PRINT ("-> $$ =", yylhs);
      yypop_ (yylen);
      yylen = 0;

      // Shift the result of the reduction.
      yypush_ (YY_NULLPTR, YY_MOVE (yylhs));
    }
    goto yynewstate;


  /*--------------------------------------.
  | yyerrlab -- here on detecting error.  |
  `--------------------------------------*/
  yyerrlab:
    // If not already recovering from an error, report this error.
    if (!yyerrstatus_)
      {
        ++yynerrs_;
        context yyctx (*this, yyla);
        std::string msg = yysyntax_error_ (yyctx);
        error (YY_MOVE (msg));
      }


    if (yyerrstatus_ == 3)
      {
        /* If just tried and failed to reuse lookahead token after an
           error, discard it.  */

        // Return failure if at end of input.
        if (yyla.kind () == symbol_kind::S_YYEOF)
          YYABORT;
        else if (!yyla.empty ())
          {
            yy_destroy_ ("Error: discarding", yyla);
            yyla.clear ();
          }
      }

    // Else will try to reuse lookahead token after shifting the error token.
    goto yyerrlab1;


  /*---------------------------------------------------.
  | yyerrorlab -- error raised explicitly by YYERROR.  |
  `---------------------------------------------------*/
  yyerrorlab:
    /* Pacify compilers when the user code never invokes YYERROR and
       the label yyerrorlab therefore never appears in user code.  */
    if (false)
      YYERROR;

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYERROR.  */
    yypop_ (yylen);
    yylen = 0;
    YY_STACK_PRINT ();
    goto yyerrlab1;


  /*-------------------------------------------------------------.
  | yyerrlab1 -- common code for both syntax error and YYERROR.  |
  `-------------------------------------------------------------*/
  yyerrlab1:
    yyerrstatus_ = 3;   // Each real token shifted decrements this.
    // Pop stack until we find a state that shifts the error token.
    for (;;)
      {
        yyn = yypact_[+yystack_[0].state];
        if (!yy_pact_value_is_default_ (yyn))
          {
            yyn += symbol_kind::S_YYerror;
            if (0 <= yyn && yyn <= yylast_
                && yycheck_[yyn] == symbol_kind::S_YYerror)
              {
                yyn = yytable_[yyn];
                if (0 < yyn)
                  break;
              }
          }

        // Pop the current state because it cannot handle the error token.
        if (yystack_.size () == 1)
          YYABORT;

        yy_destroy_ ("Error: popping", yystack_[0]);
        yypop_ ();
        YY_STACK_PRINT ();
      }
    {
      stack_symbol_type error_token;


      // Shift the error token.
      error_token.state = state_type (yyn);
      yypush_ ("Shifting", YY_MOVE (error_token));
    }
    goto yynewstate;


  /*-------------------------------------.
  | yyacceptlab -- YYACCEPT comes here.  |
  `-------------------------------------*/
  yyacceptlab:
    yyresult = 0;
    goto yyreturn;


  /*-----------------------------------.
  | yyabortlab -- YYABORT comes here.  |
  `-----------------------------------*/
  yyabortlab:
    yyresult = 1;
    goto yyreturn;


  /*-----------------------------------------------------.
  | yyreturn -- parsing is finished, return the result.  |
  `-----------------------------------------------------*/
  yyreturn:
    if (!yyla.empty ())
      yy_destroy_ ("Cleanup: discarding lookahead", yyla);

    /* Do not reclaim the symbols of the rule whose action triggered
       this YYABORT or YYACCEPT.  */
    yypop_ (yylen);
    YY_STACK_PRINT ();
    while (1 < yystack_.size ())
      {
        yy_destroy_ ("Cleanup: popping", yystack_[0]);
        yypop_ ();
      }

    return yyresult;
  }
#if YY_EXCEPTIONS
    catch (...)
      {
        YYCDEBUG << "Exception caught: cleaning lookahead and stack\n";
        // Do not try to display the values of the reclaimed symbols,
        // as their printers might throw an exception.
        if (!yyla.empty ())
          yy_destroy_ (YY_NULLPTR, yyla);

        while (1 < yystack_.size ())
          {
            yy_destroy_ (YY_NULLPTR, yystack_[0]);
            yypop_ ();
          }
        throw;
      }
#endif // YY_EXCEPTIONS
  }

  void
  parser::error (const syntax_error& yyexc)
  {
    error (yyexc.what ());
  }

  /* Return YYSTR after stripping away unnecessary quotes and
     backslashes, so that it's suitable for yyerror.  The heuristic is
     that double-quoting is unnecessary unless the string contains an
     apostrophe, a comma, or backslash (other than backslash-backslash).
     YYSTR is taken from yytname.  */
  std::string
  parser::yytnamerr_ (const char *yystr)
  {
    if (*yystr == '"')
      {
        std::string yyr;
        char const *yyp = yystr;

        for (;;)
          switch (*++yyp)
            {
            case '\'':
            case ',':
              goto do_not_strip_quotes;

            case '\\':
              if (*++yyp != '\\')
                goto do_not_strip_quotes;
              else
                goto append;

            append:
            default:
              yyr += *yyp;
              break;

            case '"':
              return yyr;
            }
      do_not_strip_quotes: ;
      }

    return yystr;
  }

  std::string
  parser::symbol_name (symbol_kind_type yysymbol)
  {
    return yytnamerr_ (yytname_[yysymbol]);
  }



  // parser::context.
  parser::context::context (const parser& yyparser, const symbol_type& yyla)
    : yyparser_ (yyparser)
    , yyla_ (yyla)
  {}

  int
  parser::context::expected_tokens (symbol_kind_type yyarg[], int yyargn) const
  {
    // Actual number of expected tokens
    int yycount = 0;

    const int yyn = yypact_[+yyparser_.yystack_[0].state];
    if (!yy_pact_value_is_default_ (yyn))
      {
        /* Start YYX at -YYN if negative to avoid negative indexes in
           YYCHECK.  In other words, skip the first -YYN actions for
           this state because they are default actions.  */
        const int yyxbegin = yyn < 0 ? -yyn : 0;
        // Stay within bounds of both yycheck and yytname.
        const int yychecklim = yylast_ - yyn + 1;
        const int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
        for (int yyx = yyxbegin; yyx < yyxend; ++yyx)
          if (yycheck_[yyx + yyn] == yyx && yyx != symbol_kind::S_YYerror
              && !yy_table_value_is_error_ (yytable_[yyx + yyn]))
            {
              if (!yyarg)
                ++yycount;
              else if (yycount == yyargn)
                return 0;
              else
                yyarg[yycount++] = YY_CAST (symbol_kind_type, yyx);
            }
      }

    if (yyarg && yycount == 0 && 0 < yyargn)
      yyarg[0] = symbol_kind::S_YYEMPTY;
    return yycount;
  }






  int
  parser::yy_syntax_error_arguments_ (const context& yyctx,
                                                 symbol_kind_type yyarg[], int yyargn) const
  {
    /* There are many possibilities here to consider:
       - If this state is a consistent state with a default action, then
         the only way this function was invoked is if the default action
         is an error action.  In that case, don't check for expected
         tokens because there are none.
       - The only way there can be no lookahead present (in yyla) is
         if this state is a consistent state with a default action.
         Thus, detecting the absence of a lookahead is sufficient to
         determine that there is no unexpected or expected token to
         report.  In that case, just report a simple "syntax error".
       - Don't assume there isn't a lookahead just because this state is
         a consistent state with a default action.  There might have
         been a previous inconsistent state, consistent state with a
         non-default action, or user semantic action that manipulated
         yyla.  (However, yyla is currently not documented for users.)
       - Of course, the expected token list depends on states to have
         correct lookahead information, and it depends on the parser not
         to perform extra reductions after fetching a lookahead from the
         scanner and before detecting a syntax error.  Thus, state merging
         (from LALR or IELR) and default reductions corrupt the expected
         token list.  However, the list is correct for canonical LR with
         one exception: it will still contain any token that will not be
         accepted due to an error action in a later state.
    */

    if (!yyctx.lookahead ().empty ())
      {
        if (yyarg)
          yyarg[0] = yyctx.token ();
        int yyn = yyctx.expected_tokens (yyarg ? yyarg + 1 : yyarg, yyargn - 1);
        return yyn + 1;
      }
    return 0;
  }

  // Generate an error message.
  std::string
  parser::yysyntax_error_ (const context& yyctx) const
  {
    // Its maximum.
    enum { YYARGS_MAX = 5 };
    // Arguments of yyformat.
    symbol_kind_type yyarg[YYARGS_MAX];
    int yycount = yy_syntax_error_arguments_ (yyctx, yyarg, YYARGS_MAX);

    char const* yyformat = YY_NULLPTR;
    switch (yycount)
      {
#define YYCASE_(N, S)                         \
        case N:                               \
          yyformat = S;                       \
        break
      default: // Avoid compiler warnings.
        YYCASE_ (0, YY_("syntax error"));
        YYCASE_ (1, YY_("syntax error, unexpected %s"));
        YYCASE_ (2, YY_("syntax error, unexpected %s, expecting %s"));
        YYCASE_ (3, YY_("syntax error, unexpected %s, expecting %s or %s"));
        YYCASE_ (4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
        YYCASE_ (5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
      }

    std::string yyres;
    // Argument number.
    std::ptrdiff_t yyi = 0;
    for (char const* yyp = yyformat; *yyp; ++yyp)
      if (yyp[0] == '%' && yyp[1] == 's' && yyi < yycount)
        {
          yyres += symbol_name (yyarg[yyi++]);
          ++yyp;
        }
      else
        yyres += *yyp;
    return yyres;
  }


  const signed char parser::yypact_ninf_ = -65;

  const signed char parser::yytable_ninf_ = -22;

  const short
  parser::yypact_[] =
  {
     -17,   -65,    19,    28,   -65,    25,   -65,   -65,    26,    13,
     -65,   -65,    17,    18,    33,    29,    39,   -65,    47,    72,
      48,   -65,   -65,   -65,   -65,    60,   -65,   -13,    67,   -65,
     -65,   -15,    76,    82,    84,    52,    88,   -65,   -65,   -65,
      66,   -65,    65,   -65,   -65,    72,    92,    89,    90,   113,
     -65,   130,    96,    72,   137,   106,    -5,   115,   -65,   116,
      72,   225,   114,   110,   261,   -65,   -65,   -65,    79,   261,
     147,   148,   149,   -65,     9,   144,   -65,   -65,   -65,   -65,
     -65,    58,   261,   122,   -65,   140,   -65,   150,   151,   301,
     157,   163,   164,   -11,   261,   261,   261,   169,   261,   261,
     253,   197,   261,   261,   261,   261,   261,   261,   261,   261,
     261,   261,   261,    -3,   -65,    75,   -65,   -65,   -65,   261,
     261,   -65,   175,   253,   261,   318,   335,   352,   -65,   -65,
     369,   112,   386,   134,   517,   517,   -11,   -11,   522,   522,
     505,   488,   -11,     7,   403,   -65,   177,   -65,   420,   -65,
     437,   454,   225,   225,   146,   145,   -65,   -65,   -65,   -65,
     261,   -65,   -65,   -65,   161,   -65,   -65,   261,   181,   174,
     471,   225,   129,   -65,   261,   -65,   -65,   471
  };

  const signed char
  parser::yydefact_[] =
  {
       0,     4,     0,     0,     7,     0,     1,     2,     3,     0,
       8,     6,     0,     0,     0,     0,     0,    11,     0,    25,
       0,    19,    18,    16,    21,    17,    10,     0,    14,    20,
      15,     0,     0,     0,     0,     0,     0,    26,     9,    24,
       0,    22,     0,    12,    23,     0,     0,     0,     0,     0,
      13,     0,     0,    35,     0,     0,     0,     0,    33,     0,
       0,     0,    32,     0,     0,    71,    72,    73,     0,     0,
       0,     0,     0,    37,    70,    17,    74,    40,    37,    38,
      39,     0,     0,     0,    29,     0,    34,    70,     0,     0,
      21,     0,     0,    62,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    42,     0,    27,    28,    31,     0,
       0,    69,     0,     0,     0,     0,     0,     0,    48,    36,
       0,     0,     0,     0,    53,    54,    55,    56,    57,    58,
      59,    60,    63,    61,     0,    65,     0,    30,     0,    75,
       0,     0,     0,     0,     0,    68,    41,    76,     5,    67,
      50,    68,    77,    64,    45,    47,    43,     0,     0,    49,
      51,     0,     0,    66,     0,    46,    44,    52
  };

  const short
  parser::yypgoto_[] =
  {
     -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   176,
     -39,   -65,   -65,   -65,   -65,   -65,   -65,   -65,   120,   -65,
     -65,   119,   -60,   -65,   -65,   -65,   -65,   -64
  };

  const unsigned char
  parser::yydefgoto_[] =
  {
       0,     2,     3,     4,     5,     8,    11,    13,    19,    77,
      27,    28,    29,    30,    31,    39,    45,    83,    84,    85,
      56,    97,   129,    79,    80,   168,   169,    81
  };

  const short
  parser::yytable_[] =
  {
      89,    78,    34,    59,   112,    93,    48,     1,   113,    37,
     102,   103,   104,   105,    55,    60,   106,   107,   115,     6,
     110,    63,   112,    38,    98,   118,   113,    35,     7,    92,
     125,   126,   127,   145,   130,   131,   132,   146,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   143,   144,   -21,
       9,    10,    99,    12,    14,   148,   132,    16,    15,   150,
     151,   102,   103,   104,   105,    18,    17,   106,   107,   108,
     109,   110,   111,   112,    20,    33,    32,   113,   102,   103,
     104,   105,    36,    40,   106,   107,   108,   109,   110,   111,
     112,    43,   164,   165,   113,    46,   170,   114,    41,    21,
      42,    22,    23,   172,    44,    47,    21,    49,    22,    23,
     177,   175,    24,    25,   147,   102,   103,   104,   105,    90,
      91,   106,   107,   108,   109,   110,   111,   112,    50,    52,
      51,   113,   102,   103,   104,   105,    54,    53,   106,   107,
     108,   109,   110,   111,   112,    57,    58,    64,   113,    82,
      86,   156,    61,    62,    94,    95,    96,    65,    66,   100,
     116,    67,    68,    69,   122,   119,   120,    21,   176,    22,
      23,    70,   158,    71,    72,    82,    64,    73,   123,   124,
      74,    75,    76,   149,   160,   166,    65,    66,   167,   173,
      67,    68,    69,   171,   174,    26,    21,   101,    22,    23,
      70,     0,    71,    72,    64,   117,    73,   128,     0,    74,
      75,    76,     0,     0,    65,    66,     0,     0,    67,    68,
      69,     0,     0,     0,    21,     0,    22,    23,    70,     0,
      71,    72,    64,     0,    73,   133,     0,    74,    75,    76,
       0,     0,    65,    66,     0,     0,    67,    68,    69,     0,
       0,     0,    21,     0,    22,    23,    70,     0,    71,    72,
      64,     0,    73,     0,     0,    74,    75,    76,    64,    41,
      65,    66,     0,     0,    67,    68,    69,     0,    65,    66,
       0,     0,    67,    68,    69,     0,     0,     0,     0,     0,
       0,     0,     0,    87,    88,    76,     0,     0,     0,     0,
       0,    87,    88,    76,   102,   103,   104,   105,     0,   121,
     106,   107,   108,   109,   110,   111,   112,     0,     0,     0,
     113,   102,   103,   104,   105,     0,   152,   106,   107,   108,
     109,   110,   111,   112,     0,     0,     0,   113,   102,   103,
     104,   105,     0,   153,   106,   107,   108,   109,   110,   111,
     112,     0,     0,     0,   113,   102,   103,   104,   105,     0,
     154,   106,   107,   108,   109,   110,   111,   112,     0,     0,
       0,   113,   102,   103,   104,   105,     0,     0,   106,   107,
     108,   109,   110,   111,   112,   155,     0,     0,   113,   102,
     103,   104,   105,     0,     0,   106,   107,   108,   109,   110,
     111,   112,   157,     0,     0,   113,   102,   103,   104,   105,
       0,     0,   106,   107,   108,   109,   110,   111,   112,   159,
       0,     0,   113,   102,   103,   104,   105,     0,     0,   106,
     107,   108,   109,   110,   111,   112,   161,     0,     0,   113,
     102,   103,   104,   105,     0,     0,   106,   107,   108,   109,
     110,   111,   112,   162,     0,     0,   113,   102,   103,   104,
     105,     0,     0,   106,   107,   108,   109,   110,   111,   112,
     163,     0,     0,   113,   102,   103,   104,   105,     0,     0,
     106,   107,   108,   109,   110,   111,   112,     0,     0,     0,
     113,   102,   103,   104,   105,     0,     0,   106,   107,   108,
       0,   110,   111,   112,     0,     0,     0,   113,   102,   103,
     104,   105,     0,     0,   106,   107,     0,     0,   110,   111,
     112,     0,   104,   105,   113,   102,   103,   104,   105,     0,
     110,     0,   112,     0,     0,   110,   113,   112,     0,     0,
       0,   113
  };

  const short
  parser::yycheck_[] =
  {
      64,    61,    15,     8,    15,    69,    45,    24,    19,    24,
       3,     4,     5,     6,    53,    20,     9,    10,    82,     0,
      13,    60,    15,    38,    15,    85,    19,    40,     0,    68,
      94,    95,    96,    36,    98,    99,   100,    40,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   111,   112,    40,
      25,    25,    43,    40,    37,   119,   120,    24,    40,   123,
     124,     3,     4,     5,     6,    26,    37,     9,    10,    11,
      12,    13,    14,    15,    27,    15,    28,    19,     3,     4,
       5,     6,    15,     7,     9,    10,    11,    12,    13,    14,
      15,    39,   152,   153,    19,    29,   160,    39,    16,    27,
      16,    29,    30,   167,    16,    40,    27,    15,    29,    30,
     174,   171,    40,    41,    39,     3,     4,     5,     6,    40,
      41,     9,    10,    11,    12,    13,    14,    15,    39,    16,
      40,    19,     3,     4,     5,     6,    40,     7,     9,    10,
      11,    12,    13,    14,    15,     8,    40,     7,    19,    35,
      40,    39,    37,    37,     7,     7,     7,    17,    18,    15,
      38,    21,    22,    23,     7,    15,    15,    27,    39,    29,
      30,    31,    38,    33,    34,    35,     7,    37,    15,    15,
      40,    41,    42,     8,     7,    39,    17,    18,    43,     8,
      21,    22,    23,    32,    20,    19,    27,    78,    29,    30,
      31,    -1,    33,    34,     7,    85,    37,    38,    -1,    40,
      41,    42,    -1,    -1,    17,    18,    -1,    -1,    21,    22,
      23,    -1,    -1,    -1,    27,    -1,    29,    30,    31,    -1,
      33,    34,     7,    -1,    37,    38,    -1,    40,    41,    42,
      -1,    -1,    17,    18,    -1,    -1,    21,    22,    23,    -1,
      -1,    -1,    27,    -1,    29,    30,    31,    -1,    33,    34,
       7,    -1,    37,    -1,    -1,    40,    41,    42,     7,    16,
      17,    18,    -1,    -1,    21,    22,    23,    -1,    17,    18,
      -1,    -1,    21,    22,    23,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    40,    41,    42,    -1,    -1,    -1,    -1,
      -1,    40,    41,    42,     3,     4,     5,     6,    -1,     8,
       9,    10,    11,    12,    13,    14,    15,    -1,    -1,    -1,
      19,     3,     4,     5,     6,    -1,     8,     9,    10,    11,
      12,    13,    14,    15,    -1,    -1,    -1,    19,     3,     4,
       5,     6,    -1,     8,     9,    10,    11,    12,    13,    14,
      15,    -1,    -1,    -1,    19,     3,     4,     5,     6,    -1,
       8,     9,    10,    11,    12,    13,    14,    15,    -1,    -1,
      -1,    19,     3,     4,     5,     6,    -1,    -1,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    19,     3,
       4,     5,     6,    -1,    -1,     9,    10,    11,    12,    13,
      14,    15,    16,    -1,    -1,    19,     3,     4,     5,     6,
      -1,    -1,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    19,     3,     4,     5,     6,    -1,    -1,     9,
      10,    11,    12,    13,    14,    15,    16,    -1,    -1,    19,
       3,     4,     5,     6,    -1,    -1,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    19,     3,     4,     5,
       6,    -1,    -1,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    19,     3,     4,     5,     6,    -1,    -1,
       9,    10,    11,    12,    13,    14,    15,    -1,    -1,    -1,
      19,     3,     4,     5,     6,    -1,    -1,     9,    10,    11,
      -1,    13,    14,    15,    -1,    -1,    -1,    19,     3,     4,
       5,     6,    -1,    -1,     9,    10,    -1,    -1,    13,    14,
      15,    -1,     5,     6,    19,     3,     4,     5,     6,    -1,
      13,    -1,    15,    -1,    -1,    13,    19,    15,    -1,    -1,
      -1,    19
  };

  const signed char
  parser::yystos_[] =
  {
       0,    24,    45,    46,    47,    48,     0,     0,    49,    25,
      25,    50,    40,    51,    37,    40,    24,    37,    26,    52,
      27,    27,    29,    30,    40,    41,    53,    54,    55,    56,
      57,    58,    28,    15,    15,    40,    15,    24,    38,    59,
       7,    16,    16,    39,    16,    60,    29,    40,    54,    15,
      39,    40,    16,     7,    40,    54,    64,     8,    40,     8,
      20,    37,    37,    54,     7,    17,    18,    21,    22,    23,
      31,    33,    34,    37,    40,    41,    42,    53,    66,    67,
      68,    71,    35,    61,    62,    63,    40,    40,    41,    71,
      40,    41,    54,    71,     7,     7,     7,    65,    15,    43,
      15,    65,     3,     4,     5,     6,     9,    10,    11,    12,
      13,    14,    15,    19,    39,    71,    38,    62,    66,    15,
      15,     8,     7,    15,    15,    71,    71,    71,    38,    66,
      71,    71,    71,    38,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    36,    40,    39,    71,     8,
      71,    71,     8,     8,     8,    16,    39,    16,    38,    16,
       7,    16,    16,    16,    66,    66,    39,    43,    69,    70,
      71,    32,    71,     8,    20,    66,    39,    71
  };

  const signed char
  parser::yyr1_[] =
  {
       0,    44,    45,    46,    48,    47,    49,    49,    51,    50,
      52,    52,    53,    53,    54,    54,    54,    54,    55,    55,
      55,    56,    57,    57,    58,    58,    60,    59,    61,    61,
      62,    63,    63,    64,    64,    64,    65,    65,    66,    66,
      66,    67,    67,    67,    67,    68,    68,    68,    68,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71
  };

  const signed char
  parser::yyr2_[] =
  {
       0,     2,     2,     2,     0,    20,     2,     0,     0,     7,
       2,     0,     3,     5,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     3,     2,     0,     0,    10,     2,     1,
       3,     2,     0,     2,     4,     0,     2,     0,     1,     1,
       1,     4,     2,     5,     7,     5,     7,     5,     3,     1,
       0,     1,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     3,     5,     3,     6,     4,     4,     3,
       1,     1,     1,     1,     1,     4,     4,     5
  };


#if YYDEBUG || 1
  // YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
  // First, the terminals, then, starting at \a YYNTOKENS, nonterminals.
  const char*
  const parser::yytname_[] =
  {
  "\"end of file\"", "error", "\"invalid token\"", "PLUSOP", "SUBOP",
  "MULTOP", "DIVOP", "LP", "RP", "LESSOP", "GREATOP", "AND", "OR", "MOD",
  "EQUAL", "LS", "RS", "TRUE", "FALSE", "DOT", "COMMA", "THIS", "NEW",
  "NOT", "PUBLIC", "CLASS", "STATIC", "VOID", "MAIN", "STRING", "BOOLEAN",
  "IF", "ELSE", "WHILE", "SYSTEMOUTPRINTLN", "RETURN", "LENGTH", "LBRACE",
  "RBRACE", "SEMICOLON", "ID", "INT", "INTLITERAL", "ASSIGN", "$accept",
  "root", "goal", "main_class", "$@1", "class_declaration_list",
  "class_declaration", "$@2", "var_declaration_list", "var_declaration",
  "type", "base_type", "type_identifier", "array_type",
  "method_declaration_list", "method_declaration", "$@3", "method_body",
  "return_statement", "non_return_statement_list", "param_list",
  "statement_list", "statement", "simple_statement", "compound_statement",
  "expression_list", "non_empty_expression_list", "expression", YY_NULLPTR
  };
#endif


#if YYDEBUG
  const short
  parser::yyrline_[] =
  {
       0,    61,    61,    64,    71,    70,    81,    84,    87,    86,
      98,   100,   103,   108,   114,   115,   116,   117,   121,   122,
     123,   128,   135,   136,   142,   143,   146,   145,   159,   165,
     171,   177,   183,   188,   195,   202,   205,   206,   209,   210,
     211,   215,   221,   226,   230,   239,   244,   250,   255,   262,
     263,   266,   267,   270,   280,   286,   292,   297,   303,   309,
     315,   321,   327,   332,   338,   341,   344,   349,   355,   361,
     362,   366,   367,   368,   369,   370,   373,   376
  };

  void
  parser::yy_stack_print_ () const
  {
    *yycdebug_ << "Stack now";
    for (stack_type::const_iterator
           i = yystack_.begin (),
           i_end = yystack_.end ();
         i != i_end; ++i)
      *yycdebug_ << ' ' << int (i->state);
    *yycdebug_ << '\n';
  }

  void
  parser::yy_reduce_print_ (int yyrule) const
  {
    int yylno = yyrline_[yyrule];
    int yynrhs = yyr2_[yyrule];
    // Print the symbols being reduced, and their result.
    *yycdebug_ << "Reducing stack by rule " << yyrule - 1
               << " (line " << yylno << "):\n";
    // The symbols being reduced.
    for (int yyi = 0; yyi < yynrhs; yyi++)
      YY_SYMBOL_PRINT ("   $" << yyi + 1 << " =",
                       yystack_[(yynrhs) - (yyi + 1)]);
  }
#endif // YYDEBUG


} // yy
#line 2152 "parser.tab.cc"

