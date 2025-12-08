// Extract.h

#ifndef ZIP7_INC_EXTRACT_H
#define ZIP7_INC_EXTRACT_H

#include "../../../Windows/FileFind.h"
// **************** 0xLC Modification Start ****************
#include "../../../Windows/FileDir.h"
#include "../../../Windows/FileName.h"
// **************** 0xLC Modification  End  ****************

#include "../../Archive/IArchive.h"

#include "ArchiveExtractCallback.h"
#include "ArchiveOpenCallback.h"
#include "ExtractMode.h"
#include "Property.h"

#include "../Common/LoadCodecs.h"

struct CExtractOptionsBase
{
  CBoolPair ElimDup;
  
  // **************** 0xLC Modification Start ****************
  CBoolPair SmartExtract;
  struct SmartExtractResult
  {
	UString baseFolder{};
	UString newFolder{};
	UString rootItem{};
	
	UString GetFinalPath(const UString& _base)
	{
		baseFolder = _base.IsEmpty() ? baseFolder : _base;
		NWindows::NFile::NName::NormalizeDirPathPrefix(baseFolder);
		return newFolder.IsEmpty() ? newFolder : baseFolder + newFolder;
	}
	
	void CheckConflict(const UString& _base, const UString& _will, const UString& _new)
	{
		if(_will.IsEmpty() || _new.IsEmpty()) return;
		
		baseFolder = _base.IsEmpty() ? baseFolder : _base;
		NWindows::NFile::NName::NormalizeDirPathPrefix(baseFolder);
		UString checkItem = baseFolder + _will;
		
		const DWORD attrib = NWindows::NFile::NFind::GetFileAttrib(checkItem);
		if(attrib != INVALID_FILE_ATTRIBUTES)
		{
			newFolder = _new;
			NWindows::NFile::NName::NormalizeDirPathPrefix(newFolder);
		}
	}
  };
  mutable SmartExtractResult SmartResult;
  // **************** 0xLC Modification  End  ****************

  bool ExcludeDirItems;
  bool ExcludeFileItems;

  bool PathMode_Force;
  bool OverwriteMode_Force;
  NExtract::NPathMode::EEnum PathMode;
  NExtract::NOverwriteMode::EEnum OverwriteMode;
  NExtract::NZoneIdMode::EEnum ZoneMode;

  CExtractNtOptions NtOptions;
  
  FString OutputDir;
  UString HashDir;

  CExtractOptionsBase():
      ExcludeDirItems(false),
      ExcludeFileItems(false),
      PathMode_Force(false),
      OverwriteMode_Force(false),
      PathMode(NExtract::NPathMode::kFullPaths),
      OverwriteMode(NExtract::NOverwriteMode::kAsk),
      ZoneMode(NExtract::NZoneIdMode::kNone)
      {}
};

struct CExtractOptions: public CExtractOptionsBase
{
  bool StdInMode;
  bool StdOutMode;
  // **************** 0xLC Modification Start ****************
  bool EnterFolder;
  UString EnterParamTarget{};
  // **************** 0xLC Modification  End  ****************
  bool YesToAll;
  bool TestMode;
  
  // bool ShowDialog;
  // bool PasswordEnabled;
  // UString Password;
  #ifndef Z7_SFX
  CObjectVector<CProperty> Properties;
  #endif

  /*
  #ifdef Z7_EXTERNAL_CODECS
  CCodecs *Codecs;
  #endif
  */

  CExtractOptions():
      StdInMode(false),
      StdOutMode(false),
	  // **************** 0xLC Modification Start ****************
	  EnterFolder(false),
	  // **************** 0xLC Modification  End  ****************
      YesToAll(false),
      TestMode(false)
      {}
};

struct CDecompressStat
{
  UInt64 NumArchives;
  UInt64 UnpackSize;
  UInt64 AltStreams_UnpackSize;
  UInt64 PackSize;
  UInt64 NumFolders;
  UInt64 NumFiles;
  UInt64 NumAltStreams;

  void Clear()
  {
    NumArchives = UnpackSize = AltStreams_UnpackSize = PackSize = NumFolders = NumFiles = NumAltStreams = 0;
  }
};

HRESULT Extract(
    // DECL_EXTERNAL_CODECS_LOC_VARS
    CCodecs *codecs,
    const CObjectVector<COpenType> &types,
    const CIntVector &excludedFormats,
    UStringVector &archivePaths, UStringVector &archivePathsFull,
    const NWildcard::CCensorNode &wildcardCensor,
    const CExtractOptions &options,
    IOpenCallbackUI *openCallback,
    IExtractCallbackUI *extractCallback,
    IFolderArchiveExtractCallback *faeCallback,
    #ifndef Z7_SFX
    IHashCalc *hash,
    #endif
    UString &errorMessage,
    CDecompressStat &st);

#endif
