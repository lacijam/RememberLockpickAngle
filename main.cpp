#include "skse64/PluginAPI.h"		// super
#include "skse64_common/skse_version.h"	// What version of SKSE is running?
#include <shlobj.h>				// CSIDL_MYCODUMENTS
#include <skse64_common/SafeWrite.h>

#include "GameMenus.h"
#include "GameEvents.h"

/*
code in LockpickBreak()
sets Lockpick angle to 0

.text:00000001408985DD                 mov     [rsi+0DCh], r14d
*/

static RelocAddr<uintptr_t> resetPickAngle_offset = (0x008985DD);

static PluginHandle					g_pluginHandle = kPluginHandle_Invalid;
static SKSEPapyrusInterface         * g_papyrus = NULL;

extern "C"	{

	bool SKSEPlugin_Query(const SKSEInterface * skse, PluginInfo * info)	{	// Called by SKSE to learn about this plugin and check that it's safe to load it
		gLog.OpenRelative(CSIDL_MYDOCUMENTS, "\\My Games\\Skyrim Special Edition\\SKSE\\Plugin.log");
		gLog.SetPrintLevel(IDebugLog::kLevel_Error);
		gLog.SetLogLevel(IDebugLog::kLevel_DebugMessage);

		_MESSAGE("Plugin");

		// populate info structure
		info->infoVersion =	PluginInfo::kInfoVersion;
		info->name =		"Plugin";
		info->version =		1;

		// store plugin handle so we can identify ourselves later
		g_pluginHandle = skse->GetPluginHandle();

		if(skse->isEditor)
		{
			_MESSAGE("loaded in editor, marking as incompatible");

			return false;
		}

		// ### do not do anything else in this callback
		// ### only fill out PluginInfo and return true/false

		// supported runtime version
		return true;
	}

	bool SKSEPlugin_Load(const SKSEInterface * skse)	{	// Called by SKSE to load this plugin
		_MESSAGE("Plugin loaded");

		_MESSAGE("Patching LockpickBreak() function...");
		SafeWrite32(resetPickAngle_offset, 0x90909090);
		SafeWrite32(resetPickAngle_offset + 4, 0x90909048);
		_MESSAGE("Success!");

		return true;
	}

};
