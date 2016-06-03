#ifndef _OPTIONS_H_
#define _OPTIONS_H_
/*--------------------------------------------------------------------------------
 * Copyright 2016 - Great Hill Corporation.
 --------------------------------------------------------------------------------*/

//--------------------------------------------------------------------------------
class COptions
{
public:
	SFBool   slurp;
	SFBool   prettyPrint;
	SFBool   rerun;
	SFBool   incomeOnly;
	SFBool   expenseOnly;
	SFInt32  firstBlock;
	SFInt32  lastBlock;
	SFTime   firstDate;
	SFTime   lastDate;
	SFBool   openFile;
	SFString addr;
	SFInt32  maxTransactions;
	SFInt32  pageSize;
	SFString exportFormat;
	FILE    *output; // for use when -a is on

	        COptions       (void);
	       ~COptions       (void);
	SFInt32 parseArguments (SFInt32 nArgs, const SFString *args);
};

//--------------------------------------------------------------------------------
typedef SFInt32 (*USAGEFUNC)(const SFString& errMsg);

//--------------------------------------------------------------------------------
class CParams
{
public:
	SFString  shortName;
	SFString  longName;
	SFString  description;
	CParams( const SFString& name, const SFString& descr );
};

//--------------------------------------------------------------------------------
class CCmdFunction
{
	CCmdFunction( void ) {}
public:
	CParams    *params;
	SFInt32     nParams;
	CCmdFunction (CParams *paramsIn, SFInt32 nParamsIn)
	{
		params       = paramsIn;
		nParams      = nParamsIn;
	}
	SFString options     (void) const;
	SFString descriptions(void) const;
	SFString purpose     (void) const;
};

extern SFString expandOption(SFString& arg);

#endif