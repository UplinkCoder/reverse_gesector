#define UNLOADED_FILE 1
#include <idc.idc>
extern f;

static upn(ea, n) { 
    auto i = 0;
    for(i = 0; i < n; i++) {
        ea = find_code(ea, 0);
    }
    return ea;
}

static readString(ea)
{
    auto i = 0;
    auto byteval = 0;

    do {
        if (i > 100)
        {
            fprintf(f, " can't read ");
            return;
        }
        byteval = byte(ea + i);
        i++;
    } while(byteval);
    
    i = 0;
    do {
        fprintf(f, "%c", byteval);
        byteval = byte(ea + i);
        i++;
    } while(byteval);


    create_strlit(ea, i);
}

static xRefHandler(ea_call)
{
    auto msg_string = "";
    auto fn_begin = 0;
    auto i = 0;
    auto string_addr = get_operand_value(upn(ea_call, 3), 0);
    auto line_ = get_operand_value(upn(ea_call, 4), 0);
    

    fprintf(f, "0x%x ", ea_call);
    readString(string_addr);
    fprintf(f, ":%d\n", line_);
    // print_operand(text_ea, 0));
}

static main(void) {

    auto geErrorLog_AddExplicit = 0x43ACD0;

    auto current_xref = get_first_cref_to(geErrorLog_AddExplicit);
    f = fopen("log_err_calls.txt", "w");

    while(current_xref != -1)
    {
        xRefHandler(current_xref);
        current_xref = get_next_cref_to(geErrorLog_AddExplicit, current_xref);
    }
    fclose(f);

}
