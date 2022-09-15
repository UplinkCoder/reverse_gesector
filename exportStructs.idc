#include <idc.idc>

static ExportKnownStructs(fName) {
  auto idx;
  auto f = fopen(fName, "w");
  
  for ( idx=GetFirstStrucIdx(); idx != -1; idx=GetNextStrucIdx(idx) ) {
    auto id,m, prevOffset = 0, hole = 0, padN = 0;
    id = GetStrucId(idx);
    if ( id == -1 ) Fatal("Internal IDA error, GetStrucId returned -1!");
    fprintf(f, "struct %s {\n",GetStrucName(id));
    //fprintf(f, "  Regular    comment: %s\n",GetStrucComment(id,0));
    //fprintf(f, "  Repeatable comment: %s\n",GetStrucComment(id,1));
    fprintf(f, "//  Size              : %d\n",GetStrucSize(id));
    fprintf(f, "//  Number of members : %d\n",GetMemberQty(id));
    for ( m = 0;
	  m != GetStrucSize(id);
	  m = GetStrucNextOff(id,m) ) {
      auto mname;
      mname = GetMemberName(id,m);
      if ( mname == "" ) {
        hole = 1;
      } else {
        auto type;
        auto memberName = GetMemberName(id,m);
        auto memberId = GetMemberId(id, m);
        auto memberTypeId = GetType(memberId);
        auto memberOffset = GetMemberOffset(id, memberName);
        if (hole)
        {
            fprintf("  BYTE pad_%d[%d];\n", padN++, memberOffset - prevOffset);
        }
        prevOffset = memberOffset;
        //fprintf(f, "    Regular cmt : %s\n",GetMemberComment(id,m,0));
        //fprintf(f, "    Rept.   cmt : %s\n",GetMemberComment(id,m,1));
        //fprintf(f, "    Member size : %d\n", GetMemberSize(id,m));
        //fprintf(f, "  Offset: %d\n", memberOffset);
        //fprintf(f, "  MemberId: %d\n", memberId);
        if (memberTypeId != "")
            fprintf(f, "  %s %s;\n", memberTypeId, memberName);
        else
            fprintf(f, "  void %s[%d]\n", memberName, GetMemberSize(id, m));
      }
    }
    fprintf(f, "\n}\n");
  }
  // Message("Total number of structures: %d\n",GetStrucQty());
}