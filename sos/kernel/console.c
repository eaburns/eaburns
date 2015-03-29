#include <console.h>
#include <kstdlib.h>
#include <device.h>
#include <kstring.h>
#include <retval.h>

#include <config/bcon.h>
#ifdef CONFIG_BCON
#include <bcon.h>
#endif

/* Console table. */
console contab[] = {
#ifdef CONFIG_BCON
  { "bconsole", /*bcon_read*/NULL, bcon_write, bcon_init, 1 },
#endif
  { NULL, NULL, NULL, 0 }
};

/* Current console. */
console *cur_con;

console *get_console( char *name )
{
  console *p = contab;

  for( ; p->name != NULL; p++ ) {
    if( kstrcmp( name, p->name ) == 0 ) { return p; }
  } 

  return NULL;
}

int init_console(void)
{
  console *p;
  cur_con = p = contab;

  while( p->name != NULL ) {
    if( p->priority > cur_con->priority )
      cur_con = p;

    p++;
  }

  if( cur_con->name == NULL )
    return RET_FAILURE;

  cur_con->init();

  puts( "(S)imple (O)perating (S)ystem.\n" );
  puts( "Ethan Burns <eaburns@cisunix.unh.edu>\n" );
  puts( "===================================================================" );
  puts( "=============\n" );
  return RET_SUCCESS;
}
