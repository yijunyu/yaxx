Format bibformat = {
	"\n",		/* Empty lines are allright */
	".{\n",		/* Record separator */
	".}\n",		/* Record separator */
} ;

char *bibtemplate[] = {
  "%%A %author_andsplit\n",
  "%%B %booktitle\n",
  "%%C %address\n",
  "%%D %month %year\n",
  "%%E %editor\n",
  "%%I %publisher\n",
  "%%J %journal\n",
  "%%K %keywords\n",
  "%%L %key\n",
  "%%N %number\n",
  "%%P %pages\n",
  "%%T %title\n",
  "%%V %volume\n",
  "%%X %abstract\n",
  NULL
} ;
