OUTPUT_FORMAT("elf32-i386")
OUTPUT_ARCH(i386)
ENTRY(_start)

SECTIONS {

  . = 0x500;
  __text_start = .;
	.text : {
		*(.text)
	}
  __text_end = .;


  __data_start = .;
	.data : {
		*(.data)
	}

	.bss : {
		*(.bss)
	}

	.rodata : {
		*(.rodata)
	}

	.initdata : {
		__init_start = .;
		*(.initdata)
		__init_end = .;
	}
  __data_end = .;

  . = ALIGN(4096);
  __system_tss = .;
  . = . + 104;
  . = ALIGN(4096);
  __page_dir = .;

  __kalloc_base = . + 4096;
}
