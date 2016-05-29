/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 1999, 2015 by Great Hill Corporation.
 * All Rights Reserved
 *
 *------------------------------------------------------------------------*/

#include "manage.h"
#include "slurp.h"

//---------------------------------------------------------------------------
IMPLEMENT_NODE(CSlurp, CBaseNode, NO_SCHEMA);

//---------------------------------------------------------------------------
void CSlurp::Format_base(CExportContext& ctx, const SFString& fmtIn) const
{
	if (!isShowing())
		return;
	
	// Split the format string into three parts: pre, post and records.
	// If no records, just process as normal. We do this because it's so slow
	// copying the records into a string, so we write it directly to the
	// export context. If there is no {RECORDS}, then just send handle it like normal
	if (!fmtIn.Contains("{RECORDS}") || transactions.getCount()==0)
	{
		SFString fmt = fmtIn;

		CSlurpNotify dn(this);
		while (!fmt.IsEmpty())
			ctx << getNextChunk(fmt, nextSlurpChunk, &dn);
	} else
	{
		SFString postFmt = fmtIn;
		postFmt.Replace("{RECORDS}","|");
		SFString preFmt = nextTokenClear(postFmt,'|');

		// We assume here that the token was properly formed. For the pre-text we
		// have to clear out the start '[', and for the post text we clear out the ']'
		preFmt.ReplaceReverse("[","");
		postFmt.Replace("]","");

		// We handle the display in three parts: pre, records, and post so as
		// to avoid building the entire record list into an ever-growing and
		// ever-slowing string
		CSlurpNotify dn(this);
		while (!preFmt.IsEmpty())
			ctx << getNextChunk(preFmt, nextSlurpChunk, &dn);
		for (int i=0;i<transactions.getCount();i++)
		{
			if (!(i%5)) { outErr << "Exporting record " << i << " of " << transactions.getCount() << (isTesting?"\n":"\r"); outErr.Flush(); }
			ctx << transactions[i].Format(displayString);
		}
		ctx << "\n";
		while (!postFmt.IsEmpty())
			ctx << getNextChunk(postFmt, nextSlurpChunk, &dn);
	}
}

//---------------------------------------------------------------------------
SFString nextSlurpChunk(const SFString& fieldIn, SFBool& force, const void *data)
{
	CSlurpNotify *sl = (CSlurpNotify*)data;
	const CSlurp *slu = sl->getDataPtr();

	// Give common (edit, delete, etc.) code a chance to override
	SFString ret = nextChunk_common(fieldIn, getString("cmd"), slu);
	if (!ret.IsEmpty())
		return ret;
	
	// Now give customized code a chance to override
	ret = nextSlurpChunk_custom(fieldIn, force, data);
	if (!ret.IsEmpty())
		return ret;
	
	switch (tolower(fieldIn[0]))
	{
		case 'a':
			if ( fieldIn % "addr" ) return slu->addr;
			break;
		case 'd':
			if ( fieldIn % "displayString" ) return slu->displayString;
			break;
		case 'h':
			if ( fieldIn % "handle" ) return asString(slu->handle);
			if ( fieldIn % "header" ) return slu->header;
			break;
		case 'l':
			if ( fieldIn % "lastPage" ) return asString(slu->lastPage);
			if ( fieldIn % "lastBlock" ) return asString(slu->lastBlock);
			break;
		case 'p':
			if ( fieldIn % "pageSize" ) return asString(slu->pageSize);
			break;
		case 't':
#if 0
			if ( fieldIn % "transactions" )
			{
				SFString ret = "\n";
				for (int i=0;i<slu->transactions.getCount();i++)
					ret += slu->transactions[i].Format("\t\t"+slu->transactions[i].asHTML());
				return ret;
			}
#endif
			break;
	}
	
	return "<span class=warning>Field not found: [{" + fieldIn + "}]</span>\n";
}

//---------------------------------------------------------------------------------------------------
SFBool CSlurp::setValueByName(const SFString& fieldName, const SFString& fieldValue)
{
	return TRUE;
}

//---------------------------------------------------------------------------------------------------
void CSlurp::Serialize(SFArchive& archive)
{
	SERIALIZE_START();
	if (archive.isReading())
	{
		archive >> handle;
		archive >> addr;
		archive >> header;
		archive >> displayString;
		archive >> pageSize;
		archive >> lastPage;
		archive >> lastBlock;
		archive >> transactions;

	} else
	{
		archive << handle;
		archive << addr;
		archive << header;
		archive << displayString;
		archive << pageSize;
		archive << lastPage;
		archive << lastBlock;
		archive << transactions;

	}
	SERIALIZE_END();
}

//---------------------------------------------------------------------------
void CSlurp::registerClass(void)
{
	SFInt32 fieldNum=1000;
	ADD_FIELD(CSlurp, "schema",  T_NUMBER|TS_LABEL, ++fieldNum);
	ADD_FIELD(CSlurp, "deleted", T_RADIO|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CSlurp, "handle", T_NUMBER|TS_LABEL,  ++fieldNum);
	ADD_FIELD(CSlurp, "addr", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "header", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "displayString", T_TEXT, ++fieldNum);
	ADD_FIELD(CSlurp, "pageSize", T_RADIO, ++fieldNum);
	ADD_FIELD(CSlurp, "lastPage", T_NUMBER, ++fieldNum);
	ADD_FIELD(CSlurp, "lastBlock", T_NUMBER, ++fieldNum);
	ADD_FIELD(CSlurp, "transactions", T_TEXT|TS_ARRAY, ++fieldNum);
}

//---------------------------------------------------------------------------
int sortSlurp(const SFString& f1, const SFString& f2, const void *rr1, const void *rr2)
{
	CSlurp *g1 = (CSlurp*)rr1;
	CSlurp *g2 = (CSlurp*)rr2;

	SFString v1 = g1->getValueByName(f1);
	SFString v2 = g2->getValueByName(f1);
	SFInt32 s = v1.Compare(v2);
	if (s || f2.IsEmpty())
		return (int)s;

	v1 = g1->getValueByName(f2);
	v2 = g2->getValueByName(f2);
	return (int)v1.Compare(v2);
}
int sortSlurpByName(const void *rr1, const void *rr2) { return sortSlurp("sl_Name", "", rr1, rr2); }
int sortSlurpByID  (const void *rr1, const void *rr2) { return sortSlurp("slurpID", "", rr1, rr2); }
