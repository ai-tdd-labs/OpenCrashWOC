#!/usr/bin/env python3
"""
Extract missing Nu2 engine structs, unions, and enums from the Crash Bandicoot
alpha NGC DWARF dump and generate C type definitions for nu2_types.h.

Parses the DWARF dump format:
  struct name { // total size: 0xNN
      type field; // offset 0xNN, size 0xNN
  };
  enum name { VAL = 0x0, ... };
  union name { ... };
  typedef struct { ... } TypedefName;

Filters out platform SDK types (D3D, GX, Windows, etc.) and types already
present in nu2_types.h.
"""

import re
import sys
import os
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[3]
DWARF_PATH = str((REPO_ROOT / "code" / "src" / "dump_alphaNGCport_DWARF.txt").resolve())
NU2_TYPES_PATH = str((REPO_ROOT / "analysis" / "ghidra" / "tooling" / "nu2_types.h").resolve())

# ============================================================================
# Type name filters - skip these prefixes/patterns
# ============================================================================

SKIP_PREFIXES = [
    # Xbox/D3D
    "D3D", "_D3D", "IDirect3D", "Direct3D", "ID3DX",
    "D3DXVECTOR", "D3DXCOLOR", "D3DX",
    # Nintendo SDK - ALL GX/AX/GBA/DVD/OS/SI/EXI/AR/DSP/CARD/PAD/VI prefixes
    "_GX", "__GX", "GX", "GBA", "Gba",
    "_AX", "AX_",
    "VI_TV", "VI_XFB",
    "uPPCWGPipe", "PPC_",
    # Nintendo SDK / Dolphin OS structures
    "OS_THREAD", "OSBoot", "OSCalendar", "OSFont", "OSImport", "OSModule",
    "OSRel", "OSReset", "OSSection", "OSSemaphore", "OSSram", "OSStop",
    "OSThread", "OSAlarm", "OSContext", "OSMutex", "OSCond", "OSMessage",
    "DVDFile", "DVDCommand", "DVDDir", "DVDBB", "DVDDisk", "DVDDrive",
    "CARD", "PADClamp", "PADStatus",
    "SIControl", "SIPacket", "SIMWHEEL",
    "EXIControl",
    "ARQRequest", "ARQ",
    "STRUCT_DSP", "SPAdpcm", "SPSound",
    # Nintendo SDK additional
    "__AM",
    # Windows/Microsoft
    "HWND__", "HHOOK__", "HKEY__", "HACCEL__", "HBITMAP__", "HBRUSH__",
    "HCOLORSPACE__", "HDC__", "HGLRC__", "HDESK__", "HENHMETAFILE__",
    "HFONT__", "HICON__", "HMENU__", "HMETAFILE__", "HINSTANCE__",
    "HPALETTE__", "HPEN__", "HRGN__", "HRSRC__", "HSTR__", "HTASK__",
    "HWINSTA__", "HKL__", "HMONITOR__", "HWINEVENTHOOK__",
    "_OVERLAPPED", "_MEMORYSTATUS", "_FILETIME", "_WIN32_FIND_DATA",
    "_XPP_DEVICE_TYPE", "_XDEVICE_PREALLOC_TYPE", "_XINPUT_",
    "_LAUNCH_DATA", "_LD_LAUNCH_DASHBOARD", "_XCALCSIG_",
    "_XGAME_FIND_DATA", "_RECTL", "_POINTL",
    "_SYSTEMTIME",
    "tagRECT", "tagPOINT", "tagSIZE", "tagPOINTS", "tagPALETTEENTRY",
    "tagBITMAP",
    "IUnknown", "FpscrUnion",
    # DEMO/debug SDK types
    "DEMO",
]

SKIP_EXACT = {
    # C stdlib
    "__va_list_tag", "_LARGE_INTEGER", "_ULARGE_INTEGER", "_GUID",
    "tm", "_glue", "__sFILE", "__sbuf", "_reent", "_Bigint", "_atexit",
    "exception", "__exception", "__dmath",
    "div_t", "ldiv_t",
    # Platform
    "Direct3D", "D3DDevice", "D3DResource", "D3DTexture", "D3DCubeTexture",
    "D3DSurface", "D3DBaseTexture", "D3DVertexBuffer", "D3DIndexBuffer",
    "D3DPalette",
    # Nintendo SDK / Dolphin specific
    "Cell", "HeapDesc", "DBInterface", "DTKTrack", "FSTEntry",
    "SramControl", "GBABootInfo", "GBAControl", "GBASecParam",
    "MIXChannel", "SS_VOICE",
    # PS2 SDK
    "sceGifTag",
    # Nintendo SDK struct types with odd naming
    "__GXData_struct", "__GXFifoObj", "__GXLightObjInt_struct",
    "__GXTexObjInt_struct", "__GXTexRegionInt_struct",
    "__GXTlutObjInt_struct", "__GXTlutRegionInt_struct",
    "__GXVerifyData",
    # Nintendo SDK typedef types that leaked through
    "GXFifoObj", "GXTexObj", "GXTlutObj", "GXLightObj",
    "GXTlutFmt", "GXTexWrapMode", "GXTexFilter",
    "GXWarningLevel", "VITVMode", "VIXFBMode",
    "DVDFileInfo", "DVDCommandBlock",
    "__AMREADINFO", "meminfo",
    # NGC-specific texture wrapper structs (use GX SDK types in fields)
    "CLUTHeader", "TEXHeader", "TEXDescriptor", "TEXPalette",
    "MyCopyTexObj",
    # Xbox/PC-specific render structs (use D3D/GUID types)
    "nudx_deviceinfo", "nudx_screenmodefilter_s",
    "rendertargetlist_s", "shaderparams_s",
    "_SHADERTYPE",
}

SKIP_ENUM_PREFIXES = [
    "_D3D", "D3D", "__fdlibm", "_MAX_FVF",
    "_GX", "GX",  # Nintendo GX enums
    "_GS_BLEND", "_GS_COMPARE", "_GS_CULLING",
    "OS_THREAD",
    "DEMO",
    "VI_TV", "VI_XFB",  # Nintendo VI enums
]

SKIP_ENUM_EXACT = {
    "__fdlibm_version",
    "GbaState", "DMTexFlt",
    "RBCLASSTYPE",  # Not a useful standalone type
    "GXWarningLevel", "VITVMode", "VIXFBMode",
    # Xbox/PC-specific shader/vertex enums
    "shadertypes_e", "vtxdecltype",
}


def should_skip_type(name):
    """Return True if this type name should be filtered out."""
    if name in SKIP_EXACT:
        return True
    for prefix in SKIP_PREFIXES:
        if name.startswith(prefix):
            return True
    return False


def should_skip_enum(name):
    """Return True if this enum name should be filtered out."""
    if name in SKIP_ENUM_EXACT:
        return True
    for prefix in SKIP_ENUM_PREFIXES:
        if name.startswith(prefix):
            return True
    return should_skip_type(name)


# ============================================================================
# Parse existing nu2_types.h to find already-defined types
# ============================================================================

def get_existing_types(path):
    """Extract all struct/union/enum names already defined in nu2_types.h."""
    existing = set()
    with open(path, "r") as f:
        text = f.read()

    # Match: struct name {, union name {, enum name {
    for m in re.finditer(r'\b(?:struct|union|enum)\s+(\w+)\s*\{', text):
        existing.add(m.group(1))

    # Match typedef struct { ... } Name;
    for m in re.finditer(r'typedef\s+(?:struct|union|enum)\s*\{[^}]*\}\s*(\w+)\s*;', text, re.DOTALL):
        existing.add(m.group(1))

    # Also match typedef struct name { already covered above
    return existing


# ============================================================================
# DWARF dump type mapping: DWARF typedef names -> C struct names
# ============================================================================

# Map from DWARF typedef names to the actual struct/pointer type to use in output
TYPE_MAP = {
    "NUVEC": "struct nuvec_s",
    "NUVEC4": "struct nuvec4_s",
    "NUIVEC": "struct nuivec_s",
    "NUIVEC4": "struct nuivec4_s",
    "NUANGVEC": "struct nuangvec_s",
    "NUPLANE": "struct nuplane_s",
    "NUMTX": "struct numtx_s",
    "NUQUAT": "struct nuquat_s",
    "NUCOLOUR3": "struct nucolour3_s",
    "NUCOLOUR4": "struct nucolour4_s",
    "NUCAMERA": "struct nucamera_s",
    "NUREFLECT": "struct nureflect_s",
    "NULIGHT": "struct nulight_s",
    "NUMTL": "struct numtl_s",
    "NUMTL_T": "struct numtl_s",
    "NUMTLATTRIB": "struct numtlattrib_s",
    "NUFX": "union nufx_u",
    "NUTEX": "struct nutex_s",
    "NUGOBJ": "struct nugobj_s",
    "NUGEOM": "struct nugeom_s",
    "NUFACEONGEOM": "struct nufaceongeom_s",
    "NUFACEON": "struct nufaceon_s",
    "NUPRIM": "struct nuprim_s",
    "NUSKIN": "struct nuskin_s",
    "NUVTXSKININFO": "struct NUVTXSKININFO_s",
    "NUBLENDGEOM": "struct NUBLENDGEOM_s",
    "NUNODE": "struct NUNODE_s",
    "NUSPLINE": "struct nuspline_s",
    "NUGSPLINE": "struct nugspline_s",
    "NUGSCENE": "struct nugscn_s",
    "NUSCENE": "struct nuscene_s",
    "NUINSTANCE": "struct nuinstance_s",
    "NUINSTFLAGS": "struct nuinstflags_s",
    "NUINSTANIM": "struct nuinstanim_s",
    "NUSPECIAL": "struct nuspecial_s",
    "NUSPECIALFLAGS": "struct nuspecialflags",
    "NUFSPECIAL": "struct nufspecial_s",
    "NUHSPECIAL": "struct nuhspecial_s",
    "NUHGOBJ": "struct NUHGOBJ_s",
    "NUJOINTDATA": "struct NUJOINTDATA_s",
    "NULAYERDATA": "struct NULAYERDATA_s",
    "NUPOINTOFINTEREST": "struct NUPOINTOFINTEREST_s",
    "NUSHADOWDATA": "struct NUSHADOWDATA_s",
    "NUELLIPSOID": "struct NUELLIPSOID_s",
    "NUCOLLISIONDATA": "struct NUCOLLISIONDATA_s",
    "NUCOLLISIONHDR": "struct NUCOLLISIONHDR_s",
    "NUANIMDATA": "struct nuanimdata_s",
    "NUANIMDATA2": "struct nuanimdata2_s",
    "NUANIMKEY": "struct nuanimkey_s",
    "NUANIMCURVE": "struct nuanimcurve_s",
    "NUANIMCURVESET": "struct nuanimcurveset_s",
    "NUANIMCURVEDATA": "struct nuanimcurvedata_s",
    "NUANIMCURVE2": "struct nuanimcurve2_s",
    "NUANIMDATACHUNK": "struct nuanimdatachunk_s",
    "NUANIMDATAHDR": "struct NUANIMDATAHDR_s",
    "NUANIMTIME": "struct nuanimtime_s",
    "NUJOINTANIM": "struct NUJOINTANIM_s",
    "NUTEXANIM": "struct nutexanim_s",
    "NUTEXANIMENV": "struct nutexanimenv_s",
    "NUTEXANIMPROG": "struct nutexanimprog_s",
    "NUTEXANIMF": "struct nutexanimf_s",
    "NUPAD": "struct nupad_s",
    "NUVIEWPORT": "struct nuviewport_s",
    "NURAND": "struct nunrand_s",
    "NUIFFHDR": "struct nuiffhdr_s",
    "NUMEMBLK": "struct numemblk_s",
    "NURNDRITEM": "struct nurndritem_s",
    "NUSYSMTL": "struct nusysmtl_s",
    "VARIPTR_T": "union variptr_u",
    "VARIPTR": "union variptr_u",
    # Scene node types
    "NUMESHNODE": "struct NUMESHNODE_s",
    "NUTRANSFORMNODE": "struct NUTRANSFORMNODE_s",
    "NUJOINTNODE": "struct NUJOINTNODE_s",
    "NUSKINNODE": "struct NUSKINNODE_s",
    "NUREFERENCENODE": "struct NUREFERENCENODE_s",
    "NUSPLINENODE": "struct NUSPLINENODE_s",
    "NUNODEPTR": "union nunodeptr_s",
    # Collision
    "NUCOLLISIONSPHERE": "struct NUCOLLISIONSPHERE_s",
    # Cutscene
    "NUCUTSCENE": "struct NUCUTSCENE_s",
    "NUCUTSCENECAMERA": "struct NUCUTSCENECAMERA_s",
    "NUHINSTANCE": "struct NUHINSTANCE_s",
    "NUCUTSCENEAUDIO": "struct NUCUTSCENEAUDIO_s",
    # Game cutscene
    "NUGCUTSCENE": "struct NUGCUTSCENE_s",
    "NUGCUTCAM": "struct NUGCUTCAM_s",
    "NUGCUTCAMTGT": "struct NUGCUTCAMTGT_s",
    "NUGCUTCAMSYS": "struct NUGCUTCAMSYS_s",
    "NUGCUTRIGID": "struct NUGCUTRIGID_s",
    "NUGCUTRIGIDSYS": "struct NUGCUTRIGIDSYS_s",
    "NUGCUTCHAR": "struct NUGCUTCHAR_s",
    "NUGCUTCHARSYS": "struct NUGCUTCHARSYS_s",
    "NUGCUTLOCATOR": "struct NUGCUTLOCATOR_s",
    "NUGCUTLOCATORSYS": "struct NUGCUTLOCATORSYS_s",
    "NUGCUTLOCATORTYPE": "struct NUGCUTLOCATORTYPE_s",
    "NUGCUTTRIGGER": "struct NUGCUTTRIGGER_s",
    "NUGCUTTRIGGERSYS": "struct NUGCUTTRIGGERSYS_s",
    "NUGCUTBBOX": "struct NUGCUTBBOX_s",
    "NUGCUTLOCFNDATA": "struct NUGCUTLOCFNDATA_s",
    "NUSTATEANIM": "struct NUSTATEANIM_s",
    # Trigger
    "NUTRIGGER": "struct NUTRIGGER_s",
    "NUTRIGGERSYS": "struct NUTRIGGERSYS_s",
    "NUTRIGGERPRIM": "struct NUTRIGGERPRIM_s",
    "NUTRIGGERPRIMTYPES": "int",  # enum used as type
    "NUTRIGGERTYPE": "int",  # enum used as type
    # Instance triggers
    "instNUTRIGGER": "struct instNUTRIGGER_s",
    "instNUTRIGGERSYS": "struct instNUTRIGGERSYS_s",
    "instNUGCUTCAM": "struct instNUGCUTCAM_s",
    "instNUGCUTCAMSYS": "struct instNUGCUTCAMSYS_s",
    "instNUGCUTRIGID": "struct instNUGCUTRIGID_s",
    "instNUGCUTRIGIDSYS": "struct instNUGCUTRIGIDSYS_s",
    "instNUGCUTCHAR": "struct instNUGCUTCHAR_s",
    "instNUGCUTCHARSYS": "struct instNUGCUTCHARSYS_s",
    "instNUGCUTLOCATOR": "struct instNUGCUTLOCATOR_s",
    "instNUGCUTLOCATORSYS": "struct instNUGCUTLOCATORSYS_s",
    "instNUGCUTTRIGGER": "struct instNUGCUTTRIGGER_s",
    "instNUGCUTTRIGGERSYS": "struct instNUGCUTTRIGGERSYS_s",
    "instNUGCUTSCENE": "struct instNUGCUTSCENE_s",
    # Creature/Game types
    "CREATURE": "struct creature_s",
    "AnimPacket": "struct anim_s",
    "GameObject": "struct obj_s",
    "RailPosition": "struct RPos_s",
    # Crate
    "CRATE": "struct crate_s",
    "CRATE_TYPE": "struct crate_type_s",
    # Debris
    "DEB3_T": "struct deb3_s",
    "DEB3": "struct deb3_s",
    "debrissphere": "struct debrissphere_s",
    "debkeydatatype": "struct debkeydatatype_s",
    # Xbox input (used in nupad_s cross-platform)
    "XINPUT_STATE": "char[0x18]",
    "XINPUT_FEEDBACK": "char[0x48]",
    # Enum types used as field types
    "NUTEXTYPE": "int",
    "NUGOBJTYPE": "int",
    "NUVTXTYPE": "int",
    "NUPRIMTYPE": "int",
    "NUFACEONTYPE": "int",
    "NUCULLTYPE": "int",
    "NUCAMFX": "int",
    "LOCATORDIRECTION": "int",
}


def resolve_type(dwarf_type):
    """
    Convert a DWARF typedef type to a Ghidra-compatible C type.
    Handles pointers, arrays, const, etc.
    """
    t = dwarf_type.strip()

    # Remove 'const' qualifier
    t = re.sub(r'\bconst\b\s*', '', t).strip()

    # Handle function pointers: type (* name)(args)
    # We'll convert to void* for Ghidra compatibility
    if '(*)' in t or '(*' in t:
        return "void *"

    # Handle arrays - extract base type and array suffix
    array_suffix = ""
    arr_m = re.match(r'^(.+?)(\[[^\]]*\].*)$', t)
    if arr_m:
        t = arr_m.group(1).strip()
        array_suffix = arr_m.group(2)

    # Count pointer levels
    ptr_count = 0
    while t.endswith('*'):
        ptr_count += 1
        t = t[:-1].strip()

    # Check if it's a struct/union/enum reference
    struct_match = re.match(r'^(struct|union|enum)\s+(\w+)$', t)
    if struct_match:
        resolved = t  # Already has struct/union/enum keyword
    elif t in TYPE_MAP:
        resolved = TYPE_MAP[t]
    else:
        # Standard C types mapping
        c_type_map = {
            "unsigned char": "u8",
            "signed char": "s8",
            "unsigned short": "u16",
            "short": "short",
            "unsigned int": "u32",
            "int": "int",
            "unsigned long": "u32",
            "long": "long",
            "long long": "s64",
            "unsigned long long": "u64",
            "float": "float",
            "double": "double",
            "void": "void",
            "char": "char",
        }
        resolved = c_type_map.get(t, t)

    # Re-add pointers
    result = resolved
    for _ in range(ptr_count):
        result += " *"

    # Re-add array suffix
    result += array_suffix

    return result


# ============================================================================
# DWARF dump parser
# ============================================================================

def parse_dwarf_dump(path):
    """
    Parse the DWARF dump file and extract all struct, union, enum, and
    typedef struct/enum definitions.

    Returns:
        structs: dict of name -> { 'size': str, 'fields': [(type, name, offset, size), ...], 'kind': 'struct'|'union' }
        enums: dict of name -> [(value_name, value), ...]
        typedef_structs: dict of TypedefName -> { 'size': str, 'fields': [...], 'kind': 'struct' }
        typedef_enums: dict of TypedefName -> [(value_name, value), ...]
    """
    structs = {}
    enums = {}
    typedef_structs = {}
    typedef_enums = {}

    with open(path, "r") as f:
        lines = f.readlines()

    i = 0
    n = len(lines)

    while i < n:
        line = lines[i].rstrip('\n')

        # Match: struct name { or union name {
        m = re.match(r'^(struct|union)\s+(\w+)\s*\{', line)
        if m:
            kind = m.group(1)
            name = m.group(2)
            i, body = parse_struct_body(lines, i, n)

            # Only keep first occurrence
            if name not in structs:
                structs[name] = body
                structs[name]['kind'] = kind
            continue

        # Match: enum name {
        m = re.match(r'^enum\s+(\w+)\s*\{', line)
        if m:
            name = m.group(1)
            i, values = parse_enum_body(lines, i, n)
            if name not in enums:
                enums[name] = values
            continue

        # Match: typedef struct { ... } Name;
        m = re.match(r'^typedef\s+(struct|union|enum)\s*\{', line)
        if m:
            typedef_kind = m.group(1)
            if typedef_kind in ('struct', 'union'):
                i, body = parse_struct_body(lines, i, n)
                # Look for the typedef name on the closing line
                # The closing }; line was consumed, but the typedef name follows
                # Actually, parse_struct_body stops at };, need to check line after
                # Let's re-check - parse_struct_body returns the line AFTER };
                # The }; line may contain: } TypeName;
                close_line = lines[i - 1].rstrip('\n') if i > 0 else ""
                tm = re.match(r'^\}\s*(\w+)\s*;', close_line)
                if tm:
                    tname = tm.group(1)
                    if tname not in typedef_structs:
                        body['kind'] = typedef_kind
                        typedef_structs[tname] = body
            elif typedef_kind == 'enum':
                i, values = parse_enum_body(lines, i, n)
                close_line = lines[i - 1].rstrip('\n') if i > 0 else ""
                tm = re.match(r'^\}\s*(\w+)\s*;', close_line)
                if tm:
                    tname = tm.group(1)
                    if tname not in typedef_enums:
                        typedef_enums[tname] = values
            else:
                i += 1
            continue

        i += 1

    return structs, enums, typedef_structs, typedef_enums


def parse_struct_body(lines, start_i, n):
    """
    Parse a struct/union body starting from the opening line.
    Returns (next_line_index, body_dict).
    body_dict = { 'size': total_size_str, 'fields': [(type_str, name, offset_str, size_str), ...] }
    """
    fields = []
    total_size = None

    i = start_i + 1  # Skip opening line

    # Check if total size is on next line
    if i < n:
        size_m = re.match(r'\s*//\s*total\s+size:\s*(0x[0-9A-Fa-f]+|\d+)', lines[i].strip())
        if size_m:
            total_size = size_m.group(1)
            i += 1

    while i < n:
        line = lines[i].rstrip('\n').strip()

        # Check for closing brace
        if line.startswith('}'):
            i += 1
            break

        # Skip nested struct/union (inline anonymous)
        if re.match(r'^(struct|union)\s*\{', line):
            # Skip until matching close
            depth = 1
            i += 1
            while i < n and depth > 0:
                l2 = lines[i].strip()
                if '{' in l2:
                    depth += 1
                if '}' in l2:
                    depth -= 1
                i += 1
            continue

        # Parse field: type name; // offset 0xNN, size 0xNN
        # Also handles bitfields: type name : N; // offset 0xNN, size 0xNN
        field_m = re.match(
            r'^(.+?)\s+(\w+)(?:\s*\[\s*(\d+)\s*\])?\s*'
            r'(?::\s*\d+)?\s*;'
            r'\s*//\s*offset\s*(0x[0-9A-Fa-f]+|\d+)\s*,\s*size\s*(0x[0-9A-Fa-f]+|\d+)',
            line
        )
        if field_m:
            ftype = field_m.group(1).strip()
            fname = field_m.group(2)
            farray = field_m.group(3)
            foffset = field_m.group(4)
            fsize = field_m.group(5)

            # Handle array
            if farray:
                fname = fname + "[" + farray + "]"
            elif re.search(r'\[', line):
                # Array in the type/name - extract
                arr_m2 = re.search(r'(\w+)\s*(\[[^\]]*\](?:\[[^\]]*\])*)\s*;', line)
                if arr_m2:
                    fname = arr_m2.group(1) + arr_m2.group(2)

            fields.append((ftype, fname, foffset, fsize))
            i += 1
            continue

        # Bitfield without offset/size comment
        field_m2 = re.match(r'^(.+?)\s+(\w+)\s*:\s*(\d+)\s*;', line)
        if field_m2:
            # Skip bitfields that were already handled above
            i += 1
            continue

        i += 1

    return i, {'size': total_size, 'fields': fields}


def parse_enum_body(lines, start_i, n):
    """
    Parse an enum body. Returns (next_line_index, [(name, value), ...]).
    """
    values = []
    i = start_i + 1  # Skip opening line

    while i < n:
        line = lines[i].rstrip('\n').strip()

        if line.startswith('}'):
            i += 1
            break

        # Match: VALUE_NAME = number,
        val_m = re.match(r'(\w+)\s*=\s*(-?\d+|0x[0-9A-Fa-f]+)\s*,?\s*$', line)
        if val_m:
            vname = val_m.group(1)
            vval = val_m.group(2)
            values.append((vname, vval))

        i += 1

    return i, values


# ============================================================================
# Format output in nu2_types.h style
# ============================================================================

def format_struct(name, data, kind="struct"):
    """Format a struct/union definition in nu2_types.h style."""
    size_str = data.get('size', None)
    actual_kind = data.get('kind', kind)

    header = f"{actual_kind} {name} {{"
    if size_str:
        header += f"             /* {size_str} */"

    lines = [header]

    for ftype, fname, foffset, fsize in data['fields']:
        resolved = resolve_type(ftype)

        # Check if fname has array suffix
        fname_base = fname
        fname_array = ""
        arr_m = re.match(r'^(\w+)(\[.+\])$', fname)
        if arr_m:
            fname_base = arr_m.group(1)
            fname_array = arr_m.group(2)

        # Format the field
        field_decl = f"    {resolved} {fname_base}{fname_array};"
        comment = f"/* {foffset} */"

        # Pad for alignment
        pad_len = max(1, 38 - len(field_decl))
        field_line = f"{field_decl}{' ' * pad_len}{comment}"
        lines.append(field_line)

    lines.append("};")
    return "\n".join(lines)


def format_enum(name, values):
    """Format an enum definition in nu2_types.h style."""
    lines = [f"enum {name} {{"]

    # Sort by numeric value
    def sort_key(v):
        val_str = v[1]
        if val_str.startswith("0x") or val_str.startswith("0X"):
            return int(val_str, 16)
        elif val_str.startswith("-"):
            return int(val_str)
        else:
            return int(val_str)

    sorted_values = sorted(values, key=sort_key)

    for i, (vname, vval) in enumerate(sorted_values):
        comma = "," if i < len(sorted_values) - 1 else ""
        # Convert decimal to hex if it's large enough
        try:
            num_val = int(vval, 0)
            if num_val < 0:
                val_display = str(num_val)
            elif num_val > 9:
                val_display = f"0x{num_val:X}"
            else:
                val_display = str(num_val)
        except ValueError:
            val_display = vval
        lines.append(f"    {vname} = {val_display}{comma}")

    lines.append("};")
    return "\n".join(lines)


def format_typedef_struct(typedef_name, data, kind="struct"):
    """Format a typedef struct { ... } Name; in nu2_types.h style."""
    size_str = data.get('size', None)
    actual_kind = data.get('kind', kind)

    header = f"typedef {actual_kind} {{"
    if size_str:
        header += f"             /* {size_str} */"

    lines = [header]

    for ftype, fname, foffset, fsize in data['fields']:
        resolved = resolve_type(ftype)

        fname_base = fname
        fname_array = ""
        arr_m = re.match(r'^(\w+)(\[.+\])$', fname)
        if arr_m:
            fname_base = arr_m.group(1)
            fname_array = arr_m.group(2)

        field_decl = f"    {resolved} {fname_base}{fname_array};"
        comment = f"/* {foffset} */"
        pad_len = max(1, 38 - len(field_decl))
        field_line = f"{field_decl}{' ' * pad_len}{comment}"
        lines.append(field_line)

    lines.append(f"}} {typedef_name};")
    return "\n".join(lines)


def format_typedef_enum(typedef_name, values):
    """Format a typedef enum { ... } Name; in nu2_types.h style."""
    lines = [f"typedef enum {{"]

    def sort_key(v):
        val_str = v[1]
        if val_str.startswith("0x") or val_str.startswith("0X"):
            return int(val_str, 16)
        elif val_str.startswith("-"):
            return int(val_str)
        else:
            return int(val_str)

    sorted_values = sorted(values, key=sort_key)

    for i, (vname, vval) in enumerate(sorted_values):
        comma = "," if i < len(sorted_values) - 1 else ""
        try:
            num_val = int(vval, 0)
            if num_val < 0:
                val_display = str(num_val)
            elif num_val > 9:
                val_display = f"0x{num_val:X}"
            else:
                val_display = str(num_val)
        except ValueError:
            val_display = vval
        lines.append(f"    {vname} = {val_display}{comma}")

    lines.append(f"}} {typedef_name};")
    return "\n".join(lines)


# ============================================================================
# Categorization
# ============================================================================

CATEGORIES = {
    "Math Types": [
        "nuquat_s", "nucolour4_s",
    ],
    "Core Types": [
        "numemblk_s", "numcseek_e", "nudatfile_s", "numemfile_s",
        "BlockInfo", "nufpcomjmp_s", "nufpar_s", "nufparpos_s",
    ],
    "Scene Node Types (nuscene.h)": [
        "NUMESHNODE_s", "NUTRANSFORMNODE_s", "NUJOINTNODE_s",
        "NUSKINNODE_s", "NUREFERENCENODE_s", "NUSPLINENODE_s",
        "nunodeptr_s",
    ],
    "Collision Types": [
        "NUCOLLISIONSPHERE_s",
    ],
    "Cutscene Types (nucutscene.h)": [
        "NUCUTSCENECAMERA_s", "NUHINSTANCE_s", "NUCUTSCENEAUDIO_s",
        "NUCUTSCENE_s",
    ],
    "Trigger Types (nutrigger.h)": [
        "NUTRIGGERPRIMTYPES_e", "NUTRIGGERTYPE_s",
        "NUTRIGGERPRIM_s", "NUTRIGGER_s", "NUTRIGGERSYS_s",
        "instNUTRIGGER_s", "instNUTRIGGERSYS_s",
    ],
    "Camera FX / Cull Types": [
        "nucamfxmode_s", "nuculltype_s",
    ],
    "Pad Recording Types": [
        "nurecmode_e",
        # typedefs: NUPADREC, PadRecType
    ],
    "Game Cutscene Types (nugcutscene.h)": [
        "NUSTATEANIM_s", "NUGCUTCAMTGT_s", "NUGCUTCAM_s",
        "instNUGCUTCAM_s", "NUGCUTCAMSYS_s", "instNUGCUTCAMSYS_s",
        "NUGCUTLOCATOR_s", "instNUGCUTLOCATOR_s",
        "NUGCUTLOCATORTYPE_s", "NUGCUTLOCATORSYS_s",
        "instNUGCUTLOCATORSYS_s",
        "NUGCUTRIGID_s", "NUGCUTRIGIDSYS_s",
        "instNUGCUTRIGID_s", "instNUGCUTRIGIDSYS_s",
        "NUGCUTCHAR_s", "NUGCUTCHARSYS_s",
        "instNUGCUTCHAR_s", "instNUGCUTCHARSYS_s",
        "NUGCUTTRIGGER_s", "NUGCUTTRIGGERSYS_s",
        "instNUGCUTTRIGGER_s", "instNUGCUTTRIGGERSYS_s",
        "NUGCUTBBOX_s", "NUGCUTSCENE_s",
        "instNUGCUTSCENE_s", "NUGCUTLOCFNDATA_s",
    ],
    "Game Cutscene Anim Enums": [
        # typedefs: RIGIDANIMCURVES, CHARANIMCURVES, LOCATORANIMCURVES, LOCATORDIRECTION
    ],
    "Crate Types (crate.h)": [
        "crate_s", "crate_enum_e", "crate_type_s",
        # typedefs: CrateCube, CrateCubeGroup, CRATETYPEDATA
    ],
    "Debris / Particle Types (debris.h)": [
        "debrissphere_s", "debkeydatatype_s", "deb3_s",
        "debris_sound_types_e",
        # typedefs: DEB3INFO, DEB3EMIT, RBCLASS
        # typedefs: debinftype, DEBRIS_COLOUR_SLOT, DEBRIS_ALPHA_SLOT, etc.
    ],
    "Sound / Listener Types": [
        "listenereffects",
    ],
    "Game Types - Additional": [
        # typedefs: TrackLink, TubInfo, VehicleToggle, PLAYEREVENT,
        # PowerData, WheelData, VehicleData, HubData, LevelData, etc.
        # GameState, LevelState, HubState, LevelTime, Timer, SurfaceData
        # LevelSpline, LevelObject, GameDebris, AnimDebris, PROJECTILE
        # ScreenWumpa, NEWWUMPA, CursorMemory, Selector
        # GameSound, CutSceneCharacter, GameCutAnim, SPACEANIM, BUGAREA
        # TerrShapeType, AnimList, CharacterData
    ],
}

# Type categories for typedef-only types
TYPEDEF_CATEGORIES = {
    "Pad Recording Types": [
        "NUPADREC", "PadRecType",
    ],
    "Game Cutscene Anim Enums": [
        "RIGIDANIMCURVES", "CHARANIMCURVES", "LOCATORANIMCURVES",
        "LOCATORDIRECTION",
    ],
    "Special Attribute Enums": [
        "NUSPECIALATTR",
    ],
    "Crate Types (crate.h)": [
        "CrateCube", "CrateCubeGroup", "CRATETYPEDATA",
    ],
    "Debris / Particle Types (debris.h)": [
        "DEB3INFO", "DEB3EMIT", "RBCLASS",
        "debinftype", "uv1debdata", "uv1deb",
        "DEBRIS_COLOUR_SLOT", "DEBRIS_ALPHA_SLOT",
        "DEBRIS_WIDTH_SLOT", "DEBRIS_HEIGHT_SLOT",
        "DEBRIS_ROTATION_SLOT", "DEBRIS_JIBBER_X_SLOT",
        "DEBRIS_JIBBER_Y_SLOT", "DEBRIS_COLLISION_SPHERE_SLOT",
        "DEBRIS_SOUND",
    ],
    "Sound / Listener Types": [
        "GameSound",
    ],
    "Game Types - Additional": [
        "TrackLink", "TubInfo", "VehicleToggle", "PLAYEREVENT",
        "PowerData", "WheelData", "VehicleData",
        "SurfaceData", "TerrShapeType",
        "LevelSpline", "LevelObject", "GameDebris", "AnimDebris",
        "PROJECTILE", "ScreenWumpa",
        "CutSceneCharacter", "GameCutAnim", "BUGAREA",
        "SPACEANIM",
    ],
}


# ============================================================================
# Main
# ============================================================================

def main():
    print("=" * 70)
    print("DWARF Type Extractor for Nu2 Engine / Crash Bandicoot WoC")
    print("=" * 70)

    # 1. Get existing types
    existing = get_existing_types(NU2_TYPES_PATH)
    print(f"\nExisting types in nu2_types.h: {len(existing)}")

    # 2. Parse DWARF dump
    print(f"Parsing DWARF dump: {DWARF_PATH}")
    structs, enums, typedef_structs, typedef_enums = parse_dwarf_dump(DWARF_PATH)
    print(f"  Found {len(structs)} named struct/union definitions")
    print(f"  Found {len(enums)} named enum definitions")
    print(f"  Found {len(typedef_structs)} typedef struct/union definitions")
    print(f"  Found {len(typedef_enums)} typedef enum definitions")

    # 3. Filter to keep only Nu2/game types not already present
    new_structs = {}
    for name, data in structs.items():
        if name in existing:
            continue
        if should_skip_type(name):
            continue
        if data.get('kind') == 'struct' and not data['fields']:
            # Skip empty structs (forward declarations like NEWBUGGY)
            continue
        new_structs[name] = data

    new_enums = {}
    for name, values in enums.items():
        if name in existing:
            continue
        if should_skip_enum(name):
            continue
        if not values:
            continue
        new_enums[name] = values

    new_typedef_structs = {}
    for name, data in typedef_structs.items():
        if name in existing:
            continue
        if should_skip_type(name):
            continue
        # Also check if the typedef name matches a struct already present
        # e.g., "CharacterData" is already in nu2_types.h
        if not data['fields']:
            continue
        new_typedef_structs[name] = data

    new_typedef_enums = {}
    for name, values in typedef_enums.items():
        if name in existing:
            continue
        if should_skip_enum(name):
            continue
        if not values:
            continue
        new_typedef_enums[name] = values

    # De-duplicate: if a name appears in both named and typedef versions,
    # keep only the typedef version (which establishes the typedef name).
    # Also remove named struct versions when a typedef struct has the same name.
    dup_struct_names = set(new_structs.keys()) & set(new_typedef_structs.keys())
    for name in dup_struct_names:
        del new_structs[name]
    dup_enum_names = set(new_enums.keys()) & set(new_typedef_enums.keys())
    for name in dup_enum_names:
        del new_enums[name]

    print(f"\nNew types to add:")
    print(f"  {len(new_structs)} named struct/union")
    print(f"  {len(new_enums)} named enum")
    print(f"  {len(new_typedef_structs)} typedef struct/union")
    print(f"  {len(new_typedef_enums)} typedef enum")
    if dup_struct_names:
        print(f"  (removed {len(dup_struct_names)} duplicate struct/union: {sorted(dup_struct_names)})")
    if dup_enum_names:
        print(f"  (removed {len(dup_enum_names)} duplicate enum: {sorted(dup_enum_names)})")

    # 4. Generate output grouped by category
    output_sections = []

    # -- Named structs/unions/enums by category --
    categorized_structs = set()
    categorized_enums = set()
    categorized_tstructs = set()
    categorized_tenums = set()

    for cat_name, members in CATEGORIES.items():
        section_items = []
        for m in members:
            if m in new_structs:
                section_items.append(format_struct(m, new_structs[m]))
                categorized_structs.add(m)
            elif m in new_enums:
                section_items.append(format_enum(m, new_enums[m]))
                categorized_enums.add(m)

        # Also check typedef categories
        if cat_name in TYPEDEF_CATEGORIES:
            for m in TYPEDEF_CATEGORIES[cat_name]:
                if m in new_typedef_structs:
                    section_items.append(format_typedef_struct(m, new_typedef_structs[m]))
                    categorized_tstructs.add(m)
                elif m in new_typedef_enums:
                    section_items.append(format_typedef_enum(m, new_typedef_enums[m]))
                    categorized_tenums.add(m)

        if section_items:
            section_header = f"/* ========== {cat_name} ========== */"
            output_sections.append(section_header + "\n\n" + "\n\n".join(section_items))

    # Check for typedef categories not linked to a CATEGORIES entry
    for cat_name, members in TYPEDEF_CATEGORIES.items():
        if cat_name in CATEGORIES:
            continue  # Already handled
        section_items = []
        for m in members:
            if m in new_typedef_structs and m not in categorized_tstructs:
                section_items.append(format_typedef_struct(m, new_typedef_structs[m]))
                categorized_tstructs.add(m)
            elif m in new_typedef_enums and m not in categorized_tenums:
                section_items.append(format_typedef_enum(m, new_typedef_enums[m]))
                categorized_tenums.add(m)
        if section_items:
            section_header = f"/* ========== {cat_name} ========== */"
            output_sections.append(section_header + "\n\n" + "\n\n".join(section_items))

    # -- Uncategorized types --
    uncategorized_items = []
    for name in sorted(new_structs.keys()):
        if name not in categorized_structs:
            uncategorized_items.append(format_struct(name, new_structs[name]))
    for name in sorted(new_enums.keys()):
        if name not in categorized_enums:
            uncategorized_items.append(format_enum(name, new_enums[name]))
    for name in sorted(new_typedef_structs.keys()):
        if name not in categorized_tstructs:
            uncategorized_items.append(format_typedef_struct(name, new_typedef_structs[name]))
    for name in sorted(new_typedef_enums.keys()):
        if name not in categorized_tenums:
            uncategorized_items.append(format_typedef_enum(name, new_typedef_enums[name]))

    if uncategorized_items:
        section_header = "/* ========== Uncategorized Types ========== */"
        output_sections.append(section_header + "\n\n" + "\n\n".join(uncategorized_items))

    # 5. Output final text
    final_output = "\n\n".join(output_sections)

    print("\n" + "=" * 70)
    print("GENERATED TYPES TO APPEND TO nu2_types.h")
    print("=" * 70)
    print(final_output)
    print("\n" + "=" * 70)

    # Also write to a temp file for easy copy
    out_path = os.path.join(os.path.dirname(NU2_TYPES_PATH), "nu2_types_new.txt")
    with open(out_path, "w") as f:
        f.write(final_output)
    print(f"\nOutput also written to: {out_path}")

    # Print summary
    total = (len(new_structs) - len(categorized_structs) + len(categorized_structs) +
             len(new_enums) - len(categorized_enums) + len(categorized_enums) +
             len(new_typedef_structs) - len(categorized_tstructs) + len(categorized_tstructs) +
             len(new_typedef_enums) - len(categorized_tenums) + len(categorized_tenums))
    print(f"\nTotal new type definitions generated: {total}")

    return final_output


if __name__ == "__main__":
    main()
