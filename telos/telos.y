%name-prefix="telos" 
%{
// extern char * telostext;
#include "header.h"
int count_element, count_link;
map <string, OBJECT*> objects;
%}
%union {
	char ident[260];
	OBJECT_PTR object;
	TUPLES_PTR tuples;
	PAIR_PTR pair;
}

%token <ident> LABEL ALPHANUM 
%token <ident> REAL INTEGER 

%token IN ISA WITH END
%token TOKEN SIMPLECLASS METACLASS METAMETACLASS

%type <ident> label objectname NUMBER propertyname propertyvalue attribute
%type <object> object 
%type <tuples> withspec decllist
%type <pair> declaration

%start telos

%%
telos
    : objects 
    {
	printf("C %d %d\n", count_element, count_link);
	int element_number = 0;
	for (map<string, OBJECT*>::iterator I = objects.begin(); 
		I!=objects.end(); I++) {
		OBJECT *o = I->second;
		if (I->first.substr(0,7)=="Element") {
			string s = (*(o->attributes))[string("name")];
			string sat = "NO";
			string den = "NO";
			ostringstream myO;
			myO << element_number;
			(*(o->attributes))[string("element_key")] = myO.str();
			string label = (*(o->attributes))[string("label")];
			if (label == "NFRSatisficedSoftgoalLabel") {
				sat = "TOTAL";
			} else if (label == "NFRWeaklySatisficedSoftgoalLabel") {
				sat = "PARTIAL";
			} 
			if (label == "NFRDeniedSoftgoalLabel") {
				den = "TOTAL";
			} else if (label == "NFRWeaklyDeniedSoftgoalLabel") {
				den = "PARTIAL";
			} 
			cout << "N " << sat << " " << den << " " << s.substr(1, s.length()-2) << endl; // get rid of ""
			element_number++;
		} else if (I->first.substr(0,4)=="Link"){
			string n = (*(o->attributes))[string("name")];
			n = n.substr(1, n.length()-2); // get rid of ""
			string f = (*(o->attributes))[string("from")];
			string t = (*(o->attributes))[string("to")];
			OBJECT *o_f = objects[f];
			OBJECT *o_t = objects[t];
			string k_f = (*(o_f->attributes))["element_key"];
			string k_t = (*(o_t->attributes))["element_key"];
			string l("");  // label
			if (n == "Help") 
				l = "+";
			else if (n=="Make")
				l = "++";
			else if (n == "Hurt") 
				l = "-";
			else if (n=="Break")
				l = "--";
			else if (n=="And")
				l = "AND";
			else if (n=="Or")
				l = "OR";
			cout << "R " << l << " " << k_t << " " << k_f << endl;
		}
	}
    }
    ;

objects
    : object {
	objects[string($1->oid)] = $1;
     }  | object  objects {
	objects[string($1->oid)] = $1;
     };

object
    : objecttype objectname inspec isaspec withspec endspec 
    {
	$$ = new OBJECT;
	strcpy($$->oid, $2);
        $$->attributes = $5;	
	if (strncmp($2,"Element", 7)==0) {
		count_element ++;
	}
	if (strncmp($2,"Link", 4)==0) {
		count_link ++;
	}
    };

objecttype
    : {}|  TOKEN {}|  SIMPLECLASS {}|  METACLASS {}|  METAMETACLASS {};

objectname
    : '(' objectname ')' 
    {
    } |  label bindings
    {
	strcpy($$,  $1);
    } |  objectname SELECTOR1 label
    {} |  objectname SELECTOR2 objectname
    {};

bindings
    : 
    {} |  '[' bindinglist ']'
    {};

bindinglist
    : singlebinding
    {}  | singlebinding ',' bindinglist 
    {};

singlebinding
    : objectname '/' label
    {}  | label ':' objectname
    {};

inspec
    : 
    {}  |  IN classlist
    {};

isaspec
    : 
    {} | ISA classlist
    {};

withspec
    : 
    {} | WITH decllist
    {
	$$ = $2;
    };

classlist
    : objectname
    {} |objectname ',' classlist
    {};

decllist
    : declaration 	   
    {
	$$ = new TUPLES;
	(*$$)[string($1->name)] = string($1->value);
    } |declaration decllist
    {
	$$ = $2;
	(*$$)[string($1->name)] = string($1->value);
    };

declaration
    : label attribute propertyname ':' propertyvalue
    {
	$$ = (PAIR*) malloc(sizeof(PAIR));
	if (strlen($3)<1) {
		strcpy($$->name , $2);
	} else {
		strcpy($$->name ,$3);
	}
	strcpy($$->value, $5);	
    };

attribute
    :    
    {
	strcpy($$, "");
    } |',' label attribute 
    {
	strcpy($$, $2);	
    };

propertyname
    :   
    {} |label 
    {};

propertyvalue
    : label	
    {} |objecttype	
    {} |enumeration
    {} |pathexpression
    {};

enumeration
    : '[' classlist ']' 
    {};

pathexpression
    : objectname SELECTORB pathargument
    {};

pathargument
    : label
    {} |label SELECTORB pathargument
    {} |restriction
    {} |restriction SELECTORB pathargument
    {};

restriction
    : '(' label ':' enumeration ')'
    {} |'(' label ':' pathexpression ')'
    {} |'(' label ':' objectname ')'
    {};

endspec
    : END
    {};

label
    : ALPHANUM
    { strcpy($$, telostext); } |LABEL
    { strcpy($$, telostext); } |NUMBER
    { strcpy($$, telostext); };

NUMBER
    : REAL
    { strcpy($$, telostext); } |INTEGER
    { strcpy($$, telostext); };

SELECTOR1
    : '!'
    {} |'^'
    {};

SELECTOR2
    : '-' '>'
    {} |'=' '>'
    {};

SELECTORB
    : '.'
    {} |'|'
    {};

%%

int yyerror(char *s)
{
    printf("%s\n", s);
    return 0;
}
