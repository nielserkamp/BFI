ENTRY(start);

PHDRS
{
  text PT_LOAD FILEHDR PHDRS FLAGS(7);
  note PT_GNU_STACK FLAGS(6);
}

SECTIONS
{
  . = SIZEOF_HEADERS;
  .text : { *(*) } :text
}

