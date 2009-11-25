/**
 * @brief Definicje przydatne przy generowaniu kodu do asemblera
 * @author Piotr Truszkowski
 */

#ifndef _J_ASM_H_
#define _J_ASM_H_

#define J_VAL_LABEL "V"
#define J_FUN_LABEL "F"
#define J_ITER_LABEL "I"
#define J_COND_LABEL "C"
#define J_EXPR_LABEL "E"

#define J_SIZE_OF_VAR (4)
#define J_STACK_ARGS_START (8)
#define J_STACK_LOCAL_START (-4)

#define J_ASM_SECTION_RODATA \
"section .rodata\n"\
"\terror_str\tdb \"ERROR\" , 0xa , 0x0\n"\
"\terror_len\tequ $-error_str\n"\
"\tread_int_failure_str\tdb \"readInt(): not a number, abort\" , 0xa , 0x0\n"\
"\tread_int_failure_len\tequ $-read_int_failure_str\n"\
"\tread_double_failure_str\tdb \"readDouble(): not a number, abort\" , 0xa , 0x0\n"\
"\tread_double_failure_len\tequ $-read_double_failure_str\n"\
"\tfmt_s\tdb \"%%s\" , 0x0\n"\
"\tfmt_i\tdb \"%%d\" , 0x0\n"\
"\tfmt_d\tdb \"%%g\" , 0x0\n"

#define J_ASM_SECTION_TEXT \
"section .text\n"\
"\textern printf\n"\
"\textern scanf\n"\
"\tglobal main\n"\
"\n"

#define J_ASM_MACRO \
"%%macro f_enter 0\n"\
"\tpush ebp\n"\
"\tmov ebp , esp\n"\
"%%endmacro\n\n"\
"%%macro f_leave 0\n"\
"\tmov esp , ebp\n"\
"\tpop ebp\n"\
"%%endmacro\n\n"

#define J_ASM_PRINT_INT_BUILTIN \
"\tf_enter\n"\
"\tpush dword [ebp+8]\n"\
"\tpush dword fmt_i\n"\
"\tcall printf\n"\
"\tadd esp , 8\n"\
"\tf_leave\n"\
"\tret\n"

#define J_ASM_PRINT_DOUBLE_BUILTIN \
"\tf_enter\n"\
"\tsub esp , 8\n"\
"\tfld dword [ebp+8]\n"\
"\tfstp qword [ebp-8]\n"\
"\tpush dword [ebp-4]\n"\
"\tpush dword [ebp-8]\n"\
"\tpush dword fmt_d\n"\
"\tcall printf\n"\
"\tadd esp , 12\n"\
"\tadd esp , 8\n"\
"\tf_leave\n"\
"\tret\n"

#define J_ASM_PRINT_STRING_BUILTIN \
"\tf_enter\n"\
"\tpush dword [ebp+8]\n"\
"\tpush dword fmt_s\n"\
"\tcall printf\n"\
"\tadd esp , 8\n"\
"\tf_leave\n"\
"\tret\n"

#define J_ASM_READ_INT_BUILTIN \
"\tf_enter\n"\
"\tsub esp , 4\n"\
"\tmov eax , ebp\n"\
"\tsub eax , 4\n"\
"\tpush dword eax\n"\
"\tpush dword fmt_i\n"\
"\tcall scanf\n"\
"\tadd esp , 8\n"\
"\ttest eax , 1\n"\
"\tjnz short read_int_success\n"\
"\tmov eax , 4\n"\
"\tmov ebx , 2\n"\
"\tmov ecx , read_int_failure_str\n"\
"\tmov edx , read_int_failure_len\n"\
"\tint 0x80\n"\
"\tmov eax , 1\n"\
"\tmov ebx , 1\n"\
"\tint 0x80\n"\
"read_int_success:\n"\
"\tmov eax , [ebp-4]\n"\
"\tadd esp , 4\n"\
"\tf_leave\n"\
"\tret\n"

#define J_ASM_READ_DOUBLE_BUILTIN \
"\tf_enter\n"\
"\tsub esp , 4\n"\
"\tmov eax , ebp\n"\
"\tsub eax , 4\n"\
"\tpush dword eax\n"\
"\tpush dword fmt_d\n"\
"\tcall scanf\n"\
"\tadd esp , 8\n"\
"\ttest eax , 1\n"\
"\tjnz short read_double_success\n"\
"\tmov eax , 4\n"\
"\tmov ebx , 2\n"\
"\tmov ecx , read_double_failure_str\n"\
"\tmov edx , read_double_failure_len\n"\
"\tint 0x80\n"\
"\tmov eax , 1\n"\
"\tmov ebx , 1\n"\
"\tint 0x80\n"\
"read_double_success:\n"\
"\tmov eax , [ebp-4]\n"\
"\tf_leave\n"\
"\tret\n"

#define J_ASM_ERROR_BUILTIN \
"\tf_enter\n"\
"\tmov eax , 4\n"\
"\tmov ebx , 2\n"\
"\tmov ecx , error_str\n"\
"\tmov edx , error_len\n"\
"\tint 0x80\n"\
"\tmov eax , 1\n"\
"\tmov ebx , 1\n"\
"\tint 0x80\n"\
"\tf_leave\n"\
"\tret\n"

#endif
