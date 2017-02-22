#include <stdlib.h>

/* Symbol Table Handling Routines for Compiler Class                     */

/* Define the Symbol Table */

struct SymbolTableNode *symtab[SYMTABSIZE];

struct SymbolTableNode *freenode, *symnode;


/* ********************************************************************** */
/*			 Hash						  */
/* ********************************************************************** */


int Hash(strng)
 char *strng;

 /* Hash the input string into an integer from 0 to MAXSYM - 1 */
 /* See AHO, p 436 */

{ char *p;
  unsigned h = 0, g;

  for (p = strng; *p != EOS; p = p + 1)
    {
      h = (h << 4) + (*p);
      g = h&0xf0000000;
      if (g)
	{
	  h = h ^ (g >> 24);
	  h = h ^ g;
	};
    };

return(h % SYMTABSIZE);

}

/* ********************************************************************** */
/*			 GetNode					  */
/* ********************************************************************** */


struct SymbolTableNode *GetNode()

/* This function gets a new node for the symbol table.			  */

{
      freenode = (struct SymbolTableNode *) malloc(sizeof (struct SymbolTableNode));

 return (freenode);

}


/* ********************************************************************** */
/*			 Lookup 					  */
/* ********************************************************************** */


long Lookup(strng) /* Lookup an entry in the symbol table		  */
char strng[];

{
  int h;
  struct SymbolTableNode *p;
  h = Hash(strng);
  p = symtab[h];       /* pointer to the correct bucket 		  */

  if (p == NULL)
     return (0);

  while(p)	       /* search linked list for correct node	       */
     {
       if (!strcmp(strng,p->symbol))
	 {
	  return ((long) p);  /* return the val variable 	       */
	 }
       else
	 p = p->next;
     };


return(0);
}

/* ********************************************************************** */
/*			 Insert 					  */
/* ********************************************************************** */


long Insert(strng) /* Insert an entry into the symbol table		  */
 char strng[];

{

  int h;
  struct SymbolTableNode *p, *q, *GetNode();

  h = Lookup(strng);  /* had better not be found !		       */
  if (h != 0)
    return (0);
  else
    h = Hash(strng);

  /* if we get here, the string was not in the table		       */

  if (symtab[h] == NULL)      /* here if first bucket for hash value   */
    {
      p = NULL;
      p = GetNode();
      p->count = 0;
      p->symtype = SYMBOL;  /* default to a symbol (not a typedef var) */
      strcpy(p->symbol,strng);
      p->next	= NULL;
      symtab[h] = p;
      return ((long) p);
    }
  else
   {
    q = GetNode();	 /* get a new node */
    strcpy(q->symbol,strng);
    q->count = 0;
    q->symtype = SYMBOL;   /* default to a symbol (not a typedef var) */
    q->next   = symtab[h];
    symtab[h] = q;
    return ((long) q);
   };

}


/* ********************************************************************** */
/*			 PrntSymtbl					  */
/* ********************************************************************** */
void PrntSymTbl()
{

  int i;
  struct SymbolTableNode *p, *q, *GetNode();

  for (i = 0; i < SYMTABSIZE ; i++ )
     {
       p = symtab[i];
       printf("symtab[%d] = %u \n",i, p);
	if (p == NULL)
	   ;
	else
	  {
	    while(p)
	       {
		 printf("    p->symbol %s p->count %d p->symtype %d p->next %u \n",
			    p->symbol, p->count, p->symtype , p->next);
		 p = p->next;
	       };
	  };
     }
   return;
}

