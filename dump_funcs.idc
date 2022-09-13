// Taken from https://stackoverflow.com/questions/8860020/is-there-a-way-to-export-function-names-from-ida-pro

#include <idc.idc>

static FuncDump(start, filename)
{
    auto ea, str, count, ref;
    auto end;
    auto teststr;
    auto f = fopen(filename, "w");

    ea = start;

    while( ea != BADADDR )
    {
        str = GetFunctionName(ea);
        if( str != 0 )
        {
            end = FindFuncEnd(ea);

            count = 0;
            ref = RfirstB(ea);
            while(ref != BADADDR)
            {
                count = count + 1;
                ref = RnextB(ea, ref);
            }

            teststr = sprintf("sub_%X", ea);
            if( teststr != str )
            {
                fprintf(f, "0x%X=%s\n", ea, str);
            }
            //Message("%s, 0x%d, 0x%x, 0x%x, 0x%x, %d\n", str, count, ea, end, end-ea, end-ea   );
        }

        ea = NextFunction(ea);
    }
    fclose(f);
}
// own work
static ReadDump(filename)
{
    Message("ReadDump");
    auto f = fopen(filename, "r");
    auto name = "", ea = 0, eq_pos = 0;
    auto c, pos = 0, in_ea = 0, in_name = 0;
    auto oldea;
    auto data = readstr(f);
    Message(data);
    do {
      oldea = ea;
    while(pos < strlen(data))
    {
        c = substr(data, pos, pos + 1);

        if (c == "=")
        {
            ea = xtol(data[0:pos]);
            eq_pos = pos;
            break;
        }
        pos = pos + 1;
    }
    {
            name = substr(data, eq_pos + 1, -1);
            name = substr(name, 0, strlen(name) - 1);
            MakeName(ea, name);
        }
        data = readstr(f);
    } while(data && ea != oldea);
}
