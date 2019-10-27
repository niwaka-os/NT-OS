ENTRY(main)
INCLUDE ld_variables.lds

SECTIONS
{
	. = SIZEOF_HEADERS;
	.text : {*(.text)}
	

}
