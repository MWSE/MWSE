#pragma once

// Intentionally not populated. As of this writing the only consumers of
// NI::VirtualTableAddress live in the MWSE project (CrashLogLabelsMWSE,
// LuaManager, TES3Reference/Object/MobileObject, CrashLogExceptionHandler)
// and never reach this file. The CS equivalents have not been catalogued.
//
// If you hit this #error it means a SharedSE or CSSE consumer just started
// reaching for NI::VirtualTableAddress::* on the CS target. Populate this
// file with the CS vtable addresses before continuing — otherwise the
// enum members the consumer expects will not exist and you'll get a
// less informative "is not a member of NI::VirtualTableAddress" error.
#error "NIVirtualTableDefines.TESConstructionSet.h is a stub. Populate before use."
