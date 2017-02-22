/*									 */

#define  EOS		     '\0'
#define  SYMTABSIZE	     19
#define  MAXIDLEN	     16

#define  SYMBOL              0
#define  TYPEDEF_T           1 


/* definition of one symbol table entry 				  */

struct SymbolTableNode {
			 char symbol[MAXIDLEN+1];
             int  symtype;      /* symbol type is either  */
                        /* symbol or typedef.     */
			 int  reserved;
			 int  count;
			 int  address;
			 int  type;
			 char *strngvalue;
			 struct SymbolTableNode *next;
};

