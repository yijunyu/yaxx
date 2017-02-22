Format rhfformat = {
	"",				/* No empty lines */
	"",				/* Record start */
	"\n",				/* Record separator */
} ;

char *rhftemplate[] = {
  "%%TI %title\n",
  "%%LT %number_techreport\n",
  "%%AU %author_andsplit\n",
  "%%RT %type_notechreport\n",
  "%%OR %organization\n",
  "%%AB %abstract\n",
  "%%KW %keywords_commasplit\n",
  "%%MN %month\n",
  "%%YR %year\n",
  "%%PU %booktitle, (%editor), %publisher, %journal, %volume, %number_notechreport, %pages\n",
  "%%PL %address\n",
  "%%AV url %urlprefix%url\n",
  "%%AV ftp %ftpprefix%ftp\n",
  "%%OC %orgcode\n",
  NULL
} ;
