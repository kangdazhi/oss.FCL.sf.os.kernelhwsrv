// Copyright (c) 2008-2009 Nokia Corporation and/or its subsidiary(-ies).
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
// f32\sfat\sl_dir_cache.inl
// 
//

/**
 @file
 @internalTechnology
*/

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!
//!! WARNING!! DO NOT edit this file !! '\sfat' component is obsolete and is not being used. '\sfat32'replaces it
//!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


#ifndef SL_DIR_CACHE_INL
#define SL_DIR_CACHE_INL

#include "sl_dir_cache.h"

/**
Get function of TDynamicDirCachePage.
@return TInt64  the starting media address of the page content.
*/
TInt64 TDynamicDirCachePage::StartPos() const
    {
    return iStartMedPos;
    }

/**
Get function of TDynamicDirCachePage.
@return TUint8* the starting ram content of the page content.
*/
TUint8* TDynamicDirCachePage::StartPtr() const
    {
    return iStartRamAddr;
    }

/**
Set function of TDynamicDirCachePage.
@param  aPtr    starting RAM Ptr that holds the cache page data.
*/
void TDynamicDirCachePage::SetStartPtr(TUint8* aPtr)
    {
    iStartRamAddr = aPtr;
    }

/**
Set function of TDynamicDirCachePage.
@param  aIsValid    boolean value to set validity of the page content.
*/
void TDynamicDirCachePage::SetValid(TBool aIsValid)
    {
    iValid = aIsValid;
    }

/**
Get function of TDynamicDirCachePage.
@return TBool   boolean value that indicates validity of the page content.
*/
TBool TDynamicDirCachePage::IsValid() const
    {
    return iValid;
    }

/**
Set function of TDynamicDirCachePage.
@param  aLocked flag that sets if the page is locked or not.
*/
void TDynamicDirCachePage::SetLocked(TBool aLocked)
    {
    iLocked = aLocked;
    }

/**
Get function of TDynamicDirCachePage.
@return TBool   boolean value that indicates if the page is locked.
*/
TBool TDynamicDirCachePage::IsLocked() const
    {
    return iLocked;
    }

/**
Set function of TDynamicDirCachePage.
@param  aType   set page type: EUnknown, ELocked, EUnlocked or EActivePage.
*/
void TDynamicDirCachePage::SetPageType(TDynamicDirCachePage::TPageType aType)
    {
    iType = aType;
    }

/**
Get function of TDynamicDirCachePage.
@return TPageType   get page type: EUnknown, ELocked, EUnlocked or EActivePage.
*/
TDynamicDirCachePage::TPageType TDynamicDirCachePage::PageType()
    {
    return iType;
    }

/**
Get function of TDynamicDirCachePage.
@return TUint32 page size in bytes.
*/
TUint32 TDynamicDirCachePage::PageSizeInBytes() const
    {
    return 1 << iOwnerCache->PageSizeInBytesLog2();
    }

/**
Deque the page from its queue.
@see    TDblQueLink::Deque()
*/
void TDynamicDirCachePage::Deque()
    {
    iLink.Deque();
    }

/**
Get function of TDynamicDirCachePage.
@return TUint32 page size in segments.
*/
TUint32 TDynamicDirCachePage::PageSizeInSegs() const
    {
    return iOwnerCache->PageSizeInSegs();
    }

/**
Interpret the media address into ram address.
@param  aPos    the media address to be interpreted
@return TUint8* the ram content pointer that contains that media content.
*/
TUint8* TDynamicDirCachePage::PtrInPage(TInt64 aPos) const
    {
    ASSERT(PosCachedInPage(aPos));
    return iStartRamAddr + (((TUint32)aPos - (TUint32)iStartMedPos) & (PageSizeInBytes() - 1));
    }

/**
Query function, to check if the media address is contained in the page.
@param  aPos    the media address to be queried.
@return TBool   ETrue if the media address is cached in the page, otherwise EFalse.
*/
TBool TDynamicDirCachePage::PosCachedInPage(TInt64 aPos) const
    {
    return (aPos >= iStartMedPos && aPos < iStartMedPos + PageSizeInBytes());
    }

/**
Reset the media address to 0, invalidate page content.
*/
void TDynamicDirCachePage::ResetPos()
    {
    iStartMedPos = 0;
    SetValid(EFalse);
    }

/**
Set page starting media address, invalidate page content.
@param  aPos    the new media address to be set.
*/
void TDynamicDirCachePage::SetPos(TInt64 aPos)
    {
    iStartMedPos = aPos;
    SetValid(EFalse);
    return;
    }


//========================================================================
/**
Calculate the page starting media address, aligned with page size.
@param  aPos    the media address to be aligned.
@return TInt64  the aligned media address.
*/
TInt64 CDynamicDirCache::CalcPageStartPos(TInt64 aPos) const
    {
    ASSERT(aPos >= iCacheBasePos);
    return (((aPos - iCacheBasePos) >> iPageSizeLog2) << iPageSizeLog2) + iCacheBasePos;
    }

/**
Check if the cache has reached its limited page number.
@return TBool   ETrue if cache is full, otherwise EFalse.
*/
TBool CDynamicDirCache::CacheIsFull() const
    {
    // active page, locked page and unlocked page
    return (iLockedQCount + iUnlockedQCount + 1 >= iMaxSizeInPages);
    }

/**
Return the maximum allowed page number of the cache.
*/
TUint32 CDynamicDirCache::MaxCacheSizeInPages() const
    {
    return iMaxSizeInPages;
    }

#endif //SL_DIR_CACHE_INL

