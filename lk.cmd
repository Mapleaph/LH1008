
-stack 0x2000
-heap  0x1000

MEMORY
{
   IBOOT    : o=00000000h,l=400h
   IRAM     : o=00000400h,l=20000h
}
SECTIONS
{
     .boot_load :> IBOOT
     .text   :> IRAM
     .switch :> IRAM
     .cinit  :> IRAM
     .const  :> IRAM
     .tables :> IRAM
     .stack  :> IRAM
     .data   :> IRAM
     .bss    :> IRAM
     .far    :> IRAM
     .cio    :> IRAM 
     .sysmem :> IRAM 

}