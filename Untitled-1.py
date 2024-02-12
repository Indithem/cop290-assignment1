{
  open Printf



  type token =
    | KEYWORD  of string
    | IDENTIFIER of string
    | BOOL_OP of string
    | BOOL_CONST of string
    | ARITH_OP of string
    | INT_CONST of int
    | COMPARISON_OP of string
    | STRING_OP of string
    | STRING_CONST of string
    | LEXICAL_ERROR of string
    | PAREN of char
    | COMMA of char
    | CHAR of char
    | PRIME of string

  let keyword_table = 
    create_hashtable 17 [
      ("if",KEYWORD "if");
      ("then",KEYWORD "then");
      ("else", KEYWORD "else");
      ("for", KEYWORD "for");
      ("while", KEYWORD "while");
      ("do", KEYWORD "do");
      ("done", KEYWORD "done");
      ("begin", KEYWORD "begin");
      ("end", KEYWORD "end");
      ("function", KEYWORD "function");
      ("in",KEYWORD "in");
      ("match",KEYWORD "match");
      ("of",KEYWORD "of");
      ("let", KEYWORD "let");
      ("with", KEYWORD "with");
      ("type",KEYWORD "type");
      ("rec",KEYWORD "rec");
      ("open",KEYWORD "open");
      ("raise",KEYWORD "raise");
      ("exception",KEYWORD "exception")
    ]

}

let digit = ['0'-'9']
let digit1=['1'-'9']
let id = ['a'-'z''_' ]['a'-'z' '0'-'9''_' ''']*
let prime=[''']

rule tokeniser = parse

  | "and" | "or" | "not" as bool_op
    { BOOL_OP bool_op }

  | digit+ as num
    {INT_CONST (int_of_string num)}
  | digit+  id*  as num
    { LEXICAL_ERROR num}
  (* | digit+ as num  "and" as temp
    { 
      INT_CONST (int_of_string num), 
      BOOL_OP temp
    } *)
  | "true" | "false" as bool_const
    { BOOL_CONST bool_const }

  | prime+ as iter
     { 
        PRIME iter
    }

  | id as word
    { 
      try
        let token = Hashtbl.find keyword_table word in
        token
      with Not_found ->
        IDENTIFIER word
    }
  

  | "+"  { ARITH_OP "+" }
  | "-"  { ARITH_OP "-" }
  | "*"  { ARITH_OP "*" }
  | "/"  { ARITH_OP "/" } 
  | "="  { COMPARISON_OP "=" }
  | "!=" { COMPARISON_OP "!=" }
  | "<"  { COMPARISON_OP "<" }
  | "<=" { COMPARISON_OP "<=" }
  | ">"  { COMPARISON_OP ">" }
  | ">=" { COMPARISON_OP ">=" }

  | "concat"     { STRING_OP "concat" }


  | "("  { PAREN '(' }
  | ")"  { PAREN ')' }
  | "]"  { PAREN ']' }
  | "["  { PAREN '[' }
  | "{"  { PAREN '{' }
  | "}"  { PAREN '}' }
  | ","  { COMMA  ','}
  | '{' [^ '\n']* '}'	(* ignore one-line comments *)
  | [' ' '\t' '\n' ]	(* ignore  whitespace *)
  	{ tokeniser lexbuf }
  
 
  | _ as c
	  {CHAR c}
 
  | eof { raise End_of_file }

{ 
  let rec parse lexbuf =
    let token = tokeniser lexbuf in
    Printf.printf "%s\n" (match token with
                          | IDENTIFIER s -> "IDENTIFIER: " ^ s
                          | KEYWORD s -> "KEYWORD: " ^ s
                          | BOOL_OP s -> "BOOL_OP: " ^ s
                          | BOOL_CONST s -> "BOOL_CONST: " ^ s
                          | ARITH_OP s -> "ARITH_OP: " ^ s
                          | INT_CONST n -> "INT_CONST: " ^ string_of_int n
                          | COMPARISON_OP s -> "COMPARISON_OP: " ^ s
                          | STRING_OP s -> "STRING_OP: " ^ s
                          | STRING_CONST s -> "STRING_CONST: " ^ s
                          | PAREN c -> "PAREN: " ^ Char.escaped c
                          | COMMA c-> "COMMA: " ^ Char.escaped c
                          | CHAR c -> "ERROR: "  ^ Char.escaped c
                          | PRIME s-> "ERROR: " ^ s
                          | LEXICAL_ERROR s->"ERROR: "^ s
                           
                         );
    parse lexbuf

  let main () =
    let cin =
      if Array.length Sys.argv > 1
      then open_in Sys.argv.(1)
      else stdin
    in
    let lexbuf = Lexing.from_channel cin in
    try parse lexbuf
    with End_of_file -> ()

  let _ = Printexc.print main ()
}
