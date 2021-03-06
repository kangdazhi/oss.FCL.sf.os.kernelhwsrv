// Copyright (c) 2007-2009 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of the License "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Trace API
//

/**
* @internalTechnology
* @deprecated
*/

#ifndef TRACEUTILS_H_
#define TRACEUTILS_H_
#include <e32btrace.h>


/**
Maximum length of a formatted string
*/
const static TInt KMaxPrintfSize = 256;

/**
Dummy class to toss away overflow

@internalComponent
*/
#ifndef __KERNEL_MODE__
class TTruncateOverflow16 : public TDes16Overflow
	{
	public:
	virtual void Overflow(TDes&) {};
	};

/**
Dummy class to toss away overflow

@internalComponent
*/
class TTruncateOverflow8 : public TDes8Overflow
	{
	public:
	virtual void Overflow(TDes8&) {};
	};
#endif  //__KERNEL_MODE__


/**
@internalComponent
*/
#define OST_HEADER(aSize,aGroupId,aContext,aPc)																\
	((((aSize) + (aContext?4:0) + (aPc?4:0)) << BTrace::ESizeIndex*8)										\
	+(((aContext?BTrace::EContextIdPresent:0) | (aPc?BTrace::EPcPresent:0)) << BTrace::EFlagsIndex*8)			\
	+((aGroupId) << BTrace::ECategoryIndex*8)																				\
	+((0) << BTrace::ESubCategoryIndex*8))


/**
@internalComponent
*/
#define OST_SECONDARY_0(aGroupId,aComponentId,aThreadIdPresent,aPcPresent,aPc,aTraceId)	\
	BTrace::OutFilteredPcFormatBig(OST_HEADER(8,aGroupId,aThreadIdPresent,aPcPresent),(TUint32)(aComponentId),aPc,aTraceId,0,0)

/**
@internalComponent
*/
#define OST_SECONDARY_1(aGroupId,aComponentId,aThreadIdPresent,aPcPresent,aPc,aTraceId, aData1) \
	BTrace::OutFilteredPcFormatBig(OST_HEADER(8,aGroupId,aThreadIdPresent,aPcPresent),(TUint32)(aComponentId),aPc,aTraceId,&aData1,4)

/**
@internalComponent
*/
#define OST_SECONDARY_ANY(aGroupId, aComponentId, aThreadIdPresent, aPcPresent, aPc, aTraceId, aData, aDataSize) \
	BTrace::OutFilteredPcFormatBig(OST_HEADER(8,aGroupId,aThreadIdPresent,aPcPresent),(TUint32)(aComponentId),aPc,aTraceId,aData,(TInt)(aDataSize))


#ifdef __MARM_ARMV5__
	//armv5
#define GET_PC(pc) \
	TUint32 pc = 0; \
	asm("mov pc, __return_address()")
#elif __MARM_ARM4__
	//arm4 not implemented yet!
#define GET_PC(pc) \
	TUint32 pc = 0;
#elif __WINS__
	//wins
#define GET_PC(pc) \
	TUint32 pc = 0; \
	_asm push edx \
	_asm mov edx, [ebp+4] \
	_asm mov [pc], edx \
	_asm pop edx
	//This is instead of doing "asm(mov pc, ebp+4)" as that gives warnings about registers being spilled.
#elif __X86__ 
	//x86 not implemented yet!
#define GET_PC(pc) \
	TUint32 pc = 0;
/*	asm("push edx"); \
	asm("mov %0, [edx+4]": "=r" (pc)); \
	asm("pop edx") */
#else
//other platforms
#define GET_PC(pc) \
	TUint32 pc = 0;
#endif

#endif /*TRACEUTILS_H_*/
