/* These are declerations for functions in cpu.c that are arch
 * specific so they do not appear in cpu.h. */

/* Loads the interrupt table descriptor. */
void __inline lidt( table_descriptor *idtd );

/* Loads the global descriptor table. */
void __inline lgdt( table_descriptor *idtd );
