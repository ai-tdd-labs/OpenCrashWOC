/**
 * Nu2 Engine Type Definitions for Ghidra Import
 *
 * Source: OpenCrashWOC (GC decompilation with DWARF debug info)
 * These structs are cross-platform and identical on PS2, GC, and Xbox.
 *
 * Import in Ghidra: File > Parse C Source > select this file
 */

/* ========== Base Types ========== */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef signed long long s64;
typedef float f32;
typedef double f64;
typedef unsigned short angle;
typedef signed int fxi;
typedef s32 fileHandle;
typedef void (*error_func)(char *msg, ...);

/* Forward declarations for opaque/callback types */
typedef void NUCUTSCENEFINISHEDCALLBACKFN;
typedef void NUGCUTLOCFN;
typedef void PARTICLEDEF;
typedef void CREATCMD;
typedef void def_fn4;
typedef void DSHashFunc;

/* Forward declarations for types defined externally or used before definition */

struct JEEPROCK { int data; };

struct SIMWHEEL { int data[64]; };

struct nuvec_s {              /* 0x0C */
    f32 x;                    /* 0x00 */
    f32 y;                    /* 0x04 */
    f32 z;                    /* 0x08 */
};

struct nuvec4_s {             /* 0x10 */
    f32 x;                    /* 0x00 */
    f32 y;                    /* 0x04 */
    f32 z;                    /* 0x08 */
    f32 w;                    /* 0x0C */
};

struct nuivec_s {             /* 0x0C */
    int x;                    /* 0x00 */
    int y;                    /* 0x04 */
    int z;                    /* 0x08 */
};

struct nuivec4_s {            /* 0x10 */
    int x;                    /* 0x00 */
    int y;                    /* 0x04 */
    int z;                    /* 0x08 */
    int w;                    /* 0x0C */
};

struct nuangvec_s {           /* 0x0C */
    int x;                    /* 0x00 */
    int y;                    /* 0x04 */
    int z;                    /* 0x08 */
};

struct numtx_s {              /* 0x40 - 4x4 matrix */
    f32 _00, _01, _02, _03;  /* row 0 */
    f32 _10, _11, _12, _13;  /* row 1 */
    f32 _20, _21, _22, _23;  /* row 2 */
    f32 _30, _31, _32, _33;  /* row 3 */
};

struct nuplane_s {            /* 0x10 */
    f32 a;                    /* 0x00 */
    f32 b;                    /* 0x04 */
    f32 c;                    /* 0x08 */
    f32 d;                    /* 0x0C */
};

struct nucolour3_s {          /* 0x0C */
    f32 r;                    /* 0x00 */
    f32 g;                    /* 0x04 */
    f32 b;                    /* 0x08 */
};

struct nunrand_s {            /* 0x04 */
    long idum;                /* 0x00 */
};

enum nufilemode_e {
    NUFILE_READ = 0,
    NUFILE_WRITE = 1,
    NUFILE_APPEND = 2,
    NUFILE_MODE_CNT = 3
};

enum nufileseek_e {
    NUFILE_SEEK_START = 0,
    NUFILE_SEEK_CURRENT = 1,
    NUFILE_SEEK_END = 2,
    NUFILE_SEEK_CNT = 3
};

struct nuiffhdr_s {           /* 0x08 */
    s32 blk;                  /* 0x00 */
    s32 size;                 /* 0x04 */
};

struct nudatinfo_s {          /* 0x10 */
    s32 foffset;              /* 0x00 */
    s32 flen;                 /* 0x04 */
    s32 uplen;                /* 0x08 */
    s32 ppack;                /* 0x0C - bitfield: 1 bit */
};

struct nudfnode_s {           /* 0x08 */
    short childix;            /* 0x00 */
    short sibix;              /* 0x02 */
    char *txt;                /* 0x04 */
};

struct nufpcomjmp_s {         /* 0x08 */
    char *fname;              /* 0x00 */
    void *func;               /* 0x04 */
};

enum gobjtype_s {
    NUGOBJ_MESH = 0,
    NUGOBJ_FACEON = 1
};

enum nutextype_e {
    NUTEX_RGB16 = 0,
    NUTEX_RGBA16 = 1,
    NUTEX_RGB24 = 2,
    NUTEX_RGBA32 = 3,
    NUTEX_PAL4 = 4,
    NUTEX_PAL8 = 5,
    NUTEX_PAL4_S = 6,
    NUTEX_PAL8_S = 7,
    NUTEX_DXT1 = 8,
    NUTEX_DXT2 = 9,
    NUTEX_DXT3 = 10,
    NUTEX_DXT4 = 11,
    NUTEX_DXT5 = 12,
    NUTEX_BB = 13,
    NUTEX_RT8 = 14,
    NUTEX_RTT8 = 15,
    NUTEX_RT8H = 16,
    NUTEX_RT32 = 17,
    NUTEX_RT24 = 18,
    NUTEX_RT16 = 19,
    NUTEX_RTT32 = 20,
    NUTEX_RTT24 = 21,
    NUTEX_RTT16 = 22,
    NUTEX_RZT32 = 23,
    NUTEX_RZT24 = 24,
    NUTEX_RZT16 = 25,
    NUTEX_NUMTEXTURETYPES = 26
};

enum nurndritemtype_s {
    NURNDRITEM_GEOM3D = 0,
    NURNDRITEM_GEOM2D = 1,
    NURNDRITEM_SKIN3D2 = 2,
    NURNDRITEM_GEOMFACE = 3
};

enum nuvtxtype_e {
    NUVT_PS = 0x11,
    NUVT_LC1 = 0x51,
    NUVT_TLTC1 = 0x53,
    NUVT_TC1 = 0x59,
    NUVT_SK3TC1 = 0x5d
};

enum nufaceontype_s {
    NUFACEON_FACEON = 0,
    NUFACEON_FACEY = 1
};

enum nuprimtype_e {
    NUPT_POINT = 0,
    NUPT_LINE = 1,
    NUPT_TRI = 2,
    NUPT_TRISTRIP = 3,
    NUPT_NDXLINE = 4,
    NUPT_NDXTRI = 5,
    NUPT_NDXTRISTRIP = 6,
    NUPT_BEZPATCH = 7,
    NUPT_BEZTRI = 8,
    NUPT_FACEON = 9,
    NUPT_QUADLIST = 10
};

struct numtlattrib_s {        /* 0x04 - bitfield */
    u32 bits;                 /* packed: alpha(2), filter(2), fx(2), utc(2), vtc(2),
                                 cull(2), zmode(2), lighting(2), colour(1), fill(1),
                                 atst(3), aref(8), afail(2), uvmode(1) */
};

union nufx_u {                /* 0x04 */
    void *voidptr;
    u8 u8val;
    s8 s8val;
    u16 u16val;
    s16 s16val;
    u32 u32val;
    s32 s32val;
    f32 f32val;
};

struct nuskin_s {             /* 0x18 */
    struct nuskin_s *next;    /* 0x00 */
    int vtxoffset;            /* 0x04 */
    int vtxcnt;               /* 0x08 */
    int mtxcnt;               /* 0x0C */
    int *mtxid;               /* 0x10 */
    float *weights;           /* 0x14 */
};

struct NUVTXSKININFO_s {      /* 0x10 */
    float wts[3];             /* 0x00 */
    u8 joint_ixs[3];          /* 0x0C */
    u8 pad;                   /* 0x0F */
};

struct NUNODE_s {             /* 0x20 */
    char *name;               /* 0x00 */
    u32 id;                   /* 0x04 */
    u32 type;                 /* 0x08 */
    int nchildren;            /* 0x0C */
    struct NUNODE_s *parent;  /* 0x10 */
    struct NUNODE_s *child;   /* 0x14 */
    struct NUNODE_s *next;    /* 0x18 */
    void *data;               /* 0x1C */
};

struct nuinstflags_s {        /* 0x04 - bitfield */
    u32 bits;                 /* visible:1, onscreen:1, visitest:1, isanimated:1 */
};

struct nuspecialflags {       /* 0x04 */
    u32 bits;                 /* ext:1, ext_vis:1, ext_onscreen:1 */
};

struct nugspline_s {          /* 0x0C */
    short len;                /* 0x00 */
    short ptsize;             /* 0x02 */
    char *name;               /* 0x04 */
    char *pts;                /* 0x08 */
};

struct NUJOINTANIM_s {        /* 0x34 */
    float rx;                 /* 0x00 */
    float ry;                 /* 0x04 */
    float rz;                 /* 0x08 */
    float tx;                 /* 0x0C */
    float ty;                 /* 0x10 */
    float tz;                 /* 0x14 */
    float sx;                 /* 0x18 */
    float sy;                 /* 0x1C */
    float sz;                 /* 0x20 */
    short max_rx;             /* 0x24 */
    short max_ry;             /* 0x26 */
    short max_rz;             /* 0x28 */
    short min_rx;             /* 0x2A */
    short min_ry;             /* 0x2C */
    short min_rz;             /* 0x2E */
    u8 joint_id;              /* 0x30 */
    u8 flags;                 /* 0x31 */
    u8 pad[2];                /* 0x32 */
};

struct nupad_s {              /* 0xF4 */
    void *padhandle;          /* 0x00 */
    char xinputs[0x18];       /* 0x04 */
    char old_xinputs[0x18];   /* 0x1C */
    char feedback[0x48];      /* 0x34 */
    s32 port;                 /* 0x7C */
    s32 slot;                 /* 0x80 */
    s32 stage;                /* 0x84 */
    s32 id;                   /* 0x88 */
    s32 ldx;                  /* 0x8C */
    s32 ldy;                  /* 0x90 */
    s32 rdx;                  /* 0x94 */
    s32 rdy;                  /* 0x98 */
    float l_nx;               /* 0x9C */
    float l_ny;               /* 0xA0 */
    float l_rx;               /* 0xA4 */
    float l_ry;               /* 0xA8 */
    float l_dx;               /* 0xAC */
    float l_dy;               /* 0xB0 */
    float r_nx;               /* 0xB4 */
    float r_ny;               /* 0xB8 */
    float r_rx;               /* 0xBC */
    float r_ry;               /* 0xC0 */
    float r_dx;               /* 0xC4 */
    float r_dy;               /* 0xC8 */
    u32 paddata;              /* 0xCC */
    u32 oldpaddata;           /* 0xD0 */
    u32 paddata_db;           /* 0xD4 */
    u32 pad_flags;            /* 0xD8 - bitfields */
    u32 padvalue;             /* 0xDC */
    u8 ok;                    /* 0xE0 */
    u8 read_id;               /* 0xE1 */
    u8 buttons_hi;            /* 0xE2 */
    u8 buttons_lo;            /* 0xE3 */
    u8 r_alg_x;              /* 0xE4 */
    u8 r_alg_y;              /* 0xE5 */
    u8 l_alg_x;              /* 0xE6 */
    u8 l_alg_y;              /* 0xE7 */
    u8 algpad[12];            /* 0xE8 */
};

struct _sceDmaTag {              /* 0x10 */
    u16 qwc;                     /* 0x00 */
    u8 mark;                     /* 0x02 */
    u8 id;                       /* 0x03 */
    struct _sceDmaTag *next;     /* 0x04 */
    u32 p[2];                    /* 0x08 */
};

struct giftag {                  /* 0x10 */
    s64 NLOOP_EOP_id_PRE_PRIM_FLG_NREG; /* 0x00 - bitfield, see DWARF */
    s64 REGS;                    /* 0x08 - 16x4-bit register fields */
};

struct filebuff_s {              /* 0x04 */
    void *unk;                   /* 0x00 */
};

struct nufparpos_s {             /* 0x14 */
    s32 line_num;                /* 0x00 */
    s32 line_pos;                /* 0x04 */
    s32 cpos;                    /* 0x08 */
    s32 buffstart;               /* 0x0C */
    s32 buffend;                 /* 0x10 */
};

enum NUANIMKEYTYPES_e {
    NUANIMKEYTYPE_NONE = 0,
    NUANIMKEYTYPE_BIG = 1,
    NUANIMKEYTYPE_SMALL = 2,
    NUANIMKEYTYPE_INTEGER = 3,
    NUANIMKEYTYPE_BOOLEAN = 4
};

enum nuanimcomponents_e {
    NUANIM_X_TRANSLATION = 0,
    NUANIM_Y_TRANSLATION = 1,
    NUANIM_Z_TRANSLATION = 2,
    NUANIM_X_ROTATION = 3,
    NUANIM_Y_ROTATION = 4,
    NUANIM_Z_ROTATION = 5,
    NUANIM_X_SCALE = 6,
    NUANIM_Y_SCALE = 7,
    NUANIM_Z_SCALE = 8,
    NUANIM_NUMMTXOPERATIONS = 9
};

enum nuanimmode_e {
    NUANIM_PLAYBACK_ONCE = 0,
    NUANIM_PLAYBACK_LOOP = 1,
    NUANIM_PLAYBACK_OSCILLATE = 2,
    NUANIM_NUMMODES = 3
};

struct nuanimkey_s {             /* 0x10 */
    f32 time;                    /* 0x00 */
    f32 dtime;                   /* 0x04 */
    f32 c;                       /* 0x08 */
    f32 d;                       /* 0x0C */
};

struct NUANIMKEYSMALL_s {        /* 0x04 */
    s16 val;                     /* 0x00 */
    char grad;                   /* 0x02 */
    char time;                   /* 0x03 */
};

struct NUANIMKEYINTEGER_s {      /* 0x08 */
    f32 val;                     /* 0x00 */
    f32 time;                    /* 0x04 */
};

struct NUANIMKEYBIG_s {          /* 0x10 */
    f32 time;                    /* 0x00 */
    f32 dtime;                   /* 0x04 */
    f32 val;                     /* 0x08 */
    f32 grad;                    /* 0x0C */
};

struct nuanimtime_s {            /* 0x14 */
    f32 time;                    /* 0x00 */
    f32 time_offset;             /* 0x04 */
    s32 chunk;                   /* 0x08 */
    u32 time_mask;               /* 0x0C */
    u32 time_byte;               /* 0x10 */
};

enum NUCOLLISIONTYPES {
    NUCOLLISIONSPHERETYPE = 1
};

enum nustencilmode_e {
    NUSTENCIL_NOSTENCIL = 0,
    NUSTENCIL_REPLACE_NODRAW = 1
};

struct matchingslot_s {          /* 0x08 */
    int batch;                   /* 0x00 */
    int slot;                    /* 0x04 */
};

struct rdata_s {                 /* 0x410 */
    int dmadata[2];              /* 0x000 */
    int unpackdata[2];           /* 0x008 */
    struct {
        float x;                 /* 0x00 */
        float y;                 /* 0x04 */
        float z;                 /* 0x08 */
        float time;              /* 0x0C */
        float mx;                /* 0x10 */
        float my;                /* 0x14 */
        float mz;                /* 0x18 */
        float etime;             /* 0x1C */
    } debris[32];                /* 0x010 */
};

struct texanimscripts_s {        /* 0x10 */
    char *path;                  /* 0x00 */
    int pad1;                    /* 0x04 */
    u64 levbits;                 /* 0x08 */
};

struct nutexanimprog_s {         /* 0x1BC */
    struct nutexanimprog_s *succ; /* 0x000 */
    struct nutexanimprog_s *prev; /* 0x004 */
    char name[32];               /* 0x008 */
    int on_sig[32];              /* 0x028 */
    int off_sig[32];             /* 0x0A8 */
    u32 on_mask;                 /* 0x128 */
    u32 off_mask;                /* 0x12C */
    short xdef_ids[32];          /* 0x130 */
    short xdef_addrs[32];        /* 0x170 */
    int xdef_cnt;                /* 0x1B0 */
    short eop;                   /* 0x1B4 */
    short dynalloc;              /* 0x1B6 - bitfield: 1 bit */
    short code[1];               /* 0x1B8 */
};

struct nuviewport_s {            /* 0x40 */
    u32 x;                       /* 0x00 */
    u32 y;                       /* 0x04 */
    u32 width;                   /* 0x08 */
    u32 height;                  /* 0x0C */
    float zmin;                  /* 0x10 */
    float zmax;                  /* 0x14 */
    float centre_x;              /* 0x18 */
    float centre_y;              /* 0x1C */
    float clipmin_x;             /* 0x20 */
    float clipmin_y;             /* 0x24 */
    float clipmax_x;             /* 0x28 */
    float clipmax_y;             /* 0x2C */
    float clip_w;                /* 0x30 */
    float clip_h;                /* 0x34 */
    float scis_w;                /* 0x38 */
    float scis_h;                /* 0x3C */
};

enum gs_texturetype_e {
    GS_TEX_RGB16 = 0,
    GS_TEX_RGBA16 = 1,
    GS_TEX_RGB24 = 2,
    GS_TEX_RGBA32 = 3,
    GS_TEX_PAL4 = 4,
    GS_TEX_PAL8 = 5,
    GS_TEX_PAL4_S = 6,
    GS_TEX_PAL8_S = 7,
    GS_TEX_DXT1 = 8,
    GS_TEX_DXT2 = 9,
    GS_TEX_DXT3 = 10,
    GS_TEX_DXT4 = 11,
    GS_TEX_DXT5 = 12,
    GS_TEX_BB = 13,
    GS_TEX_RT8 = 14,
    GS_TEX_RTT8 = 15,
    GS_TEX_RT8H = 16,
    GS_TEX_RT32 = 17,
    GS_TEX_RT24 = 18,
    GS_TEX_RT16 = 19,
    GS_TEX_RTT32 = 20,
    GS_TEX_RTT24 = 21,
    GS_TEX_RTT16 = 22,
    GS_TEX_RZT32 = 23,
    GS_TEX_RZT24 = 24,
    GS_TEX_RZT16 = 25,
    GS_TEX_NUMTEXTURETYPES = 26
};

struct _D3DCOLORVALUE {          /* 0x10 */
    float r;                     /* 0x00 */
    float g;                     /* 0x04 */
    float b;                     /* 0x08 */
    float a;                     /* 0x0C */
};

enum _D3DLIGHTTYPE {
    D3DLIGHT_POINT = 1,
    D3DLIGHT_SPOT = 2,
    D3DLIGHT_DIRECTIONAL = 3,
    D3DLIGHT_FORCE_DWORD = 0x7FFFFFFF
};

struct _D3DVECTOR {              /* 0x0C */
    f32 x;                       /* 0x00 */
    f32 y;                       /* 0x04 */
    f32 z;                       /* 0x08 */
};

struct _GS_VERTEX {              /* 0x24 */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float nx;                    /* 0x0C */
    float ny;                    /* 0x10 */
    float nz;                    /* 0x14 */
    u32 diffuse;                 /* 0x18 */
    float u;                     /* 0x1C */
    float v;                     /* 0x20 */
};

struct _GS_VERTEXSKIN {          /* 0x38 */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float weights[2];            /* 0x0C */
    float indexes[3];            /* 0x14 */
    float nx;                    /* 0x20 */
    float ny;                    /* 0x24 */
    float nz;                    /* 0x28 */
    int diffuse;                 /* 0x2C */
    float u;                     /* 0x30 */
    float v;                     /* 0x34 */
};

struct _GS_VERTEXNORM {          /* 0x18 */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float nx;                    /* 0x0C */
    float ny;                    /* 0x10 */
    float nz;                    /* 0x14 */
};

struct _GS_VERTEXTL {            /* 0x1C */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float rhw;                   /* 0x0C */
    int diffuse;                 /* 0x10 */
    float u;                     /* 0x14 */
    float v;                     /* 0x18 */
};

struct _GS_VERTEXPSTL {          /* 0x14 */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float w;                     /* 0x0C */
    int diffuse;                 /* 0x10 */
};

struct _GS_BUFFER {              /* 0x08 */
    u32 length;                  /* 0x00 */
    u32 type;                    /* 0x04 */
};

struct _GS_FrameBufferCopy {     /* 0x34 - PS2 version */
    char tobj[0x20];             /* 0x00 - platform-specific texobj */
    void *data;                  /* 0x20 */
    s32 top;                     /* 0x24 */
    s32 left;                    /* 0x28 */
    s32 width;                   /* 0x2C */
    s32 height;                  /* 0x30 */
};

struct _GS_VIEWPORT {            /* 0x18 */
    u64 X;                       /* 0x00 */
    u64 Y;                       /* 0x08 */
    u64 width;                   /* 0x10 - platform note: may differ on PS2 */
    u64 height;
    float MinZ;
    float MaxZ;
};

struct _GSMATRIX {               /* 0x40 */
    float _11;                   /* 0x00 */
    float _12;                   /* 0x04 */
    float _13;                   /* 0x08 */
    float _14;                   /* 0x0C */
    float _21;                   /* 0x10 */
    float _22;                   /* 0x14 */
    float _23;                   /* 0x18 */
    float _24;                   /* 0x1C */
    float _31;                   /* 0x20 */
    float _32;                   /* 0x24 */
    float _33;                   /* 0x28 */
    float _34;                   /* 0x2C */
    float _41;                   /* 0x30 */
    float _42;                   /* 0x34 */
    float _43;                   /* 0x38 */
    float _44;                   /* 0x3C */
};

struct _GS_VECTOR3 {             /* 0x0C */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
};

struct _GS_VECTOR4 {             /* 0x10 */
    float x;                     /* 0x00 */
    float y;                     /* 0x04 */
    float z;                     /* 0x08 */
    float w;                     /* 0x0C */
};

struct _GSMATRIX33 {             /* 0x30 */
    float _11;                   /* 0x00 */
    float _12;                   /* 0x04 */
    float _13;                   /* 0x08 */
    float _14;                   /* 0x0C */
    float _21;                   /* 0x10 */
    float _22;                   /* 0x14 */
    float _23;                   /* 0x18 */
    float _24;                   /* 0x1C */
    float _31;                   /* 0x20 */
    float _32;                   /* 0x24 */
    float _33;                   /* 0x28 */
    float _34;                   /* 0x2C */
};

typedef struct {                 /* 0x0C */
    short i;                     /* 0x00 */
    u8 flags;                    /* 0x02 */
    s8 action;                   /* 0x03 */
    float anim_time;             /* 0x04 */
    float scale;                 /* 0x08 */
} FONT3DOBJECT;

struct anim_s {                  /* 0x1C */
    float anim_time;             /* 0x00 */
    float blend_src_time;        /* 0x04 */
    float blend_dst_time;        /* 0x08 */
    short action;                /* 0x0C */
    short oldaction;             /* 0x0E */
    short newaction;             /* 0x10 */
    short blend_src_action;      /* 0x12 */
    short blend_dst_action;      /* 0x14 */
    short blend_frame;           /* 0x16 */
    short blend_frames;          /* 0x18 */
    char blend;                  /* 0x1A */
    u8 flags;                    /* 0x1B */
};

struct remember {                /* 0x02 */
    char x;                      /* 0x00 */
    char y;                      /* 0x01 */
};

struct hub_s {                   /* 0x04 */
    u8 flags;                    /* 0x00 */
    u8 crystals;                 /* 0x01 */
    char pad1;                   /* 0x02 */
    char pad2;                   /* 0x03 */
};

struct time_s {                  /* 0x08 */
    char name[4];                /* 0x00 */
    u32 itime;                   /* 0x04 */
};

struct GTimer {                  /* 0x14 */
    u32 frame;                   /* 0x00 */
    u32 itime;                   /* 0x04 */
    u32 isec;                    /* 0x08 */
    float ftime;                 /* 0x0C */
    float fsec;                  /* 0x10 */
};

struct hdata_s {                 /* 0x0C */
    s8 level[6];                 /* 0x00 */
    s8 i_spl[2];                 /* 0x06 */
    u8 barrier;                  /* 0x08 */
    u8 i_gdeb;                   /* 0x09 */
    short sfx;                   /* 0x0A */
};

struct pCHASE {                  /* 0x2C */
    char i;                      /* 0x00 */
    char i_last;                 /* 0x01 */
    char i_next;                 /* 0x02 */
    u8 cuboid;                   /* 0x03 */
    short character[6];          /* 0x04 */
    short action[6];             /* 0x10 */
    float scale[6];              /* 0x1C */
    float duration;              /* 0x34 - note: check alignment */
};

enum Actions_ {
    ACT_ATTACK = 0,
    ACT_ATTACK2 = 1,
    ACT_CRAWL = 2,
    ACT_CROUCHDOWN = 3,
    ACT_CROUCHIDLE = 4,
    ACT_CROUCHUP = 5,
    ACT_DIE = 6,
    ACT_DIE2 = 7,
    ACT_DOWN = 8,
    ACT_DROWN = 9,
    ACT_EXPLODE = 10,
    ACT_FALL = 11,
    ACT_FILL = 12,
    ACT_FLY = 13,
    ACT_GETUP = 14,
    ACT_HANG = 15,
    ACT_HOP = 16,
    ACT_IDLE = 17,
    ACT_IDLE00 = 18,
    ACT_IDLE01 = 19,
    ACT_IDLE02 = 20,
    ACT_IDLE03 = 21,
    ACT_IDLE04 = 22,
    ACT_IDLE05 = 23,
    ACT_IDLE06 = 24,
    ACT_IDLE07 = 25,
    ACT_IN = 26,
    ACT_JUMP = 27,
    ACT_JUMP2 = 28,
    ACT_LAND = 29,
    ACT_LAND2 = 30,
    ACT_LEFT = 31,
    ACT_LOSE = 32,
    ACT_OPEN = 33,
    ACT_OUT = 34,
    ACT_PULL = 35,
    ACT_PUSH = 36,
    ACT_RIGHT = 37,
    ACT_RUN = 38,
    ACT_RUN2 = 39,
    ACT_RUNNINGJUMP = 40,
    ACT_SHAKE = 41,
    ACT_SIT = 42,
    ACT_SKATE = 43,
    ACT_SKID = 44,
    ACT_SLAM = 45,
    ACT_SLEEP = 46,
    ACT_SLIDE = 47,
    ACT_SOMERSAULT = 48,
    ACT_SPENT = 49,
    ACT_SPIN = 50,
    ACT_SPIN2 = 51,
    ACT_SPRINT = 52,
    ACT_STARJUMP = 53,
    ACT_SUPERSLAM = 54,
    ACT_SWIM = 55,
    ACT_SWING = 56,
    ACT_TIPTOE = 57,
    ACT_TRAPPED = 58,
    ACT_TRAPPEDSHAKE = 59,
    ACT_TURN = 60,
    ACT_UP = 61,
    ACT_WADE = 62,
    ACT_WAIT = 63,
    ACT_WALK = 64,
    ACT_WIN = 65,
    ACT_FLYZOFFA = 66,
    MAXACTIONS = 67
};

struct PData {                   /* 0x0C */
    u8 character;                /* 0x00 */
    u8 font3d_letter;            /* 0x01 */
    char pad1;                   /* 0x02 */
    char pad2;                   /* 0x03 */
    char *name;                  /* 0x04 */
    int *description;            /* 0x08 */
};

struct MoveInfo {                /* 0x48 */
    float IDLESPEED;             /* 0x00 */
    float TIPTOESPEED;           /* 0x04 */
    float WALKSPEED;             /* 0x08 */
    float RUNSPEED;              /* 0x0C */
    float SPRINTSPEED;           /* 0x10 */
    float SLIDESPEED;            /* 0x14 */
    float CRAWLSPEED;            /* 0x18 */
    float DANGLESPEED;           /* 0x1C */
    float WADESPEED;             /* 0x20 */
    float JUMPHEIGHT;            /* 0x24 */
    float DANGLEGAP;             /* 0x28 */
    short JUMPFRAMES0;           /* 0x2C */
    short JUMPFRAMES1;           /* 0x2E */
    short JUMPFRAMES2;           /* 0x30 */
    short STARJUMPFRAMES;        /* 0x32 */
    short SOMERSAULTFRAMES;      /* 0x34 */
    short SPINFRAMES;            /* 0x36 */
    short SPINRESETFRAMES;       /* 0x38 */
    short SUPERSPINFRAMES;       /* 0x3A */
    short SUPERSPINWAITFRAMES;   /* 0x3C */
    short SLAMWAITFRAMES;        /* 0x3E */
    short SLIDEFRAMES;           /* 0x40 */
    short CROUCHINGFRAMES;       /* 0x42 */
    short JUMPLANDFRAMES;        /* 0x44 */
    short spad;                  /* 0x46 */
};

struct animlist {                /* 0x18 */
    char *file;                  /* 0x00 */
    short action;                /* 0x04 */
    u8 blend_in_frames;          /* 0x06 */
    u8 blend_out_frames;         /* 0x07 */
    f32 speed;                   /* 0x08 */
    u16 flags;                   /* 0x0C */
    char pad1;                   /* 0x0E */
    char pad2;                   /* 0x0F */
    u64 levbits;                 /* 0x10 */
};

union type_s_name {              /* 0x02 */
    u8 chrs;
    u16 all;
};

struct creatcmd_s {              /* 0x0C */
    int cmd;                     /* 0x00 */
    int i;                       /* 0x04 */
    float f;                     /* 0x08 */
};

struct NEWBUGGY {                /* 0x04 - placeholder, TODO */
    void *a;                     /* 0x00 */
};

struct rumble_s {                /* 0x04 */
    u8 buzz;                     /* 0x00 */
    u8 power;                    /* 0x01 */
    u8 frame;                    /* 0x02 */
    u8 frames;                   /* 0x03 */
};

struct nuquat_s {             /* 0x10 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    float w;                          /* 0xC */
};

struct nucolour4_s {             /* 0x10 */
    float r;                          /* 0x0 */
    float g;                          /* 0x4 */
    float b;                          /* 0x8 */
    float a;                          /* 0xC */
};

struct numemblk_s {             /* 0x10 */
    void * * free;                    /* 0x0 */
    int blksize;                      /* 0x4 */
    int blkcnt;                       /* 0x8 */
    int blkfree;                      /* 0xC */
};

enum numcseek_e {
    NUMC_SEEK_START = 0,
    NUMC_SEEK_CURRENT = 1,
    NUMC_SEEK_END = 2,
    NUMC_SEEK_CNT = 3
};

struct NUCUTSCENEAUDIO_s {             /* 0x8 */
    float start_time;                 /* 0x0 */
    char * wav_name;                  /* 0x4 */
};

enum NUTRIGGERPRIMTYPES_e {
    NUTRIGGERPRIMTYPE_NONE = 0,
    NUTRIGGERPRIMTYPE_SPHERE = 1,
    NUTRIGGERPRIMTYPE_CUBE = 2,
    NUTRIGGERPRIMTYPE_CYLINDER = 3
};

enum NUTRIGGERTYPE_s {
    NUTRIGGER_AUTO = 0,
    NUTRIGGER_PLAYER_CONTACT = 1,
    NUTRIGGER_PLAYER_WEAPON_CONTACT = 2
};

struct NUTRIGGERPRIM_s {             /* 0x8 */
    int type;                         /* 0x0 */
    void * data;                      /* 0x4 */
};

struct instNUTRIGGER_s {             /* 0x4 */
    short hitpoints;                  /* 0x0 */
    u8 enableflags;                   /* 0x2 */
    char flags;                       /* 0x3 */
};

enum nucamfxmode_s {
    NUCAMFX_NONE = 0,
    NUCAMFX_REFLECT = 1
};

enum nuculltype_s {
    NUCULL_BOUNDINGSPHERE = 0,
    NUCULL_BOUNDINGBOX = 1
};

enum nurecmode_e {
    NUPAD_NORM = 0,
    NUPAD_RECORD = 1,
    NUPAD_PLAY = 2
};

typedef struct {             /* 0x14 */
    u8 ok;                            /* 0x0 */
    u8 read_id;                       /* 0x1 */
    u8 buttons_hi;                    /* 0x2 */
    u8 buttons_lo;                    /* 0x3 */
    u8 r_alg_x;                       /* 0x4 */
    u8 r_alg_y;                       /* 0x5 */
    u8 l_alg_x;                       /* 0x6 */
    u8 l_alg_y;                       /* 0x7 */
    u8 l_algpad_r;                    /* 0x8 */
    u8 l_algpad_l;                    /* 0x9 */
    u8 l_algpad_u;                    /* 0xA */
    u8 l_algpad_d;                    /* 0xB */
    u8 r_algpad_u;                    /* 0xC */
    u8 r_algpad_r;                    /* 0xD */
    u8 r_algpad_d;                    /* 0xE */
    u8 r_algpad_l;                    /* 0xF */
    u8 l1_alg;                        /* 0x10 */
    u8 r1_alg;                        /* 0x11 */
    u8 l2_alg;                        /* 0x12 */
    u8 r2_alg;                        /* 0x13 */
} NUPADREC;

struct NUSTATEANIM_s {             /* 0xC */
    int nchanges;                     /* 0x0 */
    float * frames;                   /* 0x4 */
    u8 * states;                      /* 0x8 */
};

struct instNUGCUTCAM_s {             /* 0x4 */
    u8 flags;                         /* 0x0 */
    u8 tgt_ix;                        /* 0x1 */
    char pad[2];                      /* 0x2 */
};

struct instNUGCUTLOCATOR_s {             /* 0x8 */
    float timer;                      /* 0x0 */
    void * data;                      /* 0x4 */
};

struct NUGCUTLOCATORTYPE_s {             /* 0xC */
    char * name;                      /* 0x0 */
    u8 flags;                         /* 0x4 */
    u16 ix;                           /* 0x6 */
    char pad;                         /* 0x8 */
};

struct instNUGCUTCHAR_s {             /* 0x18 */
    void * character;                 /* 0x0 */
    float blend;                      /* 0x4 */
    float mtxblend;                   /* 0x8 */
    float frame1;                     /* 0xC */
    float frame2;                     /* 0x10 */
    char flags;                       /* 0x14 */
    u8 prev_animix;                   /* 0x15 */
    u8 current_animix;                /* 0x16 */
    u8 blendto_animix;                /* 0x17 */
};

struct instNUGCUTTRIGGER_s {             /* 0x4 */
    char next_ix;                     /* 0x0 */
    char pad[3];                      /* 0x1 */
};

struct NUGCUTLOCFNDATA_s {             /* 0x8 */
    char * name;                      /* 0x0 */
    NUGCUTLOCFN * fn;                 /* 0x4 */
};

typedef enum {
    RIGIDCURVE_X_TRANSLATION = 0,
    RIGIDCURVE_Y_TRANSLATION = 1,
    RIGIDCURVE_Z_TRANSLATION = 2,
    RIGIDCURVE_X_ROTATION = 3,
    RIGIDCURVE_Y_ROTATION = 4,
    RIGIDCURVE_Z_ROTATION = 5,
    RIGIDCURVE_X_SCALE = 6,
    RIGIDCURVE_Y_SCALE = 7,
    RIGIDCURVE_Z_SCALE = 8,
    NUM_RIGIDCURVES = 9
} RIGIDANIMCURVES;

typedef enum {
    CHARCURVE_X_TRANSLATION = 0,
    CHARCURVE_Y_TRANSLATION = 1,
    CHARCURVE_Z_TRANSLATION = 2,
    CHARCURVE_X_ROTATION = 3,
    CHARCURVE_Y_ROTATION = 4,
    CHARCURVE_Z_ROTATION = 5,
    CHARCURVE_VISIBLE = 6,
    CHARCURVE_ANIMIX = 7,
    CHARCURVE_BLENDFRAMES = 8,
    CHARCURVE_ANIMRATE = 9,
    NUM_CHARCURVES = 0xA
} CHARANIMCURVES;

typedef enum {
    LOCATORCURVE_X_TRANSLATION = 0,
    LOCATORCURVE_Y_TRANSLATION = 1,
    LOCATORCURVE_Z_TRANSLATION = 2,
    LOCATORCURVE_X_ROTATION = 3,
    LOCATORCURVE_Y_ROTATION = 4,
    LOCATORCURVE_Z_ROTATION = 5,
    LOCATORCURVE_RATE = 6,
    LOCATORCURVE_VISIBLE = 7,
    NUM_LOCATORCURVES = 8
} LOCATORANIMCURVES;

typedef enum {
    LOCATOR_NO_DIRECTION = 0,
    LOCATOR_UP = 1,
    LOCATOR_DOWN = 2,
    LOCATOR_X = 3,
    LOCATOR_NEGX = 4,
    LOCATOR_Y = 5,
    LOCATOR_NEGY = 6,
    LOCATOR_Z = 7,
    LOCATOR_NEGZ = 8
} LOCATORDIRECTION;

enum crate_enum_e {
    CRATE_OUTLINE = 0,
    CRATE_BASIC = 1,
    CRATE_EXTRALIFE = 2,
    CRATE_MASK = 3,
    CRATE_UPARROW = 4,
    CRATE_QUESTIONMARK = 5,
    CRATE_BOUNCY = 6,
    CRATE_CHECKPOINT = 7,
    CRATE_RANDOM = 8,
    CRATE_TNT = 9,
    CRATE_1 = 0xA,
    CRATE_2 = 0xB,
    CRATE_3 = 0xC,
    CRATE_METAL_UPARROW = 0xD,
    CRATE_METAL_EXCLAMATION = 0xE,
    CRATE_METAL = 0xF,
    CRATE_NITRO = 0x10,
    CRATE_NITRO_EXCLAMATION = 0x11,
    CRATE_MOTION_SENSOR = 0x12,
    CRATE_REINFORCED = 0x13,
    CRATE_INVISIBILITY = 0x14,
    CRATE_OPEN = 0x15,
    CRATE_TNT3 = 0x16,
    CRATE_TNT2 = 0x17,
    CRATE_TNT1 = 0x18,
    CRATE_COCO = 0x19,
    CRATE_MARKER = 0x1A,
    CRATE_LOCK = 0x1B,
    CRATE_HIGHLIGHT = 0x1C,
    MAXCRATETYPES = 0x1D
};

typedef struct {             /* 0x20 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    float time;                       /* 0xC */
    float mx;                         /* 0x10 */
    float my;                         /* 0x14 */
    float mz;                         /* 0x18 */
    float etime;                      /* 0x1C */
} uv1deb;

enum debris_sound_types_e {
    DEBRIS_SOUND_OFF = 0,
    DEBRIS_SOUND_ONEDGE = 1,
    DEBRIS_SOUND_OFFEDGE = 2,
    DEBRIS_SOUND_PERPART = 3,
    DEBRIS_SOUND_REGULAR = 4
};

typedef struct {             /* 0x20 */
    short type;                       /* 0x0 */
    short classid;                    /* 0x2 */
    short info;                       /* 0x4 */
    float timer;                      /* 0x8 */
    float size;                       /* 0xC */
    short deb;                        /* 0x10 */
    short rate;                       /* 0x12 */
    def_fn4 * impact;                 /* 0x14 */
    def_fn4 * end;                    /* 0x18 */
    int data;                         /* 0x1C */
} DEB3INFO;

typedef struct {             /* 0x24 */
    float yoff;                       /* 0x0 */
    float grav;                       /* 0x4 */
    float px;                         /* 0x8 */
    float py;                         /* 0xC */
    float pz;                         /* 0x10 */
    float vx;                         /* 0x14 */
    float vy;                         /* 0x18 */
    float vz;                         /* 0x1C */
    float ang;                        /* 0x20 */
} DEB3EMIT;

typedef struct {             /* 0x10 */
    float t;                          /* 0x0 */
    float r;                          /* 0x4 */
    float g;                          /* 0x8 */
    float b;                          /* 0xC */
} DEBRIS_COLOUR_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float a;                          /* 0x4 */
} DEBRIS_ALPHA_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float w;                          /* 0x4 */
} DEBRIS_WIDTH_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float h;                          /* 0x4 */
} DEBRIS_HEIGHT_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float r;                          /* 0x4 */
} DEBRIS_ROTATION_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float x;                          /* 0x4 */
} DEBRIS_JIBBER_X_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float y;                          /* 0x4 */
} DEBRIS_JIBBER_Y_SLOT;

typedef struct {             /* 0x8 */
    float t;                          /* 0x0 */
    float r;                          /* 0x4 */
} DEBRIS_COLLISION_SPHERE_SLOT;

typedef struct {             /* 0xC */
    int id;                           /* 0x0 */
    int type;                         /* 0x4 */
    int delay;                        /* 0x8 */
} DEBRIS_SOUND;

enum listenereffects {
    LISTENER_NORMAL = 0,
    LISTENER_CAVE = 1,
    LISTENER_METALROOM = 2,
    LISTENER_STONEROOM = 3,
    LISTENER_UNDERWATER = 4,
    LISTENER_LONGRANGEHEARING = 5,
    LISTENER_NULL = 6
};

typedef struct {             /* 0x6 */
    s8 i_left;                        /* 0x0 */
    s8 i_mid;                         /* 0x1 */
    s8 i_right;                       /* 0x2 */
    u8 start;                         /* 0x3 */
    u8 end;                           /* 0x4 */
    u8 flags;                         /* 0x5 */
} TrackLink;

typedef struct {             /* 0xC */
    u8 character;                     /* 0x0 */
    u8 font3d_letter;                 /* 0x1 */
    char pad1;                        /* 0x2 */
    char pad2;                        /* 0x3 */
    char * name;                      /* 0x4 */
    int * description;                /* 0x8 */
} PowerData;

typedef struct {             /* 0x8 */
    char * name;                      /* 0x0 */
    int flags;                        /* 0x4 */
} WheelData;

typedef struct {             /* 0x8 */
    float friction;                   /* 0x0 */
    short gdeb;                       /* 0x4 */
    u16 flags;                        /* 0x6 */
} SurfaceData;

typedef struct {             /* 0x10 */
    int i;                            /* 0x0 */
    char * name;                      /* 0x4 */
    u64 levbits;                      /* 0x8 */
} GameDebris;

typedef struct {             /* 0x30 */
    short character;                  /* 0x0 */
    short action;                     /* 0x2 */
    float time;                       /* 0x4 */
    float time2;                      /* 0x8 */
    u32 flags;                        /* 0xC */
    u16 locators;                     /* 0x10 */
    u8 gdeb;                          /* 0x12 */
    char count;                       /* 0x13 */
    u8 random;                        /* 0x14 */
    u8 xrot;                          /* 0x15 */
    s8 type;                          /* 0x16 */
    s8 target;                        /* 0x17 */
    short sfx;                        /* 0x18 */
    u8 rumble;                        /* 0x1A */
    u8 buzz;                          /* 0x1B */
    u64 levbits;                      /* 0x20 */
    float min;                        /* 0x28 */
    float max;                        /* 0x2C */
} AnimDebris;

typedef struct {             /* 0x8 */
    short character;                  /* 0x0 */
    short action;                     /* 0x2 */
    short sfx;                        /* 0x4 */
    s8 i;                             /* 0x6 */
    char pad1;                        /* 0x7 */
} GameCutAnim;

typedef struct {             /* 0x10 */
    char in_pad;                      /* 0x0 */
    s8 in_iRAIL;                      /* 0x1 */
    short in_iALONG;                  /* 0x2 */
    float in_fALONG;                  /* 0x4 */
    char out_pad;                     /* 0x8 */
    s8 out_iRAIL;                     /* 0x9 */
    short out_iALONG;                 /* 0xA */
    float out_fALONG;                 /* 0xC */
} BUGAREA;

typedef enum {
    NUSP_VISIBLE = 0,
    NUSP_RESETMTX = 1,
    NUSP_MTX = 2,
    NUSP_PREMTX = 3,
    NUSP_POSTMTX = 4,
    NUSP_RESETPOS = 5,
    NUSP_WPOS = 6,
    NUSP_WPOSR = 7,
    NUSP_LPOS = 8,
    NUSP_LPOSR = 9,
    NUSP_RESETROT = 0xA,
    NUSP_ROTX = 0xB,
    NUSP_ROTY = 0xC,
    NUSP_ROTZ = 0xD,
    NUSP_ROTXR = 0xE,
    NUSP_ROTYR = 0xF,
    NUSP_ROTZR = 0x10,
    NUSP_PROTXR = 0x11,
    NUSP_PROTYR = 0x12,
    NUSP_PROTZR = 0x13,
    NUSP_SCALE = 0x14,
    NUSP_SCALER = 0x15,
    NUSP_END = 0x16
} NUSPECIALATTR;

struct POINTANG {             /* 0x10 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    int Ang;                          /* 0xC */
};

struct STRUCT_TIMELOG {             /* 0x30 */
    s64 t_start;                      /* 0x0 */
    s64 t1;                           /* 0x8 */
    s64 t2;                           /* 0x10 */
    s64 t3;                           /* 0x18 */
    s64 t4;                           /* 0x20 */
    s64 t_end;                        /* 0x28 */
};

struct TERRAINFLAGS_s {             /* 0x4 */
    u32 rot;                          /* 0x0 */
};

struct XY {             /* 0x4 */
    u16 line;                         /* 0x0 */
    u8 count;                         /* 0x2 */
};

struct ZOFFASTART {             /* 0x10 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    float Angle;                      /* 0xC */
};

struct _GS_COLOURARGB {             /* 0x10 */
    float a;                          /* 0x0 */
    float r;                          /* 0x4 */
    float g;                          /* 0x8 */
    float b;                          /* 0xC */
};

struct _GS_POINT {             /* 0x8 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
};

struct _GS_VBLANKDATA {             /* 0xC */
    u32 VBlank;                       /* 0x0 */
    u32 Swap;                         /* 0x4 */
    u32 Flags;                        /* 0x8 */
};

struct _GS_VECTOR2 {             /* 0x8 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
};

struct _GS_VERTEXL {             /* 0x18 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    int diffuse;                      /* 0xC */
    float u;                          /* 0x10 */
    float v;                          /* 0x14 */
};

struct _GS_VERTEXPS {             /* 0x10 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    int diffuse;                      /* 0xC */
};

struct _PERFTIMER {             /* 0x14 */
    char * name;                      /* 0x0 */
    u32 start;                        /* 0x4 */
    u32 stop;                         /* 0x8 */
    u32 elapsed;                      /* 0xC */
    u32 average;                      /* 0x10 */
};

struct _REMAPTABLE {             /* 0x8 */
    char * name;                      /* 0x0 */
    u8 * remap;                       /* 0x4 */
};

struct cutscenedesc_s {             /* 0x10 */
    char * gscname;                   /* 0x0 */
    char * cutname;                   /* 0x4 */
    int sfxid;                        /* 0x8 */
    int debgroup;                     /* 0xC */
};

struct memblk_head_s {             /* 0x20 */
    struct memblk_head_s * succ;      /* 0x0 */
    struct memblk_head_s * prev;      /* 0x4 */
    int line;                         /* 0x8 */
    int cnt;                          /* 0xC */
    int size;                         /* 0x10 */
    char mark[12];                    /* 0x14 */
};

struct memblk_tail_s {             /* 0xC */
    char mark[12];                    /* 0x0 */
};

struct nusound_filename_info_s {             /* 0x30 */
    char Filename[32];                /* 0x0 */
    int Pitch;                        /* 0x20 */
    int LoopInfo;                     /* 0x24 */
    int Type;                         /* 0x28 */
    int ID;                           /* 0x2C */
};

struct platattrib {             /* 0x4 */
    u32 rotate;                       /* 0x0 */
    u32 hit;                          /* 0x0 */
};

struct tb_info {             /* 0x20C */
    long start[16];                   /* 0x80 */
    long end[16];                     /* 0xC0 */
    long peak[16];                    /* 0x100 */
    int col[16];                      /* 0x140 */
    int buf[16];                      /* 0x180 */
    char * txt[16];                   /* 0x1C0 */
    long timerfreq;                   /* 0x200 */
    long timerfreq2;                  /* 0x204 */
    long performance_counter;         /* 0x208 */
};

struct visbndassoc_s {             /* 0x10 */
    int visspline;                    /* 0x0 */
    int bndspline;                    /* 0x4 */
    int visstart_ix;                  /* 0x8 */
    int visend_ix;                    /* 0xC */
};

enum BIGGUNACTIONS {
    BIGGUN_IDLE = 0,
    BIGGUN_FIRE = 1,
    BIGGUN_DESTROYED = 2
};

enum BOLTMODE {
    BOLT_INACTIVE = 0,
    BOLT_HOME = 1,
    BOLT_FREE = 2
};

enum EB_ACTIONS {
    EB_ENTER = 0,
    EB_FORMA = 1,
    EB_FORMB = 2,
    EB_STAND = 3,
    EB_ROLL = 4,
    EB_WAITROLL = 5,
    EB_SLOW = 6,
    EB_BEENHIT = 7,
    EB_WAITBIGJUMP = 8,
    EB_BIGJUMP = 9,
    EB_WAITCAMERA = 0xA,
    EB_WHACKO = 0xB
};

enum FIREBOSSACTIONS {
    FBACT_INTRO = 0,
    FBACT_FRONTHURL = 1,
    FBACT_TIRED = 2,
    FBACT_RUNAWAY = 3,
    FBACT_GETANGRY = 4,
    FBACT_DEAD = 5
};

enum FIREFLYINTRO {
    FFI_TOWARDS = 0,
    FFI_MATCHSPEED = 1,
    FFI_PREPAN = 2,
    FFI_PAN = 3,
    FFI_DONE = 4,
    FFI_COMPLETED = 5
};

enum HAILMODE {
    HM_INACTIVE = 0,
    HM_FLYUP = 1,
    HM_WAIT = 2,
    HM_RAINDOWN = 3,
    HM_DONE = 4
};

enum PLANEACTIONS {
    PLANEACT_INCOMING = 0,
    PLANEACT_DROP = 1,
    PLANEACT_AWAY = 2,
    PLANEACT_DONE = 3
};

enum POINTNUMS {
    PN_FRONTLWHEEL = 0,
    PN_FRONTRWHEEL = 1,
    PN_REARLWHEEL = 2,
    PN_REARRWHEEL = 3,
    PN_ROTATEPOINT = 4,
    PN_MIDPOINT = 5,
    PN_NUMPOINTS = 6
};

enum WBACTIONS {
    WBACT_SEEK = 0,
    WBACT_ATTACKLEFT = 1,
    WBACT_ATTACKEYES = 2,
    WBACT_ATTACKRIGHT = 3,
    WBACT_ATTACKCHEST = 4,
    WBACT_DEAD = 5
};

enum ZOFFA_ACTIONS {
    ZOFFA_NOTACTIVE = 0,
    ZOFFA_PATROL = 1,
    ZOFFA_ATTACK = 2,
    ZOFFA_DODGE = 3
};

enum debris_chunk_control_e {
    DEBRIS_CHUNK_CONTROL_FREE = 0,
    DEBRIS_CHUNK_CONTROL_FREE_AND_UNLINK = 1,
    DEBRIS_CHUNK_CONTROL_SORT_DMA = 2,
    DEBRIS_CHUNK_CONTROL_RETURN_TO_STACK = 3,
    DEBRIS_CHUNK_CONTROL_CHANGE_COLOUR = 4,
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEY = 5,
    DEBRIS_CHUNK_CONTROL_DO_BOUNCEXZ = 6
};

enum fxidtypes_e {
    NOFX = 0,
    GLASSFX = 1,
    SPECULARFX = 2,
    SPECULARGLASSFX = 3,
    WATERFX = 4,
    DYNAMICWATERFX = 0x5C,
    BRDFFX = 0x5D,
    PROJTEXFX = 0x5E,
    LIGHTHAZEFX = 0x5F,
    DEPTHBLENDFX = 0x60,
    SNOWCLOUDFX = 0x61,
    CAUSTICSFX = 0x62,
    HAZEFX = 0x63,
    XRAYFX = 0x64,
    BUMPMAPFX = 0x87,
    FURFX = 0x88,
    ENVIRONMAPFX = 0x89
};

enum nuinputdevice_e {
    NU_KEYBOARD = 0,
    NU_MOUSE = 1,
    NU_JOYSTICK1 = 2,
    NU_JOYSTICK2 = 3,
    NU_JOYSTICK3 = 4,
    NU_JOYSTICK4 = 5,
    NU_MAXINPUTDEVICES = 6
};

enum nukey_e {
    NU_JOYB1 = 0,
    NU_MOUSEB1 = 0,
    NU_JOYB2 = 1,
    NU_MOUSEB2 = 1,
    NU_JOYB3 = 2,
    NU_MOUSEB3 = 2,
    NU_JOYB4 = 3,
    NU_MOUSEB4 = 3,
    NU_JOYB5 = 4,
    NU_MOUSEB5 = 4,
    NU_JOYB6 = 5,
    NU_MOUSEB6 = 5,
    NU_JOYB7 = 6,
    NU_MOUSEB7 = 6,
    NU_JOYB8 = 7,
    NU_MOUSEB8 = 7,
    NU_JOYB9 = 8,
    NU_MOUSEX = 8,
    NU_JOYB10 = 9,
    NU_MOUSEY = 9,
    NU_JOYX = 0xA,
    NU_MOUSEZ = 0xA,
    NU_JOYY = 0xB,
    NU_JOYZ = 0xC
};

enum nukeystate_e {
    NU_KEYDOWN = 1,
    NU_KEYUP = 2,
    NU_KEYCHANGED = 4,
    NU_KEYCLEAR = 8
};

enum object_trigger_types_e {
    OBJECT_TRIGGER_NONE = 0,
    OBJECT_TRIGGER_SWITCH = 1,
    OBJECT_TRIGGER_SWITCH_ONE_CYCLE = 2,
    OBJECT_TRIGGER_SWITCH_CONTINUOUS = 3,
    OBJECT_TRIGGER_PROXIMITY = 4,
    OBJECT_TRIGGER_PROXIMITY_ONE_CYCLE = 5,
    OBJECT_TRIGGER_PROXIMITY_CONTINUOUS = 6,
    OBJECT_TRIGGER_TERRAIN = 7,
    OBJECT_TRIGGER_TERRAIN_ONE_CYCLE = 8,
    OBJECT_TRIGGER_TERRAIN_CONTINUOUS = 9,
    OBJECT_TRIGGER_OVERRIDE_NOANIM = 0xA,
    OBJECT_TRIGGER_OVERRIDE_PLAY = 0xB,
    OBJECT_TRIGGER_OVERRIDE_PLAY_CONTINUOUS = 0xC
};

enum terr_type {
    TERR_TYPE_EMPTY = -1,
    TERR_TYPE_NORMAL = 0,
    TERR_TYPE_PLATFORM = 1,
    TERR_TYPE_WALLSPL = 2,
    TERR_TYPE_CRASHDATA = 3
};

typedef struct {             /* 0x1C */
    short character;                  /* 0x0 */
    short points;                     /* 0x2 */
    CREATCMD * cmd;                   /* 0x4 */
    char name[16];                    /* 0x8 */
    float delay;                      /* 0x18 */
} AITYPE;

typedef struct {             /* 0x18 */
    char * file;                      /* 0x0 */
    short action;                     /* 0x4 */
    u8 blend_in_frames;               /* 0x6 */
    u8 blend_out_frames;              /* 0x7 */
    float speed;                      /* 0x8 */
    u16 flags;                        /* 0xC */
    char pad1;                        /* 0xE */
    char pad2;                        /* 0xF */
    u64 levbits;                      /* 0x10 */
} AnimList;

typedef struct {             /* 0x1C */
    float anim_time;                  /* 0x0 */
    float blend_src_time;             /* 0x4 */
    float blend_dst_time;             /* 0x8 */
    short action;                     /* 0xC */
    short oldaction;                  /* 0xE */
    short newaction;                  /* 0x10 */
    short blend_src_action;           /* 0x12 */
    short blend_dst_action;           /* 0x14 */
    short blend_frame;                /* 0x16 */
    short blend_frames;               /* 0x18 */
    s8 blend;                         /* 0x1A */
    u8 flags;                         /* 0x1B */
} AnimPacket;

typedef struct {             /* 0x38 */
    s8 i;                             /* 0x0 */
    s8 i_last;                        /* 0x1 */
    s8 i_next;                        /* 0x2 */
    u8 cuboid;                        /* 0x3 */
    short character[6];               /* 0x4 */
    short action[6];                  /* 0x10 */
    float scale[6];                   /* 0x1C */
    float duration;                   /* 0x34 */
} ChaseData;

typedef struct {             /* 0x8 */
    char * txt;                       /* 0x0 */
    short colour;                     /* 0x4 */
    short size;                       /* 0x6 */
} CreditEntry;

typedef struct {             /* 0x2 */
    s8 x;                             /* 0x0 */
    s8 y;                             /* 0x1 */
} CursorMemory;

typedef struct {             /* 0x10 */
    char * Prev;                      /* 0x0 */
    char * Next;                      /* 0x4 */
    char * Parent;                    /* 0x8 */
    char * Children;                  /* 0xC */
} DSBranch;

typedef struct {             /* 0x8 */
    char * Prev;                      /* 0x0 */
    char * Next;                      /* 0x4 */
} DSLink;

typedef struct {             /* 0xC */
    u32 Offset;                       /* 0x0 */
    char * Head;                      /* 0x4 */
    char * Tail;                      /* 0x8 */
} DSList;

typedef struct {             /* 0x8 */
    u32 Offset;                       /* 0x0 */
    char * Root;                      /* 0x4 */
} DSTree;

typedef struct {             /* 0xC */
    s8 level[6];                      /* 0x0 */
    s8 i_spl[2];                      /* 0x6 */
    u8 barrier;                       /* 0x8 */
    u8 i_gdeb;                        /* 0x9 */
    short sfx;                        /* 0xA */
} HubData;

typedef struct {             /* 0x4 */
    u8 flags;                         /* 0x0 */
    u8 crystals;                      /* 0x1 */
    char pad1;                        /* 0x2 */
    char pad2;                        /* 0x3 */
} HubState;

typedef struct {             /* 0x8 */
    char name[4];                     /* 0x0 */
    u32 itime;                        /* 0x4 */
} LevelTime;

typedef struct {             /* 0xC */
    u32 numMtx;                       /* 0x0 */
} MtxStack;

typedef struct {             /* 0xC */
    int i_objtab;                     /* 0x0 */
    float duration;                   /* 0x4 */
    float gravity;                    /* 0x8 */
} PanelDebrisInfo;

typedef struct {             /* 0x8 */
    short count;                      /* 0x0 */
    short draw;                       /* 0x2 */
    s8 frame;                         /* 0x4 */
    s8 wait;                          /* 0x5 */
    u8 delay;                         /* 0x6 */
    u8 item;                          /* 0x7 */
} PanelItem;

typedef struct {             /* 0x10 */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    float w;                          /* 0xC */
} Quaternion;

typedef struct {             /* 0x4 */
    u8 buzz;                          /* 0x0 */
    u8 power;                         /* 0x1 */
    u8 frame;                         /* 0x2 */
    u8 frames;                        /* 0x3 */
} RumblePacket;

typedef struct {             /* 0x6 */
    short x;                          /* 0x0 */
    short y;                          /* 0x2 */
    short z;                          /* 0x4 */
} S16Vec;

typedef struct {             /* 0xC */
    void * ptrid;                     /* 0x0 */
    short platid;                     /* 0x4 */
    short platinf;                    /* 0x6 */
    short timer;                      /* 0x8 */
    short pad;                        /* 0xA */
} TERRINFO;

typedef struct {             /* 0x10 */
    char * path;                      /* 0x0 */
    int pad1;                         /* 0x4 */
    u64 levbits;                      /* 0x8 */
} TexAnimList;

typedef struct {             /* 0x14 */
    u32 frame;                        /* 0x0 */
    u32 itime;                        /* 0x4 */
    u32 isec;                         /* 0x8 */
    float ftime;                      /* 0xC */
    float fsec;                       /* 0x10 */
} Timer;

typedef struct {             /* 0xC */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
} Vec;

typedef struct {             /* 0x14 */
    short tabindex;                   /* 0x0 */
    short count;                      /* 0x2 */
    float minx;                       /* 0x4 */
    float minz;                       /* 0x8 */
    float maxx;                       /* 0xC */
    float maxz;                       /* 0x10 */
} terrgrouptype;

typedef enum {
    SAVELOAD_STATUS_NONE = 0,
    SAVELOAD_STATUS_READY = 1,
    SAVELOAD_STATUS_QUICKCHECK = 2,
    SAVELOAD_STATUS_FULLCHECK = 3,
    SAVELOAD_STATUS_FORMATTING = 4,
    SAVELOAD_STATUS_READ01 = 5,
    SAVELOAD_STATUS_READ02 = 6,
    SAVELOAD_STATUS_READ03 = 7,
    SAVELOAD_STATUS_READ04 = 8,
    SAVELOAD_STATUS_WRITE01 = 9,
    SAVELOAD_STATUS_WRITE02 = 0xA,
    SAVELOAD_STATUS_WRITE03 = 0xB,
    SAVELOAD_STATUS_WRITE04 = 0xC,
    SAVELOAD_STATUS_WRITE05 = 0xD,
    SAVELOAD_STATUS_WRITEDIR01 = 0xE,
    SAVELOAD_STATUS_WRITEDIR02 = 0xF,
    SAVELOAD_STATUS_WRITEDIR03 = 0x10,
    SAVELOAD_STATUS_WRITEDIR04 = 0x11,
    SAVELOAD_STATUS_WRITEDIR04a = 0x12,
    SAVELOAD_STATUS_WRITEDIR04b = 0x13,
    SAVELOAD_STATUS_WRITEDIR04c = 0x14,
    SAVELOAD_STATUS_WRITEDIR05 = 0x15,
    SAVELOAD_STATUS_WRITEDIR06 = 0x16,
    SAVELOAD_STATUS_WRITEDIR07 = 0x17,
    SAVELOAD_STATUS_WRITEDIR08 = 0x18,
    SAVELOAD_STATUS_DELETE01 = 0x19,
    SAVELOAD_STATUS_DELETE02 = 0x1A,
    SAVELOAD_STATUS_DELETE03 = 0x1B,
    SAVELOAD_STATUS_DELETE04 = 0x1C,
    SAVELOAD_STATUS_DELETE05 = 0x1D,
    SAVELOAD_STATUS_DELETE06 = 0x1E,
    SAVELOAD_STATUS_DELETE07 = 0x1F,
    SAVELOAD_STATUS_DELETE08 = 0x20
} saveload_status_enum;

typedef struct {             /* 0x80 */
    u8 ai_type;                       /* 0x0 */
    s8 status;                        /* 0x1 */
    char pad1;                        /* 0x2 */
    char pad2;                        /* 0x3 */
    char pad3;                        /* 0x4 */
    s8 iRAIL;                         /* 0x5 */
    short iALONG;                     /* 0x6 */
    float fALONG;                     /* 0x8 */
    float time;                       /* 0xC */
    float delay;                      /* 0x10 */
    struct nuvec_s pos[8];            /* 0x14 */
    struct nuvec_s origin;            /* 0x74 */
} LevelAI;

typedef struct {             /* 0x24 */
    s8 iRAIL;                         /* 0x0 */
    s8 vertical;                      /* 0x1 */
    short iALONG;                     /* 0x2 */
    short i1;                         /* 0x4 */
    short i2;                         /* 0x6 */
    float fALONG;                     /* 0x8 */
    float fACROSS;                    /* 0xC */
    u16 angle;                        /* 0x10 */
    u16 cam_angle;                    /* 0x12 */
    u8 mode;                          /* 0x14 */
    char pad1;                        /* 0x15 */
    char pad2;                        /* 0x16 */
    char pad3;                        /* 0x17 */
    struct nuvec_s pos;               /* 0x18 */
} RailPosition;

typedef struct {             /* 0x14 */
    struct nuvec_s pos;               /* 0x0 */
    float time;                       /* 0xC */
    int type;                         /* 0x10 */
} FIREDROP;

struct nuvtx_ps_s {              /* 0x10 */
    struct nuvec_s pnt;          /* 0x00 */
    int diffuse;                 /* 0x0C */
};

struct RPos_s {                  /* 0x24 */
    char iRAIL;                  /* 0x00 */
    char vertical;               /* 0x01 */
    short iALONG;                /* 0x02 */
    short i1;                    /* 0x04 */
    short i2;                    /* 0x06 */
    float fALONG;                /* 0x08 */
    float fACROSS;               /* 0x0C */
    u16 angle;                   /* 0x10 */
    u16 cam_angle;               /* 0x12 */
    u8 mode;                     /* 0x14 */
    char pad1;                   /* 0x15 */
    char pad2;                   /* 0x16 */
    char pad3;                   /* 0x17 */
    struct nuvec_s pos;          /* 0x18 */
};

typedef struct {             /* 0x20 */
    struct nuvec_s pos1;              /* 0x0 */
    struct nuvec_s pos2;              /* 0xC */
    int intensity;                    /* 0x18 */
    int RealIntensity;                /* 0x1C */
} TRAIL;

struct VEHICLE {             /* 0x100 */
    struct nuvec_s ActualWheelPosition[4]; /* 0x0 */
    struct nuvec_s OldWheelPosition[4]; /* 0x30 */
    int BigSpin[4];                   /* 0x60 */
    struct nuvec_s ActualPosition;    /* 0x70 */
    struct nuvec_s Resolved;          /* 0x7C */
    struct nuvec_s Velocity;          /* 0x88 */
    struct nuvec_s WheelAxis[3];      /* 0x94 */
    float FrontWheelSpeedAdj;         /* 0xB8 */
    short aTargetAngle;               /* 0xBC */
    short aTarSurfRotX;               /* 0xBE */
    short aTarSurfRotZ;               /* 0xC0 */
    short aActualAngle;               /* 0xC2 */
    short aActSurfRotX;               /* 0xC4 */
    short aActSurfRotZ;               /* 0xC6 */
    short ActFrontRotX;               /* 0xC8 */
    short ActRearRotX;                /* 0xCA */
    short TarFrontRotX;               /* 0xCC */
    short TarRearRotX;                /* 0xCE */
    int AnyOnGroundBits;              /* 0xD0 */
    int AllOnGroundBits;              /* 0xD4 */
    int AllTouchingGroundBits;        /* 0xD8 */
    int AnyTouchingGroundBits;        /* 0xDC */
    struct nuvec_s AirNormal;         /* 0xE0 */
    struct nuvec_s SurfaceNormal;     /* 0xEC */
    short * TerrHandle;               /* 0xF8 */
    int FrontWheelGroundBits;         /* 0xFC */
};

struct newwumpa_s {              /* 0x24 */
    struct nuvec_s world_pos;    /* 0x00 */
    struct nuvec_s screen_pos;   /* 0x0C */
    float screen_scale;          /* 0x18 */
    u8 count;                    /* 0x1C */
    u8 delay;                    /* 0x1D */
    s8 transformed;              /* 0x1E */
    s8 bonus;                    /* 0x1F */
    s8 active;                   /* 0x20 */
    char pad1;                   /* 0x21 */
    char pad2;                   /* 0x22 */
    char pad3;                   /* 0x23 */
};

struct nufaceon_s {           /* 0x18 */
    struct nuvec_s point;     /* 0x00 */
    float width;              /* 0x0C */
    float height;             /* 0x10 */
    int colour;               /* 0x14 */
};

typedef struct {             /* 0x20 */
    struct nuvec_s pos;               /* 0x0 */
    float timer;                      /* 0xC */
    float xs;                         /* 0x10 */
    float ys;                         /* 0x14 */
    float scale;                      /* 0x18 */
    s8 bonus;                         /* 0x1C */
    char pad1;                        /* 0x1D */
    char pad2;                        /* 0x1E */
    char pad3;                        /* 0x1F */
} ScreenWumpa;

struct NUELLIPSOID_s {        /* 0x30 */
    struct nuvec_s centre;    /* 0x00 */
    struct nuvec_s y_axis;    /* 0x0C */
    struct nuvec_s x_axis;    /* 0x18 */
    struct nuvec_s z_axis;    /* 0x24 */
};

struct edpp_ptldesc_s {             /* 0x4C */
    struct nuvec_s pos;               /* 0x0 */
    int type;                         /* 0xC */
    int handle;                       /* 0x10 */
    short rotz;                       /* 0x14 */
    short roty;                       /* 0x16 */
    short emitrotz;                   /* 0x18 */
    short emitroty;                   /* 0x1A */
    int offset;                       /* 0x1C */
    char name[16];                    /* 0x20 */
    int trigger_type;                 /* 0x30 */
    int trigger_id;                   /* 0x34 */
    float trigger_var;                /* 0x38 */
    short refrotz;                    /* 0x3C */
    short refroty;                    /* 0x3E */
    float refoff;                     /* 0x40 */
    float refbounce;                  /* 0x44 */
    short group_id;                   /* 0x48 */
};

struct object_path_s {             /* 0x3EC */
    int objid;                        /* 0x0 */
    float speed;                      /* 0x4 */
    int oscillate;                    /* 0x8 */
    int repeat;                       /* 0xC */
    float pause;                      /* 0x10 */
    int usedway;                      /* 0x14 */
    int usedtime;                     /* 0x18 */
    int usedpart;                     /* 0x1C */
    int usedsound;                    /* 0x20 */
    int start_offset;                 /* 0x24 */
    int terrplatid;                   /* 0x28 */
    struct nuvec_s waypoint[8];       /* 0x2C */
    float waypoint_speed[8];          /* 0x8C */
    struct nuvec_s waypoint_rot[8];   /* 0xAC */
    int waypoint_time[8];             /* 0x10C */
    int trigger_type;                 /* 0x12C */
    int trigger_id;                   /* 0x130 */
    float trigger_var;                /* 0x134 */
    float trigger_wait;               /* 0x138 */
    int particle_type[8];             /* 0x1BC */
    int particle_rate[8];             /* 0x1DC */
    int particle_switch[8];           /* 0x1FC */
    struct nuvec_s particle_offset[8]; /* 0x21C */
    short particle_emitrotz[8];       /* 0x27C */
    short particle_emitroty[8];       /* 0x28C */
    float sound_last_time;            /* 0x29C */
    int sound_id[8];                  /* 0x320 */
    int sound_type[8];                /* 0x340 */
    float sound_time[8];              /* 0x360 */
    struct nuvec_s sound_offset[8];   /* 0x380 */
    float playergrav;                 /* 0x3E0 */
    float tension;                    /* 0x3E4 */
    float damping;                    /* 0x3E8 */
};

struct nuvtx_lc1_s {             /* 0x18 */
    struct nuvec_s pnt;          /* 0x00 */
    int diffuse;                 /* 0x0C */
    float tc[2];                 /* 0x10 */
};

struct edgra_clump_s {             /* 0x24 */
    int id;                           /* 0x0 */
    int num;                          /* 0x4 */
    struct nuvec_s centre;            /* 0x8 */
    float radius;                     /* 0x14 */
    float wind;                       /* 0x18 */
    int reactive;                     /* 0x1C */
    float height;                     /* 0x20 */
};

struct nuvtx_tltc1_s {           /* 0x1C */
    struct nuvec_s pnt;          /* 0x00 */
    float rhw;                   /* 0x0C */
    int diffuse;                 /* 0x10 */
    float tc[2];                 /* 0x14 */
};

struct particle_s {             /* 0x3C */
    struct particle_s * succ;         /* 0x0 */
    struct particle_s * prev;         /* 0x4 */
    struct nuvec_s pos;               /* 0x8 */
    struct nuvec_s vel;               /* 0x14 */
    struct nuvec_s acc;               /* 0x20 */
    short ang;                        /* 0x2C */
    short angv;                       /* 0x2E */
    int life;                         /* 0x30 */
    short handle;                     /* 0x34 */
    PARTICLEDEF * pd;                 /* 0x38 */
};

struct award_s {                 /* 0x30 */
    float time;                  /* 0x00 */
    u16 yrot;                    /* 0x04 */
    u16 got;                     /* 0x06 */
    s8 level;                    /* 0x08 */
    s8 wait;                     /* 0x09 */
    char pad1;                   /* 0x0A */
    char pad2;                   /* 0x0B */
    struct nuvec_s oldpos0;      /* 0x0C */
    struct nuvec_s oldpos1;      /* 0x18 */
    struct nuvec_s newpos;       /* 0x24 */
};

typedef struct {             /* 0xC004 */
    u16 count;                        /* 0x0 */
    u16 val;                          /* 0x2 */
    struct nuvec_s spl[4096];         /* 0x4 */
} WallSpline;

struct ShadPolDat {              /* 0x18 */
    struct nuvec_s pos;          /* 0x00 */
    float size;                  /* 0x0C */
    short shade;                 /* 0x10 */
    short xrot;                  /* 0x12 */
    short yrot;                  /* 0x14 */
    short zrot;                  /* 0x16 */
};

struct NUGCUTCAMTGT_s {             /* 0x10 */
    struct nuvec_s * tgt;             /* 0x0 */
    float frame;                      /* 0x4 */
    float nframes;                    /* 0x8 */
    char camid;                       /* 0xC */
    char pad[3];                      /* 0xD */
};

typedef struct {             /* 0x40 */
    struct nuvec_s start;             /* 0x0 */
    struct nuvec_s target;            /* 0xC */
    struct nuvec_s lasdir;            /* 0x18 */
    float sizew;                      /* 0x24 */
    float sizel;                      /* 0x28 */
    float sizewob;                    /* 0x2C */
    float arcsize;                    /* 0x30 */
    u8 type;                          /* 0x34 */
    u8 p1;                            /* 0x35 */
    u8 p2;                            /* 0x36 */
    u8 p3;                            /* 0x37 */
    int col;                          /* 0x38 */
    int seed;                         /* 0x3C */
} NuLgtArcLaserDataType;

typedef struct {             /* 0x30 */
    float time;                       /* 0x0 */
    u16 yrot;                         /* 0x4 */
    u16 got;                          /* 0x6 */
    s8 level;                         /* 0x8 */
    s8 wait;                          /* 0x9 */
    char pad1;                        /* 0xA */
    char pad2;                        /* 0xB */
    struct nuvec_s oldpos0;           /* 0xC */
    struct nuvec_s oldpos1;           /* 0x18 */
    struct nuvec_s newpos;            /* 0x24 */
} AWARD;

struct nuvtx_tc1_s {             /* 0x24 */
    struct nuvec_s pnt;          /* 0x00 */
    struct nuvec_s nrm;          /* 0x0C */
    int diffuse;                 /* 0x18 */
    float tc[2];                 /* 0x1C */
};

typedef struct {             /* 0x24 */
    struct nuvec_s world_pos;         /* 0x0 */
    struct nuvec_s screen_pos;        /* 0xC */
    float screen_scale;               /* 0x18 */
    u8 count;                         /* 0x1C */
    u8 delay;                         /* 0x1D */
    s8 transformed;                   /* 0x1E */
    s8 bonus;                         /* 0x1F */
    s8 active;                        /* 0x20 */
    char pad1;                        /* 0x21 */
    char pad2;                        /* 0x22 */
    char pad3;                        /* 0x23 */
} NEWWUMPA;

struct GLIDERBOMBSTRUCT {             /* 0x44 */
    struct nuvec_s Vel;               /* 0x0 */
    struct nuvec_s Pos;               /* 0xC */
    float AngY;                       /* 0x18 */
    int Life;                         /* 0x1C */
    float Gravity;                    /* 0x20 */
    struct nuvec_s Target;            /* 0x24 */
    struct nuvec_s * TargetPntr;      /* 0x30 */
    struct nuvec_s * TargetVelPntr;   /* 0x34 */
    int TargetMoving;                 /* 0x38 */
    int UnderWater;                   /* 0x3C */
    float DropTimer;                  /* 0x40 */
};

typedef struct {             /* 0x18 */
    struct nuvec_s pos;               /* 0x0 */
    float size;                       /* 0xC */
    short shade;                      /* 0x10 */
    short xrot;                       /* 0x12 */
    short yrot;                       /* 0x14 */
    short zrot;                       /* 0x16 */
} NuRndrShadType;

struct wumpa_s {                 /* 0x4C */
    struct nuvec_s pos0;         /* 0x00 */
    struct nuvec_s pos1;         /* 0x0C */
    struct nuvec_s pos;          /* 0x18 */
    struct nuvec_s mom;          /* 0x24 */
    float shadow;                /* 0x30 */
    u16 surface_xrot;            /* 0x34 */
    u16 surface_zrot;            /* 0x36 */
    float time;                  /* 0x38 */
    float duration;              /* 0x3C */
    s8 active;                   /* 0x40 */
    s8 iRAIL;                    /* 0x41 */
    short iALONG;                /* 0x42 */
    float fALONG;                /* 0x44 */
    s8 destroy;                  /* 0x48 */
    s8 fired;                    /* 0x49 */
    s8 in_range;                 /* 0x4A */
    s8 surface_type;             /* 0x4B */
};

struct setupDat {                /* 0x28 */
    struct nuvec_s vt[3];        /* 0x00 */
    int colour;                  /* 0x24 */
};

struct NUCOLLISIONSPHERE_s {             /* 0x14 */
    struct nuvec_s offset;            /* 0x0 */
    float r;                          /* 0xC */
    u8 joint;                         /* 0x10 */
    u8 pad[3];                        /* 0x11 */
};

typedef struct {             /* 0x30 */
    char name[16];                    /* 0x0 */
    u16 pitch;                        /* 0x10 */
    u16 volume;                       /* 0x12 */
    u8 buzz;                          /* 0x14 */
    u8 rumble;                        /* 0x15 */
    u8 delay;                         /* 0x16 */
    u8 wait;                          /* 0x17 */
    char * path;                      /* 0x18 */
    u16 frequency;                    /* 0x1C */
    u16 stream;                       /* 0x1E */
    s8 type;                          /* 0x20 */
    char pad1;                        /* 0x21 */
    u16 id;                           /* 0x22 */
    struct nuvec_s Pos;               /* 0x24 */
} GameSound;

typedef struct {             /* 0x10 */
    struct nuvec_s pos;               /* 0x0 */
    float radius;                     /* 0xC */
} Shperetype;

struct nuspline_s {           /* 0x40 */
    char *name;               /* 0x00 */
    int ncvs;                 /* 0x04 */
    struct nuvec_s *cvs;      /* 0x08 */
    float r;                  /* 0x0C */
    float r2;                 /* 0x10 */
    struct nuvec_s min;       /* 0x14 */
    struct nuvec_s max;       /* 0x20 */
    struct nuvec_s cntr;      /* 0x2C */
    float cntr_r;             /* 0x38 */
    float cntr_r2;            /* 0x3C */
};

typedef struct {             /* 0x34 */
    int offset;                       /* 0x0 */
    struct nuvec_s translation;       /* 0x4 */
    short type;                       /* 0x10 */
    short info;                       /* 0x12 */
    short rx;                         /* 0x14 */
    short ry;                         /* 0x16 */
    short rz;                         /* 0x18 */
    short pad;                        /* 0x1A */
    struct nuvec_s rotation;          /* 0x1C */
    TERRAINFLAGS flags;               /* 0x28 */
    short prim;                       /* 0x2C */
    short id;                         /* 0x2E */
    int datapos;                      /* 0x30 */
} OffType;

struct pSFX {                    /* 0x24 */
    char name[16];               /* 0x00 */
    u16 pitch;                   /* 0x10 */
    u16 volume;                  /* 0x12 */
    u8 buzz;                     /* 0x14 */
    u8 rumble;                   /* 0x15 */
    u8 delay;                    /* 0x16 */
    u8 wait;                     /* 0x17 */
    char *path;                  /* 0x18 */
    u16 frequency;               /* 0x1C */
    u16 stream;                  /* 0x1E */
    char type;                   /* 0x20 */
    char pad1;                   /* 0x21 */
    u16 id;                      /* 0x22 */
    struct nuvec_s Pos;          /* added - not always present */
};

struct debrissphere_s {             /* 0x10 */
    struct nuvec_s emit;              /* 0x0 */
    float t;                          /* 0xC */
};

struct anim_parameters_s {             /* 0x2D0 */
    int objid;                        /* 0x0 */
    int usedpart;                     /* 0x4 */
    int usedsound;                    /* 0x8 */
    int trigger_type;                 /* 0xC */
    int trigger_id;                   /* 0x10 */
    float trigger_var;                /* 0x14 */
    float trigger_wait;               /* 0x18 */
    int particle_type[8];             /* 0x9C */
    int particle_rate[8];             /* 0xBC */
    int particle_switch[8];           /* 0xDC */
    struct nuvec_s particle_offset[8]; /* 0xFC */
    short particle_emitrotz[8];       /* 0x15C */
    short particle_emitroty[8];       /* 0x16C */
    float sound_last_time;            /* 0x17C */
    int sound_id[8];                  /* 0x200 */
    int sound_type[8];                /* 0x220 */
    float sound_time[8];              /* 0x240 */
    struct nuvec_s sound_offset[8];   /* 0x260 */
    int terrplatid;                   /* 0x2C0 */
    float playergrav;                 /* 0x2C4 */
    float tension;                    /* 0x2C8 */
    float damping;                    /* 0x2CC */
};

struct WaterDat {                /* 0x24 */
    struct nuvec_s pos;          /* 0x00 */
    float size;                  /* 0x0C */
    float endsize;               /* 0x10 */
    float cursize;               /* 0x14 */
    u32 shade;                   /* 0x18 */
    u32 curshade;                /* 0x1C */
    short timer;                 /* 0x20 */
    short otimer;                /* 0x22 */
};

typedef struct {             /* 0x38 */
    struct nuvec_s Location;          /* 0x0 */
    short * model;                    /* 0xC */
    struct nuvec_s min;               /* 0x10 */
    struct nuvec_s max;               /* 0x1C */
    TERRAINFLAGS flags;               /* 0x28 */
    TERR_TYPE type;                   /* 0x2C */
    short info;                       /* 0x30 */
    short id;                         /* 0x32 */
    float radius;                     /* 0x34 */
} terrsitu;

typedef struct {             /* 0x4C */
    struct nuvec_s pos0;              /* 0x0 */
    struct nuvec_s pos1;              /* 0xC */
    struct nuvec_s pos;               /* 0x18 */
    struct nuvec_s mom;               /* 0x24 */
    float shadow;                     /* 0x30 */
    u16 surface_xrot;                 /* 0x34 */
    u16 surface_zrot;                 /* 0x36 */
    float time;                       /* 0x38 */
    float duration;                   /* 0x3C */
    s8 active;                        /* 0x40 */
    s8 iRAIL;                         /* 0x41 */
    short iALONG;                     /* 0x42 */
    float fALONG;                     /* 0x44 */
    s8 destroy;                       /* 0x48 */
    s8 fired;                         /* 0x49 */
    s8 in_range;                      /* 0x4A */
    s8 surface_type;                  /* 0x4B */
} WumpaFruit;

struct crate_s {             /* 0x28 */
    int id;                           /* 0x0 */
    char type[4];                     /* 0x4 */
    struct nuvec_s pos;               /* 0x8 */
    struct crate_s * linked;          /* 0x14 */
    struct crate_s * trigger;         /* 0x18 */
    u16 orientation;                  /* 0x1C */
    short offx;                       /* 0x1E */
    short offy;                       /* 0x20 */
    short offz;                       /* 0x22 */
    short ccindex;                    /* 0x24 */
    s8 draw;                          /* 0x26 */
    char cpad1;                       /* 0x27 */
};

struct mg_wumpa_s {             /* 0x10 */
    struct nuvec_s pos;               /* 0x0 */
    int type;                         /* 0xC */
};

typedef struct {             /* 0x64 */
    float minx;                       /* 0x0 */
    float maxx;                       /* 0x4 */
    float miny;                       /* 0x8 */
    float maxy;                       /* 0xC */
    float minz;                       /* 0x10 */
    float maxz;                       /* 0x14 */
    struct nuvec_s pnts[4];           /* 0x18 */
    struct nuvec_s norm[2];           /* 0x48 */
    u8 info[4];                       /* 0x60 */
} tertype;

typedef struct {             /* 0x14 */
    struct nuvec_s offset;            /* 0x0 */
    float ang;                        /* 0xC */
    float size;                       /* 0x10 */
} TerrShapeType;

struct nuvtx_sk3tc1_s {          /* 0x38 */
    struct nuvec_s pnt;          /* 0x00 */
    float weights[2];            /* 0x0C */
    float indexes[3];            /* 0x14 */
    struct nuvec_s nrm;          /* 0x20 */
    int diffuse;                 /* 0x2C */
    float tc[2];                 /* 0x30 */
};

typedef struct {             /* 0x28 */
    struct nuvec_s vt[3];             /* 0x0 */
    int colour;                       /* 0x24 */
} PartList;

typedef struct {             /* 0x24 */
    struct nuvec_s pos;               /* 0x0 */
    float size;                       /* 0xC */
    float endsize;                    /* 0x10 */
    float cursize;                    /* 0x14 */
    u32 shade;                        /* 0x18 */
    u32 curshade;                     /* 0x1C */
    short timer;                      /* 0x20 */
    short otimer;                     /* 0x22 */
} NuRndrWaterRType;

struct NUGCUTBBOX_s {             /* 0x18 */
    struct nuvec_s min;               /* 0x0 */
    struct nuvec_s max;               /* 0xC */
};

struct NUBLENDGEOM_s {        /* 0x40 */
    int nblends;              /* 0x00 */
    struct nuvec_s **blend_offsets; /* 0x04 */
    int *ix;                  /* 0x08 */
    struct nuvec_s *offsets;  /* 0x0C */
    struct nuvec_s *ooffsets; /* 0x10 */
    int hVB;                  /* 0x14 */
    int blendindex[10];       /* 0x18 */
};

typedef struct {             /* 0x24 */
    u16 type;                         /* 0x0 */
    short i;                          /* 0x2 */
    struct nuvec_s pos;               /* 0x4 */
    float radius0;                    /* 0x10 */
    float radius1;                    /* 0x14 */
    float time;                       /* 0x18 */
    float duration;                   /* 0x1C */
    u16 yrot;                         /* 0x20 */
    char pad2;                        /* 0x22 */
    char pad3;                        /* 0x23 */
} KABOOM;

struct trail_s {
    struct nuvec_s pos1;      /* 0x00 */
    struct nuvec_s pos2;      /* 0x0C */
    s32 intensity;            /* 0x18 */
    s32 RealIntensity;        /* 0x1C */
};

typedef struct {             /* 0x30 */
    struct nuvec_s origin;            /* 0x0 */
    float radius;                     /* 0xC */
    short iCrate;                     /* 0x10 */
    short nCrates;                    /* 0x12 */
    u16 angle;                        /* 0x14 */
    char pad1;                        /* 0x16 */
    char pad2;                        /* 0x17 */
    struct nuvec_s minclip;           /* 0x18 */
    struct nuvec_s maxclip;           /* 0x24 */
} CrateCubeGroup;

struct WBBOLT {             /* 0x30 */
    int Mode;                         /* 0x0 */
    int Owner;                        /* 0x4 */
    struct nuvec_s Position;          /* 0x8 */
    struct nuvec_s Velocity;          /* 0x14 */
    float Life;                       /* 0x20 */
    int Type;                         /* 0x24 */
    float SeekSpeed;                  /* 0x28 */
    float Scale;                      /* 0x2C */
};

struct JEEPBALLOON {             /* 0x30 */
    struct nuvec_s Pos;               /* 0x0 */
    struct nuvec_s Vel;               /* 0xC */
    int Active;                       /* 0x18 */
    int Seen;                         /* 0x1C */
    int Explode;                      /* 0x20 */
    int SmallDamage;                  /* 0x24 */
    float Life;                       /* 0x28 */
    short AngY;                       /* 0x2C */
};

struct ASTEROIDSTRUCT {             /* 0x4C */
    int Active;                       /* 0x0 */
    struct nuvec_s Position;          /* 0x4 */
    struct nuvec_s Velocity;          /* 0x10 */
    float AngleX;                     /* 0x1C */
    float AngleY;                     /* 0x20 */
    float AngVelX;                    /* 0x24 */
    float AngVelY;                    /* 0x28 */
    struct nuvec_s Scale;             /* 0x2C */
    int Seen;                         /* 0x38 */
    int HitPoints;                    /* 0x3C */
    float CentreDist2;                /* 0x40 */
    int Stealth;                      /* 0x44 */
    int Explode;                      /* 0x48 */
};

typedef struct {             /* 0x20 */
    struct nuvec_s start;             /* 0x0 */
    struct nuvec_s end;               /* 0xC */
    int time;                         /* 0x18 */
    int ang;                          /* 0x1C */
} ELEC;

struct lgtarclaserdata_s {       /* 0x40 */
    struct nuvec_s start;        /* 0x00 */
    struct nuvec_s target;       /* 0x0C */
    struct nuvec_s lasdir;       /* 0x18 */
    f32 sizew;                   /* 0x24 */
    f32 sizel;                   /* 0x28 */
    f32 sizewob;                 /* 0x2C */
    f32 arcsize;                 /* 0x30 */
    u8 type;                     /* 0x34 */
    u8 p1;                       /* 0x35 */
    u8 p2;                       /* 0x36 */
    u8 p3;                       /* 0x37 */
    s32 col;                     /* 0x38 */
    s32 seed;                    /* 0x3C */
};

struct FootData {                /* 0x34 */
    struct nuvec_s pnts[4];      /* 0x00 */
    short brightness;            /* 0x30 */
    char timer;                  /* 0x32 */
    char gfx;                    /* 0x33 */
};

struct nuvtx_ts_s {              /* 0x18 */
    struct nuvec_s vTangent;     /* 0x00 */
    struct nuvec_s vNormal;      /* 0x0C */
};

typedef struct {             /* 0x34 */
    struct nuvec_s pnts[4];           /* 0x0 */
    short brightness;                 /* 0x30 */
    char timer;                       /* 0x32 */
    char gfx;                         /* 0x33 */
} NuRndrFootType;

struct NUCYLINDER_s {         /* 0x40 */
    struct nuvec_s centre;    /* 0x00 */
    struct nuvec_s y_axis;    /* 0x0C */
    struct nuvec4_s x_axis;   /* 0x18 */
    struct nuvec4_s z_axis;   /* 0x28 */
    char pad[8];              /* 0x38 */
};

struct NUSHADOWMESH_s {
    struct nuvec4_s *normals;
    struct nuvec4_s *verts;
};

typedef struct {             /* 0x34 */
    struct nuangvec_s ang;            /* 0x0 */
    struct nuangvec_s angmom;         /* 0xC */
    struct nuvec_s pos;               /* 0x18 */
    struct nuvec_s mom;               /* 0x24 */
    int rndfade;                      /* 0x30 */
} BoxPolType;

struct CamMtx {
    struct numtx_s m;            /* 0x00 */
    struct numtx_s m0;           /* 0x40 */
    struct nuvec_s vX;           /* 0x80 */
    struct nuvec_s vY;           /* 0x8C */
    struct nuvec_s vZ;           /* 0x98 */
    struct nuvec_s pos;          /* 0xA4 */
    struct nuvec_s oldpos;       /* 0xB0 */
    struct nuvec_s newpos;       /* 0xBC */
    struct nuvec_s pos2;         /* 0xC8 */
    struct nuvec_s seek;         /* 0xD4 */
    float distance;              /* 0xE0 */
    float ahead;                 /* 0xE4 */
    float judder;                /* 0xE8 */
    float blend_time;            /* 0xEC */
    float blend_duration;        /* 0xF0 */
    int xrot;                    /* 0xF4 */
    int yrot;                    /* 0xF8 */
    int zrot;                    /* 0xFC */
    u16 old_xrot;                /* 0x100 */
    u16 new_xrot;                /* 0x102 */
    u16 old_yrot;                /* 0x104 */
    u16 new_yrot;                /* 0x106 */
    u16 old_zrot;                /* 0x108 */
    u16 new_zrot;                /* 0x10A */
    u16 hdg_to_player;           /* 0x10C */
    short route;                 /* 0x10E */
    short i0;                    /* 0x110 */
    short i1;                    /* 0x112 */
    char mode;                   /* 0x114 */
    char iRAIL;                  /* 0x115 */
    short iALONG;                /* 0x116 */
    u16 angle;                   /* 0x118 */
    char vertical;               /* 0x11A */
    char pad1;                   /* 0x11B */
};

struct nureflect_s {          /* 0x84 */
    float distortion;         /* 0x00 */
    struct numtx_s mtx;       /* 0x04 */
    struct numtx_s uvmtx;     /* 0x44 */
};

typedef struct {             /* 0x11C */
    struct numtx_s m;                 /* 0x0 */
    struct numtx_s m0;                /* 0x40 */
    struct nuvec_s vX;                /* 0x80 */
    struct nuvec_s vY;                /* 0x8C */
    struct nuvec_s vZ;                /* 0x98 */
    struct nuvec_s pos;               /* 0xA4 */
    struct nuvec_s oldpos;            /* 0xB0 */
    struct nuvec_s newpos;            /* 0xBC */
    struct nuvec_s pos2;              /* 0xC8 */
    struct nuvec_s seek;              /* 0xD4 */
    float distance;                   /* 0xE0 */
    float ahead;                      /* 0xE4 */
    float judder;                     /* 0xE8 */
    float blend_time;                 /* 0xEC */
    float blend_duration;             /* 0xF0 */
    int xrot;                         /* 0xF4 */
    int yrot;                         /* 0xF8 */
    int zrot;                         /* 0xFC */
    u16 old_xrot;                     /* 0x100 */
    u16 new_xrot;                     /* 0x102 */
    u16 old_yrot;                     /* 0x104 */
    u16 new_yrot;                     /* 0x106 */
    u16 old_zrot;                     /* 0x108 */
    u16 new_zrot;                     /* 0x10A */
    u16 hdg_to_player;                /* 0x10C */
    short route;                      /* 0x10E */
    short i0;                         /* 0x110 */
    short i1;                         /* 0x112 */
    s8 mode;                          /* 0x114 */
    s8 iRAIL;                         /* 0x115 */
    short iALONG;                     /* 0x116 */
    u16 angle;                        /* 0x118 */
    s8 vertical;                      /* 0x11A */
    char pad1;                        /* 0x11B */
} GameCamera;

struct winfo_s {                 /* 0x50 */
    struct numtx_s m;            /* 0x00 */
    float scale;                 /* 0x40 */
    float dy;                    /* 0x44 */
    u16 angle;                   /* 0x48 */
    u16 spin;                    /* 0x4A */
    char pad1;                   /* 0x4C */
    char pad2;                   /* 0x4D */
    char pad3;                   /* 0x4E */
    char pad4;                   /* 0x4F */
};

struct NUJOINTDATA_s {        /* large */
    struct numtx_s orient;    /* 0x00 */
    struct nuvec_s locator_offset; /* 0x40 */
    char *name;               /* 0x4C */
    int parent_ix;            /* 0x50 */
    char flags;               /* 0x54 */
    char pad[11];             /* 0x55 */
};

struct nuinstanim_s {         /* 0x60 */
    struct numtx_s mtx;       /* 0x00 */
    float tfactor;            /* 0x40 */
    float tfirst;             /* 0x44 */
    float tinterval;          /* 0x48 */
    float ltime;              /* 0x4C */
    u32 flags;                /* 0x50 - playing:1, backwards:1, waiting:1, repeating:1, oscillate:1 */
    int ipad[2];              /* 0x54 */
    u8 anim_ix;               /* 0x5C */
    char pad[3];              /* 0x5D */
};

typedef struct {             /* 0x64 */
    struct numtx_s oldmtx;            /* 0x0 */
    struct numtx_s * curmtx;          /* 0x40 */
    short terrno;                     /* 0x44 */
    short instance;                   /* 0x46 */
    PLATATTRIB status;                /* 0x48 */
    short hitcnt;                     /* 0x4C */
    float plrgrav;                    /* 0x50 */
    float ypos;                       /* 0x54 */
    float yvel;                       /* 0x58 */
    float tension;                    /* 0x5C */
    float damp;                       /* 0x60 */
} TERRPLATTYPE;

struct NUPOINTOFINTEREST_s {  /* large */
    struct numtx_s offset;    /* 0x00 */
    char *name;               /* 0x40 */
    u8 parent_joint_ix;       /* 0x44 */
    u8 pad[11];               /* 0x45 */
};

struct GLIDERBULLET {             /* 0x50 */
    struct numtx_s Mat;               /* 0x0 */
    struct nuvec_s Vel;               /* 0x40 */
    short Life;                       /* 0x4C */
    short FirstLife;                  /* 0x4E */
};

typedef struct {             /* 0x90 */
    struct numtx_s mM;                /* 0x0 */
    struct numtx_s mS;                /* 0x40 */
    float time;                       /* 0x80 */
    float duration;                   /* 0x84 */
    float shadow;                     /* 0x88 */
    char pad1;                        /* 0x8C */
    char pad2;                        /* 0x8D */
    char pad3;                        /* 0x8E */
    char pad4;                        /* 0x8F */
} MaskFeathersInfo;

struct nucamera_s {           /* 0x60 */
    struct numtx_s mtx;       /* 0x00 */
    f32 fov;                  /* 0x40 */
    f32 aspect;               /* 0x44 */
    f32 nearclip;             /* 0x48 */
    f32 farclip;              /* 0x4C */
    f32 portalnearclip;       /* 0x50 */
    struct nuvec_s scale;     /* 0x54 */
};

struct mfeathers_s {             /* 0x90 */
    struct numtx_s mM;           /* 0x00 */
    struct numtx_s mS;           /* 0x40 */
    float time;                  /* 0x80 */
    float duration;              /* 0x84 */
    float shadow;                /* 0x88 */
    char pad1;                   /* 0x8C */
    char pad2;                   /* 0x8D */
    char pad3;                   /* 0x8E */
    char pad4;                   /* 0x8F */
};

typedef struct {             /* 0x50 */
    struct numtx_s m;                 /* 0x0 */
    float scale;                      /* 0x40 */
    float dy;                         /* 0x44 */
    u16 angle;                        /* 0x48 */
    u16 spin;                         /* 0x4A */
    char pad1;                        /* 0x4C */
    char pad2;                        /* 0x4D */
    char pad3;                        /* 0x4E */
    char pad4;                        /* 0x4F */
} WumpaInfo;

struct NUGCUTCAM_s {             /* 0x50 */
    struct numtx_s mtx;               /* 0x0 */
    u8 flags;                         /* 0x40 */
    u8 id;                            /* 0x41 */
    u8 anim_ix;                       /* 0x42 */
    u8 pad[13];                       /* 0x43 */
};

struct deb3_s {             /* 0xE0 */
    struct numtx_s mtx;               /* 0x0 */
    struct numtx_s invWorldInertiaTensor; /* 0x40 */
    struct nuvec_s velocity;          /* 0x80 */
    struct nuvec_s angularvelocity;   /* 0x8C */
    struct nuvec_s angularMomentum;   /* 0x98 */
    struct nuvec_s impact;            /* 0xA4 */
    struct nuvec_s norm;              /* 0xB0 */
    struct nuvec_s diff;              /* 0xBC */
    float shadow;                     /* 0xC8 */
    float grav;                       /* 0xCC */
    short status;                     /* 0xD0 */
    short timer;                      /* 0xD2 */
    short check;                      /* 0xD4 */
    short count;                      /* 0xD6 */
    int data;                         /* 0xDC */
};

typedef struct {             /* 0x4C */
    struct numtx_s invBodyInertiaTensor; /* 0x0 */
    float mass;                       /* 0x40 */
    float kr;                         /* 0x44 */
    float kf;                         /* 0x48 */
} RBCLASS;

typedef struct {             /* 0x20 */
    int Index;                        /* 0x0 */
    struct nuvec_s Direction;         /* 0x4 */
    struct nucolour3_s Colour;        /* 0x10 */
    float Distance;                   /* 0x1C */
} NearLight;

struct nulight_s {               /* 0x58 */
    struct nucolour3_s ambient;  /* 0x00 */
    struct nucolour3_s diffuse;  /* 0x0C */
    struct numtx_s mtx;          /* 0x18 */
};

typedef struct {             /* 0x34 */
    struct nucolour3_s HighColour;    /* 0x0 */
    struct nucolour3_s LowColour;     /* 0xC */
    struct nucolour3_s CurColour;     /* 0x18 */
    struct nucolour3_s ColDiff;       /* 0x24 */
    u16 Step;                         /* 0x30 */
    u16 CurAngle;                     /* 0x32 */
} Glow_Light;

struct pdir_s {                  /* 0x20 */
    int Index;                   /* 0x00 */
    struct nuvec_s Direction;    /* 0x04 */
    struct nucolour3_s Colour;   /* 0x10 */
    float Distance;              /* 0x1C */
};

struct numemfile_s {          /* 0x14 */
    char *start;              /* 0x00 */
    char *end;                /* 0x04 */
    char *curr;               /* 0x08 */
    int mode;                 /* 0x0C - nufilemode_e */
    s32 used;                 /* 0x10 */
};

struct blkinfo_s {             /* 0xC */
    struct nuiffhdr_s hdr;            /* 0x0 */
    int pos;                          /* 0x8 */
};

struct BlockInfo {            /* 0x0C */
    struct nuiffhdr_s hdr;    /* 0x00 */
    s32 pos;                  /* 0x08 */
};

struct nudathdr_s {           /* 0x30 */
    s32 ver;                  /* 0x00 */
    s32 nfiles;               /* 0x04 */
    struct nudatinfo_s *finfo;/* 0x08 */
    s32 treecnt;              /* 0x0C */
    struct nudfnode_s *filetree; /* 0x10 */
    s32 leafnamesize;         /* 0x14 */
    char *leafnames;          /* 0x18 */
    s32 dfhandle;             /* 0x1C */
    s32 fh;                   /* 0x20 */
    short intalloc;           /* 0x24 */
    short openmode;           /* 0x26 */
    s32 start_lsn;            /* 0x28 */
    void *memdatptr;          /* 0x2C */
};

struct nufpar_s {             /* large */
    s32 handle;               /* 0x000 */
    char fbuff[4096];         /* 0x004 */
    char lbuff[256];          /* 0x1004 */
    char wbuff[260];          /* 0x1104 */
    s32 line_num;
    s32 line_pos;
    s32 cpos;
    s32 buffstart;
    s32 buffend;
    struct nufpcomjmp_s *comstack[8];
    s32 compos;
    s32 size;
};

struct nutex_s {              /* 0x1C */
    int type;                 /* 0x00 - nutextype_e */
    int width;                /* 0x04 */
    int height;               /* 0x08 */
    int mmcnt;                /* 0x0C */
    void *bits;               /* 0x10 */
    int *pal;                 /* 0x14 */
    short decal;              /* 0x18 */
    short linear;             /* 0x1A */
};

struct nurndritem_s {         /* 0x10 */
    struct nurndritem_s *next; /* 0x00 */
    int type;                 /* 0x04 - nurndritemtype_s */
    s32 flags;                /* 0x08 */
    s16 lights_index;         /* 0x0C */
};

struct nuprim_s {             /* 0x3C */
    struct nuprim_s *next;    /* 0x00 */
    int type;                 /* 0x04 - nuprimtype_e */
    u16 cnt;                  /* 0x08 */
    u16 max;                  /* 0x0A */
    u16 *vid;                 /* 0x0C */
    struct nuplane_s *pln;    /* 0x10 */
    int idxbuff;              /* 0x14 */
    int cached;               /* 0x18 */
    short skinmtxlookup[16];  /* 0x1C */
};

struct numtl_s {              /* 0x54 */
    struct numtl_s *next;     /* 0x00 */
    struct numtlattrib_s attrib; /* 0x04 */
    struct nucolour3_s ambient;  /* 0x08 */
    struct nucolour3_s diffuse;  /* 0x14 */
    union nufx_u fx1;         /* 0x20 */
    union nufx_u fx2;         /* 0x24 */
    union nufx_u fx3;         /* 0x28 */
    union nufx_u fx4;         /* 0x2C */
    f32 power;                /* 0x30 */
    f32 alpha;                /* 0x34 */
    s32 tid;                  /* 0x38 */
    s16 alpha_sort;           /* 0x3C */
    u8 fxid;                  /* 0x3E */
    u8 special_id;            /* 0x3F */
    s16 K;                    /* 0x40 */
    u8 L;                     /* 0x42 */
    u8 anmmode;               /* 0x43 - uanmmode:4, vanmmode:4 */
    f32 du;                   /* 0x44 */
    f32 dv;                   /* 0x48 */
    f32 su;                   /* 0x4C */
    f32 sv;                   /* 0x50 */
};

struct NUREFERENCENODE_s {             /* 0x2C */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    u32 ref_id;                       /* 0x20 */
    struct NUNODE_s * ref_node;       /* 0x24 */
    char * ref_name;                  /* 0x28 */
};

struct NUTRANSFORMNODE_s {             /* 0xA8 */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    struct numtx_s T;                 /* 0x20 */
    struct numtx_s WT;                /* 0x60 */
    int transform_flags;              /* 0xA0 */
    void * ref_node;                  /* 0xA4 */
};

struct NUJOINTNODE_s {             /* 0x114 */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    struct numtx_s orient_mtx;        /* 0x20 */
    struct numtx_s T;                 /* 0x60 */
    struct numtx_s WT;                /* 0xA0 */
    char mtx_flags;                   /* 0xE0 */
    struct nuvec_s translation;       /* 0xE4 */
    struct nuangvec_s rotation;       /* 0xF0 */
    struct nuvec_s scale;             /* 0xFC */
    struct nuvec_s locator_offset;    /* 0x108 */
};

struct nufspecial_s {            /* 0x50 */
    struct numtx_s mtx;          /* 0x00 */
    int instanceix;              /* 0x40 */
    int nameix;                  /* 0x44 */
    struct nuspecialflags flags; /* 0x48 */
    int pad;                     /* 0x4C */
};

typedef struct {             /* 0x28 */
    struct nugspline_s * pINPLAT;     /* 0x0 */
    struct nugspline_s * pINCAM;      /* 0x4 */
    struct nugspline_s * pLEFT;       /* 0x8 */
    struct nugspline_s * pCAM;        /* 0xC */
    struct nugspline_s * pRIGHT;      /* 0x10 */
    struct nugspline_s * pOUTCAM;     /* 0x14 */
    struct nugspline_s * pOUTPLAT;    /* 0x18 */
    float in_distance;                /* 0x1C */
    float out_distance;               /* 0x20 */
    short edges;                      /* 0x24 */
    s8 type;                          /* 0x26 */
    s8 circuit;                       /* 0x27 */
} RailData;

struct spltab_s {                /* 0x18 */
    struct nugspline_s *spl;     /* 0x00 */
    char *name;                  /* 0x04 */
    short min;                   /* 0x08 */
    short max;                   /* 0x0A */
    u64 levbits;                 /* 0x10 */
};

typedef struct {             /* 0x18 */
    struct nugspline_s * spl;         /* 0x0 */
    char * name;                      /* 0x4 */
    short min;                        /* 0x8 */
    short max;                        /* 0xA */
    u64 levbits;                      /* 0x10 */
} LevelSpline;

struct MYSPLINE {             /* 0x30 */
    struct nugspline_s * Spline;      /* 0x0 */
    float Cur;                        /* 0x4 */
    float Nex;                        /* 0x8 */
    float Act;                        /* 0xC */
    float Inc;                        /* 0x10 */
    struct nuvec_s CurPos;            /* 0x14 */
    struct nuvec_s NexPos;            /* 0x20 */
    float LookaheadDist;              /* 0x2C */
};

struct MYDRAW {             /* 0xE0 */
    int Character;                    /* 0x20 */
    int NumJoints;                    /* 0x24 */
    struct NUJOINTANIM_s * JointList; /* 0x28 */
    struct nuvec_s * Position;        /* 0x2C */
};

union variptr_u {                /* 0x04 */
    void *voidptr;
    u8 *u8ptr;
    u16 *u16ptr;
    s16 *s16ptr;
    u32 *u32ptr;
    s64 *u64ptr;
    s64 *u128ptr;
    f32 *f32ptr;
    struct nuvec_s *vec3;
    struct nuvec4_s *vec4;
    struct nuivec_s *ivec3;
    struct nuivec4_s *ivec4;
    struct numtx_s *mtx44;
    u32 *viftag;
    u32 intaddr;
    struct _sceDmaTag *dmatag;
    struct giftag *gifTag;
};

struct fileinfo_s {              /* 0x20 */
    s32 fh;                      /* 0x00 */
    s32 read_pos;                /* 0x04 */
    s32 file_pos;                /* 0x08 */
    s32 file_length;             /* 0x0C */
    s32 buff_start;              /* 0x10 */
    s32 buff_length;             /* 0x14 */
    s32 use_buff;                /* 0x18 */
    struct filebuff_s *buff;     /* 0x1C */
};

struct NUCOLLISIONHDR_s {        /* 0x08 */
    int type;                    /* 0x00 - NUCOLLISIONTYPES */
    void *data;                  /* 0x04 */
};

struct _GS_TEXTURE {             /* 0x20 - PS2 version without GC texobj */
    u32 Flags;                   /* 0x00 */
    int Format;                  /* 0x04 - gs_texturetype_e */
    u32 Width;                   /* 0x08 */
    u32 Height;                  /* 0x0C */
    u32 TexBits;                 /* 0x10 */
    u32 PalBits;                 /* 0x14 */
    u32 NUID;                    /* 0x18 */
    u32 Pad;                     /* 0x1C */
};

struct _D3DMATERIAL8 {           /* 0x44 */
    struct _D3DCOLORVALUE Diffuse;  /* 0x00 */
    struct _D3DCOLORVALUE Ambient;  /* 0x10 */
    struct _D3DCOLORVALUE Specular; /* 0x20 */
    struct _D3DCOLORVALUE Emissive; /* 0x30 */
    float Power;                 /* 0x40 */
};

struct _D3DLIGHT8 {              /* 0x68 */
    int Type;                    /* 0x00 - _D3DLIGHTTYPE */
    struct _D3DCOLORVALUE Diffuse;  /* 0x04 */
    struct _D3DCOLORVALUE Specular; /* 0x14 */
    struct _D3DCOLORVALUE Ambient;  /* 0x24 */
    struct _D3DVECTOR Position;  /* 0x34 */
    struct _D3DVECTOR Direction; /* 0x40 */
    f32 range;                   /* 0x4C */
    f32 falloff;                 /* 0x50 */
    f32 attenuation0;            /* 0x54 */
    f32 attenuation1;            /* 0x58 */
    f32 attenuation2;            /* 0x5C */
    f32 theta;                   /* 0x60 */
    f32 phi;                     /* 0x64 */
};

struct pVTog {
    struct nugspline_s *pTRIGGER; /* 0x00 */
    struct nugspline_s *pCAM;    /* 0x04 */
    struct nugspline_s *pLOOK;   /* 0x08 */
    struct numtx_s m;            /* 0x0C */
    struct nuvec_s pos;          /* 0x4C */
    struct anim_s anim;          /* 0x58 */
    char type;                   /* 0x74 */
    char iRAIL;                  /* 0x75 */
    short iALONG;                /* 0x76 */
    float fALONG;                /* 0x78 */
    u16 xrot;                    /* 0x7C */
    u16 yrot;                    /* 0x7E */
    u16 zrot;                    /* 0x80 */
    char pad1;                   /* 0x82 */
    char pad2;                   /* 0x83 */
};

typedef struct {             /* 0x84 */
    struct nugspline_s * pTRIGGER;    /* 0x0 */
    struct nugspline_s * pCAM;        /* 0x4 */
    struct nugspline_s * pLOOK;       /* 0x8 */
    struct numtx_s m;                 /* 0xC */
    struct nuvec_s pos;               /* 0x4C */
    struct anim_s anim;               /* 0x58 */
    s8 type;                          /* 0x74 */
    s8 iRAIL;                         /* 0x75 */
    short iALONG;                     /* 0x76 */
    float fALONG;                     /* 0x78 */
    u16 xrot;                         /* 0x7C */
    u16 yrot;                         /* 0x7E */
    u16 zrot;                         /* 0x80 */
    char pad1;                        /* 0x82 */
    char pad2;                        /* 0x83 */
} VehicleToggle;

struct cursor_s {
    struct remember remember[48]; /* 0x00 */
    u32 menu_frame;              /* 0x60 */
    u32 item_frame;              /* 0x64 */
    char x;                      /* 0x68 */
    char y;                      /* 0x69 */
    char x_min;                  /* 0x6A */
    char y_min;                  /* 0x6B */
    char x_max;                  /* 0x6C */
    char y_max;                  /* 0x6D */
    char menu;                   /* 0x6E */
    char new_menu;               /* 0x6F */
    char new_level;              /* 0x70 */
    u8 wait;                     /* 0x71 */
    u8 wait_frames;              /* 0x72 */
    char wait_hack;              /* 0x73 */
    u8 button_lock;              /* 0x74 */
    char pad1;                   /* 0x75 */
    char pad2;                   /* 0x76 */
    char pad3;                   /* 0x77 */
};

struct level_s {
    u16 flags;                   /* 0x00 */
    char pad1;                   /* 0x02 */
    char pad2;                   /* 0x03 */
    struct time_s time[3];       /* 0x04 */
};

struct space_s {                 /* 0x7C */
    s32 character;               /* 0x00 */
    struct animlist animlist[5]; /* 0x04 */
};

typedef struct {                 /* 0x34 */
    char *path;                  /* 0x00 */
    char *file;                  /* 0x04 */
    char *name;                  /* 0x08 */
    struct animlist *anim;       /* 0x0C */
    float radius;                /* 0x10 */
    struct nuvec_s min;          /* 0x14 */
    struct nuvec_s max;          /* 0x20 */
    float scale;                 /* 0x2C */
    float shadow_scale;          /* 0x30 */
} CharacterData;

struct BUGSAVE {             /* 0x1C */
    struct nuquat_s Orientation;      /* 0x0 */
    short x;                          /* 0x10 */
    short y;                          /* 0x12 */
    short z;                          /* 0x14 */
    short FrontWheelAng;              /* 0x16 */
    s8 WheelLength[4];                /* 0x18 */
};

struct NUTRIGGER_s {             /* 0x34 */
    char * triggername;               /* 0x0 */
    int trigger_type;                 /* 0x4 */
    short hitpoints;                  /* 0x8 */
    char enableflags;                 /* 0xA */
    char pad;                         /* 0xB */
    int scale_transform;              /* 0xC */
    int display_box;                  /* 0xC */
    int persistant;                   /* 0xC */
    float radius;                     /* 0x10 */
    struct nuvec_s min;               /* 0x14 */
    struct nuvec_s max;               /* 0x20 */
    short numprims;                   /* 0x2C */
    short instance_ix;                /* 0x2E */
    struct NUTRIGGERPRIM_s * prims;   /* 0x30 */
};

typedef struct {             /* 0xEA610 */
    int padpointer;                   /* 0x0 */
    int padmode;                      /* 0x4 */
    int padend;                       /* 0x8 */
    int padsize;                      /* 0xC */
    NUPADREC PadRecData[48000];       /* 0x10 */
} PadRecType;

struct NUGCUTTRIGGER_s {             /* 0xC */
    short ix;                         /* 0x0 */
    short pad;                        /* 0x2 */
    char * triggername;               /* 0x4 */
    struct NUSTATEANIM_s * enableflag1anim; /* 0x8 */
};

struct instNUGCUTLOCATORSYS_s {             /* 0x4 */
    struct instNUGCUTLOCATOR_s * ilocators; /* 0x0 */
};

struct instNUGCUTCHARSYS_s {             /* 0x4 */
    struct instNUGCUTCHAR_s * ichars; /* 0x0 */
};

typedef struct {             /* 0x410 */
    int dmadata[2];                   /* 0x0 */
    int unpackdata[2];                /* 0x8 */
    uv1deb debris[32];                /* 0x10 */
} uv1debdata;

struct singledebriskeytype_s {             /* 0x8 */
    struct singledebriskeytype_s * next; /* 0x0 */
    uv1deb * debris;                  /* 0x4 */
};

typedef struct {             /* 0x6C */
    char * path;                      /* 0x0 */
    char * chassis_name;              /* 0x4 */
    WheelData wheel[12];              /* 0x8 */
    float radius;                     /* 0x68 */
} VehicleData;

typedef struct {             /* 0x80 */
    int character;                    /* 0x0 */
    AnimList animlist[5];             /* 0x8 */
} SPACEANIM;

typedef struct {             /* 0x78 */
    CursorMemory remember[48];        /* 0x0 */
    u32 menu_frame;                   /* 0x60 */
    u32 item_frame;                   /* 0x64 */
    s8 x;                             /* 0x68 */
    s8 y;                             /* 0x69 */
    s8 x_min;                         /* 0x6A */
    s8 y_min;                         /* 0x6B */
    s8 x_max;                         /* 0x6C */
    s8 y_max;                         /* 0x6D */
    s8 menu;                          /* 0x6E */
    s8 new_menu;                      /* 0x6F */
    s8 new_level;                     /* 0x70 */
    u8 wait;                          /* 0x71 */
    u8 wait_frames;                   /* 0x72 */
    s8 wait_hack;                     /* 0x73 */
    u8 button_lock;                   /* 0x74 */
    char pad1;                        /* 0x75 */
    char pad2;                        /* 0x76 */
    char pad3;                        /* 0x77 */
} Selector;

typedef struct {             /* 0x18 */
    DSLink Link;                      /* 0x0 */
    char * Name;                      /* 0xC */
    char * Data;                      /* 0x10 */
    u16 ReferenceCount;               /* 0x14 */
} DSCacheNode;

typedef struct {             /* 0x10 */
    u8 PurgeFlag;                     /* 0x0 */
    DSList CacheNodeList;             /* 0x4 */
} DSCache;

typedef struct {             /* 0xC */
    DSList * table;                   /* 0x0 */
    u16 tableSize;                    /* 0x4 */
    DSHashFunc * hash;                /* 0x8 */
} DSHashTable;

typedef struct {             /* 0x1C */
    u16 flags;                        /* 0x0 */
    char pad1;                        /* 0x2 */
    char pad2;                        /* 0x3 */
    LevelTime time[3];                /* 0x4 */
} LevelState;

typedef struct {             /* 0x40 */
    PanelDebrisInfo * info;           /* 0x0 */
    struct nuvec_s oldpos;            /* 0x4 */
    struct nuvec_s pos;               /* 0x10 */
    struct nuvec_s mom;               /* 0x1C */
    float time;                       /* 0x28 */
    float oldscale;                   /* 0x2C */
    float scale;                      /* 0x30 */
    u16 xrot;                         /* 0x34 */
    u16 yrot;                         /* 0x36 */
    u16 zrot;                         /* 0x38 */
    short i_objtab;                   /* 0x3A */
    s8 type;                          /* 0x3C */
    s8 active;                        /* 0x3D */
    char pad1;                        /* 0x3E */
    char pad2;                        /* 0x3F */
} PanelDebris;

struct LIGHTENINGHAIL {             /* 0x24 */
    int Mode;                         /* 0x0 */
    float Timer;                      /* 0x4 */
    float FallX;                      /* 0x8 */
    struct nuvec_s Position;          /* 0xC */
    struct nuvec_s Velocity;          /* 0x18 */
};

struct instNUGCUTCAMSYS_s {             /* 0x10 */
    struct NUGCUTCAMTGT_s * tgts;     /* 0x0 */
    struct instNUGCUTCAM_s * icutcams; /* 0x4 */
    u8 next_switch;                   /* 0x8 */
    char current_camera;              /* 0x9 */
    u8 next_tgt_ix;                   /* 0xA */
    u8 tgtarraysize;                  /* 0xB */
    u8 ntgts;                         /* 0xC */
    char pad[3];                      /* 0xD */
};

struct primdef_s {               /* 0x174 */
    struct nuvtx_tc1_s vrts[3];  /* 0x000 */
    int vid[3];                  /* 0x06C */
    float weights[3][15];        /* 0x078 */
    int mtxid[15];               /* 0x12C */
    int nummtx;                  /* 0x168 */
    int sorted;                  /* 0x16C */
    int baseid;                  /* 0x170 */
};

struct setup_s {                 /* 0xA48 */
    int DmaHeader[4];            /* 0x000 */
    float grav;                  /* 0x010 */
    float gtime;                 /* 0x014 */
    int DmaBody[4];              /* 0x018 */
    float u0;                    /* 0x028 */
    float v0;                    /* 0x02C */
    float u1;                    /* 0x030 */
    float v1;                    /* 0x034 */
    float u2;                    /* 0x038 */
    float v2;                    /* 0x03C */
    float u3;                    /* 0x040 */
    float v3;                    /* 0x044 */
    struct setupDat Data[64];    /* 0x048 */
};

struct NUSPLINENODE_s {             /* 0x2C */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    int nsplines;                     /* 0x20 */
    struct nuspline_s * * spline;     /* 0x24 */
    int * sid;                        /* 0x28 */
};

typedef struct {             /* 0x6804 */
    int count;                        /* 0x0 */
    OffType offlist[512];             /* 0x4 */
} OffFileType;

struct ldata_s {                 /* 0x58 */
    char *filepath;              /* 0x00 */
    u8 *clist;                   /* 0x04 - modelList */
    struct pCHASE *pChase;       /* 0x08 */
    u32 time[3];                 /* 0x0C */
    short music[2];              /* 0x18 */
    struct pSFX *pSFX;           /* 0x1C */
    short nSFX;                  /* 0x20 */
    char pad1;                   /* 0x22 */
    char hub;                    /* 0x23 */
    u16 flags;                   /* 0x24 */
    short character;             /* 0x26 */
    short vehicle;               /* 0x28 */
    u16 farclip;                 /* 0x2A */
    struct nuvec_s vSTART;       /* 0x2C */
    struct nuvec_s vBONUS;       /* 0x38 */
    float fognear;               /* 0x44 */
    float fogfar;                /* 0x48 */
    u8 fogr;                     /* 0x4C */
    u8 fogg;                     /* 0x4D */
    u8 fogb;                     /* 0x4E */
    u8 foga;                     /* 0x4F */
    u8 hazer;                    /* 0x50 */
    u8 hazeg;                    /* 0x51 */
    u8 hazeb;                    /* 0x52 */
    u8 hazea;                    /* 0x53 */
    int totalsize;               /* 0x54 */
};

typedef struct {             /* 0x58 */
    char * path;                      /* 0x0 */
    u8 * clist;                       /* 0x4 */
    ChaseData * pCHASE;               /* 0x8 */
    u32 time[3];                      /* 0xC */
    short music[2];                   /* 0x18 */
    GameSound * pSFX;                 /* 0x1C */
    short nSFX;                       /* 0x20 */
    char pad1;                        /* 0x22 */
    s8 hub;                           /* 0x23 */
    u16 flags;                        /* 0x24 */
    short character;                  /* 0x26 */
    short vehicle;                    /* 0x28 */
    u16 farclip;                      /* 0x2A */
    struct nuvec_s vSTART;            /* 0x2C */
    struct nuvec_s vBONUS;            /* 0x38 */
    float fognear;                    /* 0x44 */
    float fogfar;                     /* 0x48 */
    u8 fogr;                          /* 0x4C */
    u8 fogg;                          /* 0x4D */
    u8 fogb;                          /* 0x4E */
    u8 foga;                          /* 0x4F */
    u8 hazer;                         /* 0x50 */
    u8 hazeg;                         /* 0x51 */
    u8 hazeb;                         /* 0x52 */
    u8 hazea;                         /* 0x53 */
    int totalsize;                    /* 0x54 */
} LevelData;

typedef struct {             /* 0x90 */
    struct crate_s * model;           /* 0x0 */
    struct nuvec_s pos0;              /* 0x4 */
    struct nuvec_s pos;               /* 0x10 */
    float oldy;                       /* 0x1C */
    float shadow;                     /* 0x20 */
    float mom;                        /* 0x24 */
    float timer;                      /* 0x28 */
    float duration;                   /* 0x2C */
    s8 on;                            /* 0x30 */
    s8 iRAIL;                         /* 0x31 */
    short iALONG;                     /* 0x32 */
    float fALONG;                     /* 0x34 */
    u16 flags;                        /* 0x38 */
    s8 type1;                         /* 0x3A */
    s8 type2;                         /* 0x3B */
    s8 type3;                         /* 0x3C */
    s8 type4;                         /* 0x3D */
    s8 newtype;                       /* 0x3E */
    s8 subtype;                       /* 0x3F */
    s8 i;                             /* 0x40 */
    s8 metal_count;                   /* 0x41 */
    s8 appeared;                      /* 0x42 */
    s8 in_range;                      /* 0x43 */
    short dx;                         /* 0x44 */
    short dy;                         /* 0x46 */
    short dz;                         /* 0x48 */
    short iU;                         /* 0x4A */
    short iD;                         /* 0x4C */
    short iN;                         /* 0x4E */
    short iS;                         /* 0x50 */
    short iE;                         /* 0x52 */
    short iW;                         /* 0x54 */
    short trigger;                    /* 0x56 */
    s8 counter;                       /* 0x58 */
    s8 anim_cycle;                    /* 0x59 */
    short index;                      /* 0x5A */
    float anim_time;                  /* 0x5C */
    float anim_duration;              /* 0x60 */
    float anim_speed;                 /* 0x64 */
    u16 xrot0;                        /* 0x68 */
    u16 zrot0;                        /* 0x6A */
    u16 xrot;                         /* 0x6C */
    u16 zrot;                         /* 0x6E */
    u16 surface_xrot;                 /* 0x70 */
    u16 surface_zrot;                 /* 0x72 */
    short character;                  /* 0x74 */
    short action;                     /* 0x76 */
    struct nuvec_s colbox[2];         /* 0x78 */
} CrateCube;

typedef struct {             /* 0x930 */
    struct nuvec_s origpos;           /* 0x0 */
    struct nuvec_s origvel;           /* 0xC */
    struct nuvec_s curpos;            /* 0x18 */
    struct nuvec_s curvel;            /* 0x24 */
    short id;                         /* 0x30 */
    short scanmode;                   /* 0x32 */
    float stopflag;                   /* 0x34 */
    float vellen;                     /* 0x38 */
    u8 * flags;                       /* 0x3C */
    float ax;                         /* 0x40 */
    float ay;                         /* 0x44 */
    float len;                        /* 0x48 */
    float size;                       /* 0x4C */
    float sizesq;                     /* 0x50 */
    float sizediv;                    /* 0x54 */
    float yscale;                     /* 0x58 */
    float yscalesq;                   /* 0x5C */
    float inyscale;                   /* 0x60 */
    float inyscalesq;                 /* 0x64 */
    short hitcnt;                     /* 0x68 */
    short hitterrno;                  /* 0x6A */
    float csx;                        /* 0x6C */
    float csy;                        /* 0x70 */
    float csz;                        /* 0x74 */
    float cex;                        /* 0x78 */
    float cey;                        /* 0x7C */
    float cez;                        /* 0x80 */
    short hittype;                    /* 0x84 */
    short plathit;                    /* 0x86 */
    short * PlatScanStart;            /* 0x88 */
    tertype * hitter;                 /* 0x8C */
    float hittime;                    /* 0x90 */
    float timeadj;                    /* 0x94 */
    float impactadj;                  /* 0x98 */
    struct nuvec_s hitnorm;           /* 0x9C */
    struct nuvec_s uhitnorm;          /* 0xA8 */
    struct nuvec_s tempvec[2];        /* 0xB4 */
    tertype rotter;                   /* 0xCC */
    tertype * hitdata[512];           /* 0x130 */
} TerTempInfoType;

typedef struct {             /* 0x81C */
    float x;                          /* 0x0 */
    float y;                          /* 0x4 */
    float z;                          /* 0x8 */
    float x2;                         /* 0xC */
    float y2;                         /* 0x10 */
    float z2;                         /* 0x14 */
    short timer;                      /* 0x18 */
    short pad;                        /* 0x1A */
    tertype * hitdata[512];           /* 0x1C */
} pollisttype;

typedef struct {             /* 0xA48 */
    int DmaHeader[4];                 /* 0x0 */
    float grav;                       /* 0x10 */
    float gtime;                      /* 0x14 */
    int DmaBody[4];                   /* 0x18 */
    float u0;                         /* 0x28 */
    float v0;                         /* 0x2C */
    float u1;                         /* 0x30 */
    float v1;                         /* 0x34 */
    float u2;                         /* 0x38 */
    float v2;                         /* 0x3C */
    float u3;                         /* 0x40 */
    float v3;                         /* 0x44 */
    PartList Data[64];                /* 0x48 */
} PartHeader;

struct NUSHADOWDATA_s {
    struct NUELLIPSOID_s *ellipsoids;
    void *cylinders;          /* NUCYLINDERS_s* */
    struct NUSHADOWMESH_s *shadow_meshes;
    u8 nellipsoids;
    u8 ncylinders;
    u8 nshadow_meshes;
    u8 joint;
};

typedef struct {             /* 0x76C */
    short time;                       /* 0x0 */
    short type;                       /* 0x2 */
    struct nuvec_s colbox[2];         /* 0x4 */
    BoxPolType BoxPol[36];            /* 0x1C */
} BoxExpType;

struct nuinstance_s {         /* 0x50 */
    struct numtx_s mtx;       /* 0x00 */
    int objid;                /* 0x40 */
    struct nuinstflags_s flags; /* 0x44 */
    struct nuinstanim_s *anim; /* 0x48 */
    short room_group;         /* 0x4C */
    char special_flag;        /* 0x4E */
    char pad[1];              /* 0x4F */
};

typedef struct {             /* 0x330C */
    terrsitu * terr;                  /* 0x0 */
    terrsitu * curlist;               /* 0x4 */
    short idlookup[128];              /* 0x8 */
    TERRPLATTYPE platdata[128];       /* 0x108 */
    void * wallinfo;                  /* 0x3308 */
} TERRPICKUPSET;

struct NUCUTSCENECAMERA_s {             /* 0x4 */
    struct nucamera_s * camera;       /* 0x0 */
};

struct nusyslight_s {            /* 0x64 */
    struct nulight_s light;      /* 0x00 */
    s32 index;                   /* 0x58 */
    s32 next;                    /* 0x5C */
    s32 last;                    /* 0x60 */
};

struct nulights_s {              /* 0x108 */
    struct nulight_s light[3];   /* 0x00 */
};

typedef struct {             /* 0x48 */
    int type;                         /* 0x0 */
    struct nuvec_s pos;               /* 0x4 */
    struct nuvec_s radius_pos;        /* 0x10 */
    float radius;                     /* 0x1C */
    u8 r;                             /* 0x20 */
    u8 g;                             /* 0x21 */
    u8 b;                             /* 0x22 */
    u8 glow;                          /* 0x23 */
    struct nucolour3_s colour;        /* 0x24 */
    struct nuvec_s direction;         /* 0x30 */
    u8 globalflag;                    /* 0x3C */
    u8 blendtype;                     /* 0x3D */
    u8 brightness;                    /* 0x3E */
    Glow_Light * glowlight;           /* 0x40 */
    u8 pad2;                          /* 0x44 */
} Ed_Light;

struct Nearest_Light_s {
    int AmbIndex;                /* 0x00 */
    struct nuvec_s AmbCol;       /* 0x04 */
    float ambientdist;           /* 0x10 */
    int CurLoopIndex;            /* 0x14 */
    struct pdir_s dir1;          /* 0x18 */
    struct pdir_s dir2;          /* 0x38 */
    struct pdir_s dir3;          /* 0x58 */
    struct pdir_s *pDir1st;      /* 0x78 */
    struct pdir_s *pDir2nd;      /* 0x7C */
    struct pdir_s *pDir3rd;      /* 0x80 */
    int glbambindex;             /* 0x84 */
    int negativeindex;           /* 0x88 */
    float negativedist;          /* 0x8C */
    struct pdir_s glbdirectional; /* 0x90 */
};

struct nudatfile_s {          /* 0x14 */
    struct nudathdr_s *ndh;   /* 0x00 */
    s32 start;                /* 0x04 */
    s32 len;                  /* 0x08 */
    s32 fix;                  /* 0x0C */
    s32 used;                 /* 0x10 */
};

struct nusystex_s {              /* 0x28 */
    struct nutex_s tex;          /* 0x00 */
    void *dds;                   /* 0x1C - platform texture ptr */
    int ref;                     /* 0x20 */
    int rtid;                    /* 0x24 */
};

struct nufaceongeom_s {       /* 0x1C */
    struct nufaceongeom_s *next; /* 0x00 */
    struct numtl_s *mtl;      /* 0x04 */
    int mtl_id;               /* 0x08 */
    int faceon_type;          /* 0x0C - nufaceontype_s */
    struct nufaceon_s *faceons; /* 0x10 */
    int nfaceons;             /* 0x14 */
    float faceon_radius;      /* 0x18 */
};

struct particledef_s {             /* 0x24 */
    int life;                         /* 0x0 */
    float w;                          /* 0x4 */
    float h;                          /* 0x8 */
    float uv[4];                      /* 0xC */
    int col;                          /* 0x1C */
    struct numtl_s * mtl;             /* 0x20 */
};

struct nugeom_s {             /* 0x30 */
    struct nugeom_s *next;    /* 0x00 */
    struct numtl_s *mtl;      /* 0x04 */
    s32 mtl_id;               /* 0x08 */
    int vtxtype;              /* 0x0C - nuvtxtype_e */
    s32 vtxcnt;               /* 0x10 */
    s32 vtxmax;               /* 0x14 */
    s32 hVB;                  /* 0x18 */
    void *basisvbptr;         /* 0x1C */
    struct nuprim_s *prim;    /* 0x20 */
    struct nuskin_s *skin;    /* 0x24 */
    struct NUVTXSKININFO_s *vtxskininfo; /* 0x28 */
    struct NUBLENDGEOM_s *blendgeom;     /* 0x2C */
};

struct nutexanimenv_s {          /* 0xEC */
    struct nutexanimprog_s *prog; /* 0x00 */
    int pc;                      /* 0x04 */
    int rep_count[16];           /* 0x08 */
    int rep_start[16];           /* 0x48 */
    int rep_ix;                  /* 0x88 */
    int ra[16];                  /* 0x8C */
    int ra_ix;                   /* 0xCC */
    int pause;                   /* 0xD0 */
    int pause_r;                 /* 0xD4 */
    int pause_cnt;               /* 0xD8 */
    struct numtl_s *mtl;         /* 0xDC */
    short *tids;                 /* 0xE0 */
    int tex_ix;                  /* 0xE4 */
    int dynalloc;                /* 0xE8 - bitfield: 1 bit */
};

struct HOVASTRUCT {             /* 0x134 */
    int ActiveMode;                   /* 0x0 */
    struct MYDRAW MainDraw;           /* 0x4 */
    struct nuvec_s Position;          /* 0xE4 */
    struct nuvec_s StartPos;          /* 0xF0 */
    struct nuvec_s Velocity;          /* 0xFC */
    float AngleY;                     /* 0x108 */
    int Thrust;                       /* 0x10C */
    float ThrustOff;                  /* 0x110 */
    int TerminalDive;                 /* 0x114 */
    int Explode;                      /* 0x118 */
    int HitPoints;                    /* 0x11C */
    float StayStillTimer;             /* 0x120 */
    float MoveTimer;                  /* 0x124 */
    struct nuvec_s TargetVelocity;    /* 0x128 */
};

struct TORNADOSTRUCT {             /* 0x110 */
    int Active;                       /* 0x0 */
    struct nuvec_s Position;          /* 0x4 */
    struct nuvec_s StartPosition;     /* 0x10 */
    struct MYDRAW MainDraw;           /* 0x1C */
    float Scale;                      /* 0xFC */
    float YAng;                       /* 0x100 */
    float YAngInc;                    /* 0x104 */
    float Rad;                        /* 0x108 */
    float RadInc;                     /* 0x10C */
};

struct FIREBOSSSTRUCT {             /* 0x688 */
    struct numtx_s Locator[16];       /* 0x0 */
    int DropSide;                     /* 0x400 */
    float FootTime;                   /* 0x404 */
    int HitPoints;                    /* 0x408 */
    int Pass;                         /* 0x40C */
    int Active;                       /* 0x410 */
    float AngleY;                     /* 0x414 */
    struct nuvec_s Position;          /* 0x418 */
    struct MYDRAW MainDraw;           /* 0x424 */
    struct MYDRAW ExplodeDraw;        /* 0x504 */
    struct MYSPLINE MainSpline;       /* 0x5E4 */
    int Seen;                         /* 0x614 */
    float Speed;                      /* 0x618 */
    int Action;                       /* 0x61C */
    int LastAction;                   /* 0x620 */
    float ActionTimer;                /* 0x624 */
    float ActionTimer2;               /* 0x628 */
    struct nuvec_s Vel;               /* 0x62C */
    int Dead;                         /* 0x638 */
    struct numtx_s DrawMtx;           /* 0x63C */
    int BeenHitThisPass;              /* 0x67C */
    float BeenHitThisPassTimer;       /* 0x680 */
    struct JEEPROCK * Rock;           /* 0x684 */
};

struct VEHMASK {             /* 0x174 */
    int Active;                       /* 0x0 */
    int Id;                           /* 0x4 */
    int KillAtEnd;                    /* 0x8 */
    struct nuvec_s Position;          /* 0xC */
    struct MYDRAW MainDraw;           /* 0x18 */
    float DrawAngY;                   /* 0xF8 */
    int Action;                       /* 0xFC */
    int LastAction;                   /* 0x100 */
    int EffectType;                   /* 0x104 */
    float Tween;                      /* 0x108 */
    float TweenInc;                   /* 0x10C */
    float Ang;                        /* 0x110 */
    struct nuvec_s Store[2];          /* 0x114 */
    struct nuvec_s * Point[2];        /* 0x12C */
    struct nuvec_s Offset[2];         /* 0x134 */
    float AngInc[2];                  /* 0x14C */
    float Rad[2];                     /* 0x154 */
    float Scale[2];                   /* 0x15C */
    float TiltX[2];                   /* 0x164 */
    float DrawScale;                  /* 0x16C */
    int Seen;                         /* 0x170 */
};

typedef struct {             /* 0x740 */
    struct nuvec_s Position;          /* 0x0 */
    MYDRAW Draw;                      /* 0xC */
    float PlayBackTime;               /* 0xEC */
    int PlayBackMax;                  /* 0xF0 */
    struct NUJOINTANIM_s Joints[8];   /* 0xF4 */
    struct numtx_s Locators[16];      /* 0x294 */
    int LocatorValidFrame;            /* 0x694 */
    SIMWHEEL TrailWheel[4];           /* 0x698 */
    float TimeLine;                   /* 0x738 */
    char Active;                      /* 0x73C */
    char DrawOn;                      /* 0x73D */
    char TrailOn;                     /* 0x73E */
    char Pad;                         /* 0x73F */
} ENEMYJEEPSTRUCT;

struct ZOFFASTRUCT {             /* 0x5B0 */
    struct MYDRAW MainDraw;           /* 0x0 */
    int ActiveMode;                   /* 0xE0 */
    float RespawnTimer;               /* 0xE4 */
    struct nuvec_s Position;          /* 0xE8 */
    struct nuvec_s Velocity;          /* 0xF4 */
    struct nuvec_s Resolved;          /* 0x100 */
    float TiltX;                      /* 0x10C */
    float TiltZ;                      /* 0x110 */
    float DestTiltX;                  /* 0x114 */
    float DestTiltZ;                  /* 0x118 */
    float DestAngleY;                 /* 0x11C */
    float AngleY;                     /* 0x120 */
    int NoFireSound;                  /* 0x124 */
    float Temp[6];                    /* 0x128 */
    float VisibleTimer;               /* 0x140 */
    float NewDirectionTimer;          /* 0x144 */
    float NewAltitudeTimer;           /* 0x148 */
    float NewAltitudeTarget;          /* 0x14C */
    int TerminalDive;                 /* 0x150 */
    int Explode;                      /* 0x154 */
    int HitPoints;                    /* 0x158 */
    int FireFrame;                    /* 0x15C */
    int FireNow;                      /* 0x160 */
    float FireTimer;                  /* 0x164 */
    int Seen;                         /* 0x168 */
    int SmkTimer;                     /* 0x16C */
    struct numtx_s Locators[16];      /* 0x170 */
    int SmokeCounter;                 /* 0x570 */
    float AggressionTimer;            /* 0x574 */
    int PlayerCloseAndVisable;        /* 0x578 */
    float NotSeenTimer;               /* 0x57C */
    float AggressionPoints;           /* 0x580 */
    int Teleport;                     /* 0x584 */
    float NoTeleportTimer;            /* 0x588 */
    float Speed;                      /* 0x58C */
    int InFront;                      /* 0x590 */
    int FacingTarget;                 /* 0x594 */
    float Dist;                       /* 0x598 */
    int FireSide;                     /* 0x59C */
    float NotInFrontTimer;            /* 0x5A0 */
    float LockedOnTimer;              /* 0x5A4 */
    float KeepSameVelocityTimer;      /* 0x5A8 */
    float FireBurstTimer;             /* 0x5AC */
};

struct GUNBOATSTRUCT {             /* 0x134 */
    int Active;                       /* 0x0 */
    int Character;                    /* 0x4 */
    struct MYDRAW MainDraw;           /* 0x8 */
    struct nuvec_s Position;          /* 0xE8 */
    float AngleY;                     /* 0xF4 */
    float FireAngleX;                 /* 0xF8 */
    float FireAngleY;                 /* 0xFC */
    float TiltX;                      /* 0x100 */
    float TiltZ;                      /* 0x104 */
    float DestAngleY;                 /* 0x108 */
    float DestTiltX;                  /* 0x10C */
    float DestTiltZ;                  /* 0x110 */
    int FireFrame;                    /* 0x114 */
    int Seen;                         /* 0x118 */
    int Action;                       /* 0x11C */
    int LastAction;                   /* 0x120 */
    float FireAngMomX;                /* 0x124 */
    float FireAngMomY;                /* 0x128 */
    float BurstTimer;                 /* 0x12C */
    float SunkTimer;                  /* 0x130 */
};

struct BOSSSTRUCT {             /* 0x65C */
    int Active;                       /* 0x0 */
    int Unleashed;                    /* 0x4 */
    float FireTimer;                  /* 0x8 */
    int FireSide;                     /* 0xC */
    struct MYDRAW MainDraw;           /* 0x10 */
    struct MYDRAW BonesDraw;          /* 0xF0 */
    struct numtx_s Locator[16];       /* 0x1D0 */
    struct nuvec_s Position;          /* 0x5D0 */
    float PossYDest;                  /* 0x5DC */
    float AngleY;                     /* 0x5E0 */
    float BaseAngleY;                 /* 0x5E4 */
    float DestAngleY;                 /* 0x5E8 */
    int HitPoints[4];                 /* 0x5EC */
    int Seen;                         /* 0x5FC */
    float Distance;                   /* 0x600 */
    float DistanceDest;               /* 0x604 */
    int Action;                       /* 0x608 */
    int OldAction;                    /* 0x60C */
    int NextAction;                   /* 0x610 */
    float ActionTimer;                /* 0x614 */
    int LastNonSeekAction;            /* 0x618 */
    int Dead;                         /* 0x61C */
    struct MYSPLINE MainSpline;       /* 0x620 */
    float HoldFrameTimer;             /* 0x650 */
    float ChestSoundBTimer;           /* 0x654 */
    int HitSoundFrame;                /* 0x658 */
};

struct PLANESTRUCT {             /* 0x104 */
    struct nuvec_s Pos;               /* 0x0 */
    struct nuvec_s Vel;               /* 0xC */
    float ActionTimer;                /* 0x18 */
    int ActionStatus;                 /* 0x1C */
    int Active;                       /* 0x20 */
    struct MYDRAW MainDraw;           /* 0x24 */
};

struct BIGGUNSTRUCT {             /* 0x154 */
    int Active;                       /* 0x0 */
    struct MYDRAW MainDraw;           /* 0x4 */
    struct nuvec_s Position;          /* 0xE4 */
    int Type;                         /* 0xF0 */
    int HitPoints;                    /* 0xF4 */
    float AngleY;                     /* 0xF8 */
    float FireAngleX;                 /* 0xFC */
    float FireAngleY;                 /* 0x100 */
    float TiltX;                      /* 0x104 */
    float TiltZ;                      /* 0x108 */
    float DestAngleY;                 /* 0x10C */
    float DestTiltX;                  /* 0x110 */
    float DestTiltZ;                  /* 0x114 */
    int FireFrame;                    /* 0x118 */
    int Seen;                         /* 0x11C */
    int Action;                       /* 0x120 */
    int LastAction;                   /* 0x124 */
    float FireAngMomX;                /* 0x128 */
    float FireAngMomY;                /* 0x12C */
    float BurstTimer;                 /* 0x130 */
    int ExplosionEffect;              /* 0x134 */
    float FireAngMinX;                /* 0x138 */
    float FireAngMaxX;                /* 0x13C */
    float FireAngMainY;               /* 0x140 */
    float FireAngDeviationY;          /* 0x144 */
    struct nuvec_s TerrPos;           /* 0x148 */
};

struct BATTLESHIPSTRUCT {             /* 0x554 */
    int Active;                       /* 0x0 */
    struct MYDRAW MainDraw;           /* 0x4 */
    struct numtx_s Locator[16];       /* 0xE4 */
    struct nuvec_s Position;          /* 0x4E4 */
    float AngleY;                     /* 0x4F0 */
    float TiltX;                      /* 0x4F4 */
    float TiltZ;                      /* 0x4F8 */
    float DestTiltX;                  /* 0x4FC */
    float DestTiltZ;                  /* 0x500 */
    float DestY;                      /* 0x504 */
    float Seek;                       /* 0x508 */
    int Seen;                         /* 0x50C */
    int HitPoints;                    /* 0x510 */
    float FireTimer[2];               /* 0x514 */
    struct nuvec_s GooScale;          /* 0x51C */
    struct nuvec_s DestGooScale;      /* 0x528 */
    struct nuvec_s BaseGooScale;      /* 0x534 */
    float GooSpeed;                   /* 0x540 */
    struct nuvec_s GooTimer;          /* 0x544 */
    int KillMeNow;                    /* 0x550 */
};

struct memexternal_s {           /* 0x08 */
    union variptr_u *ptr;        /* 0x00 */
    union variptr_u *end;        /* 0x04 */
};

struct NUCOLLISIONDATA_s {       /* 0x08 */
    u32 nitems;                  /* 0x00 */
    struct NUCOLLISIONHDR_s *hdrs; /* 0x04 */
};

struct _LIGHTLIST {              /* 0x6C */
    int EnableLight;             /* 0x00 */
    struct _D3DLIGHT8 Light;     /* 0x04 */
};

struct NUTRIGGERSYS_s {             /* 0x10 */
    int version;                      /* 0x0 */
    int address_offset;               /* 0x4 */
    int ntriggers;                    /* 0x8 */
    struct NUTRIGGER_s * triggers;    /* 0xC */
};

struct NUGCUTTRIGGERSYS_s {             /* 0x8 */
    int ntriggers;                    /* 0x0 */
    struct NUGCUTTRIGGER_s * triggers; /* 0x4 */
};

struct debkeydatatype_s {             /* 0x36C */
    uv1debdata * chunks[128];         /* 0x0 */
    struct numtx_s rotmtx;            /* 0x200 */
    struct numtx_s emitrotmtx;        /* 0x240 */
    short count;                      /* 0x280 */
    short type;                       /* 0x282 */
    short active;                     /* 0x284 */
    short debcount;                   /* 0x286 */
    short disposed;                   /* 0x288 */
    short pointer;                    /* 0x28A */
    short reqcount;                   /* 0x28C */
    short reqdebcount;                /* 0x28E */
    float x;                          /* 0x290 */
    float y;                          /* 0x294 */
    float z;                          /* 0x298 */
    float toffx;                      /* 0x29C */
    float toffy;                      /* 0x2A0 */
    float toffz;                      /* 0x2A4 */
    struct debkeydatatype_s * next;   /* 0x2B0 */
    struct debkeydatatype_s * prev;   /* 0x2B4 */
    short rotory;                     /* 0x2B8 */
    short rotorz;                     /* 0x2BA */
    int instances;                    /* 0x2BC */
    short oncount;                    /* 0x2C0 */
    short delay;                      /* 0x2C2 */
    struct debrissphere_s spheres[8]; /* 0x2C4 */
    short sphere_next;                /* 0x344 */
    short sphere_next_emit;           /* 0x346 */
    int trigger_type;                 /* 0x348 */
    int trigger_id;                   /* 0x34C */
    float trigger_var;                /* 0x350 */
    short refrotz;                    /* 0x354 */
    short refroty;                    /* 0x356 */
    float refoff;                     /* 0x358 */
    float refbounce;                  /* 0x35C */
    short sound_next[4];              /* 0x360 */
    short group_id;                   /* 0x368 */
};

typedef struct {             /* 0x8 */
    CrateCube * crate;                /* 0x0 */
    s8 type1;                         /* 0x4 */
    s8 type2;                         /* 0x5 */
    s8 type3;                         /* 0x6 */
    s8 type4;                         /* 0x7 */
} CRATETYPEDATA;

typedef struct {             /* 0xAF50 */
    terrsitu * terr;                  /* 0x0 */
    TERRPLATTYPE platdata[128];       /* 0x4 */
    void * wallinfo;                  /* 0x3204 */
    TERRINFO TrackInfo[4];            /* 0x3208 */
    terrgrouptype terrgroup[257];     /* 0x3238 */
    int terrainlow;                   /* 0x464C */
    int terrgcnt;                     /* 0x4650 */
    short terrlist[4096];             /* 0x4654 */
    pollisttype pollist[9];           /* 0x6654 */
} TERRSET;

typedef struct {             /* 0x354 */
    char id[16];                      /* 0x0 */
    char listid;                      /* 0x10 */
    short generate;                   /* 0x12 */
    short debnum;                     /* 0x14 */
    short ival_offset;                /* 0x16 */
    short ival_on;                    /* 0x18 */
    short ival_on_ran;                /* 0x1A */
    short ival_off;                   /* 0x1C */
    short ival_off_ran;               /* 0x1E */
    char gensort;                     /* 0x20 */
    char gencode;                     /* 0x21 */
    char mat;                         /* 0x22 */
    char efftab;                      /* 0x23 */
    float radius;                     /* 0x24 */
    float cuton;                      /* 0x28 */
    float cutoff;                     /* 0x2C */
    float drawcutoff;                 /* 0x30 */
    float emitmag;                    /* 0x34 */
    struct nuvec_s variable_emit;     /* 0x38 */
    struct nuvec_s variable_emit_ranscale; /* 0x44 */
    struct nuvec_s variable_start;    /* 0x50 */
    struct nuvec_s variable_start_ranscale; /* 0x5C */
    float rsx;                        /* 0x68 */
    float rsy;                        /* 0x6C */
    float rsz;                        /* 0x70 */
    float osx;                        /* 0x74 */
    float osy;                        /* 0x78 */
    float osz;                        /* 0x7C */
    float rvx;                        /* 0x80 */
    float rvy;                        /* 0x84 */
    float rvz;                        /* 0x88 */
    float ovx;                        /* 0x8C */
    float ovy;                        /* 0x90 */
    float ovz;                        /* 0x94 */
    float grav;                       /* 0x98 */
    float etime;                      /* 0x9C */
    short uw;                         /* 0xA0 */
    char frames;                      /* 0xA2 */
    char rate;                        /* 0xA3 */
    float uvanim;                     /* 0xA4 */
    float jibxfreq;                   /* 0xA8 */
    float jibxamp;                    /* 0xAC */
    float jibyfreq;                   /* 0xB0 */
    float jibyamp;                    /* 0xB4 */
    DEBRIS_COLOUR_SLOT colslot[8];    /* 0xB8 */
    DEBRIS_ALPHA_SLOT alpslot[8];     /* 0x138 */
    float minsize;                    /* 0x178 */
    float maxsize;                    /* 0x17C */
    DEBRIS_WIDTH_SLOT wslot[8];       /* 0x180 */
    DEBRIS_HEIGHT_SLOT hslot[8];      /* 0x1C0 */
    float minrot;                     /* 0x200 */
    float maxrot;                     /* 0x204 */
    DEBRIS_ROTATION_SLOT rotslot[8];  /* 0x208 */
    DEBRIS_JIBBER_X_SLOT jibxslot[8]; /* 0x248 */
    DEBRIS_JIBBER_Y_SLOT jibyslot[8]; /* 0x288 */
    float u1;                         /* 0x2C8 */
    float v1;                         /* 0x2CC */
    float u2;                         /* 0x2D0 */
    float v2;                         /* 0x2D4 */
    PartHeader * DmaDebTypePointer;   /* 0x2D8 */
    DEBRIS_COLLISION_SPHERE_SLOT sphereslot[8]; /* 0x2DC */
    char numspheres;                  /* 0x31C */
    int variable_key;                 /* 0x320 */
    DEBRIS_SOUND sounds[4];           /* 0x324 */
} debinftype;

typedef struct {             /* 0xB4C */
    struct numtx_s mtx[24];           /* 0x480 */
    struct nuinstance_s * instance[24]; /* 0xA80 */
    struct nuinstance_s * ipost;      /* 0xAE0 */
    struct nuvec_s center;            /* 0xAE4 */
    float radius;                     /* 0xAF0 */
    short plat[24];                   /* 0xAF4 */
    char inrange;                     /* 0xB24 */
    char onscreen;                    /* 0xB25 */
    char sections;                    /* 0xB26 */
    char postint;                     /* 0xB27 */
    int colour;                       /* 0xB28 */
    short hit;                        /* 0xB2C */
    short yang;                       /* 0xB2E */
    float width;                      /* 0xB30 */
    float tension;                    /* 0xB34 */
    float gravity;                    /* 0xB38 */
    float damp;                       /* 0xB3C */
    float plrweight;                  /* 0xB40 */
    float postw;                      /* 0xB44 */
    float posth;                      /* 0xB48 */
} BridgeType;

typedef struct {             /* 0x28 */
    struct nuinstance_s * instance;   /* 0x0 */
    struct numtx_s * mtx;             /* 0x4 */
    struct nuvec_s center;            /* 0x8 */
    char onscreen;                    /* 0x14 */
    char inrange;                     /* 0x15 */
    short objcount;                   /* 0x16 */
    int collide;                      /* 0x18 */
    float wind;                       /* 0x1C */
    float height;                     /* 0x20 */
    float radius;                     /* 0x24 */
} NuWindGType;

struct visidat_s {             /* 0xC */
    struct nugspline_s * sp;          /* 0x0 */
    int numinstances;                 /* 0x4 */
    struct nuinstance_s * i[1];       /* 0x8 */
};

struct nuspecial_s {          /* 0x50 */
    struct numtx_s mtx;       /* 0x00 */
    struct nuinstance_s *instance; /* 0x40 */
    char *name;               /* 0x44 */
    struct nuspecialflags flags; /* 0x48 */
    int pad;                  /* 0x4C */
};

struct mask {
    struct numtx_s mM;           /* 0x000 */
    struct numtx_s mS;           /* 0x040 */
    struct nuvec_s pos;          /* 0x080 */
    struct nuvec_s newpos;       /* 0x08C */
    struct Nearest_Light_s lights; /* 0x098 */
    struct anim_s anim;          /* 0x148 */
    float scale;                 /* 0x164 */
    float shadow;                /* 0x168 */
    short character;             /* 0x16C */
    short active;                /* 0x16E */
    short sfx;                   /* 0x170 */
    char pad1;                   /* 0x172 */
    char pad2;                   /* 0x173 */
    u16 xrot;                    /* 0x174 */
    u16 yrot;                    /* 0x176 */
    u16 angle;                   /* 0x178 */
    u16 surface_xrot;            /* 0x17A */
    u16 surface_zrot;            /* 0x17C */
    u16 wibble_ang[3];           /* 0x17E */
    float idle_time;             /* 0x184 */
    float idle_duration;         /* 0x188 */
    s8 idle_mode;                /* 0x18C */
    s8 reflect;                  /* 0x18D */
    u8 offset_frame;             /* 0x18E */
    s8 hold;                     /* 0x18F */
};

typedef struct {             /* 0x190 */
    struct numtx_s mM;                /* 0x0 */
    struct numtx_s mS;                /* 0x40 */
    struct nuvec_s pos;               /* 0x80 */
    struct nuvec_s newpos;            /* 0x8C */
    Nearest_Light_s lights;           /* 0x98 */
    struct anim_s anim;               /* 0x148 */
    float scale;                      /* 0x164 */
    float shadow;                     /* 0x168 */
    short character;                  /* 0x16C */
    short active;                     /* 0x16E */
    short sfx;                        /* 0x170 */
    char pad1;                        /* 0x172 */
    char pad2;                        /* 0x173 */
    u16 xrot;                         /* 0x174 */
    u16 yrot;                         /* 0x176 */
    u16 angle;                        /* 0x178 */
    u16 surface_xrot;                 /* 0x17A */
    u16 surface_zrot;                 /* 0x17C */
    u16 wibble_ang[3];                /* 0x17E */
    float idle_time;                  /* 0x184 */
    float idle_duration;              /* 0x188 */
    s8 idle_mode;                     /* 0x18C */
    s8 reflect;                       /* 0x18D */
    u8 offset_frame;                  /* 0x18E */
    s8 hold;                          /* 0x18F */
} MaskInfo;

struct nugeomitem_s {         /* 0x24 */
    struct nurndritem_s hdr;  /* 0x00 */
    struct numtx_s *mtx;      /* 0x10 */
    struct nugeom_s *geom;    /* 0x14 */
    f32 **blendvals;          /* 0x18 */
    u16 instancelights_index[3]; /* 0x1C */
    u16 hShader;              /* 0x22 */
};

struct nugobj_s {             /* 0x64 */
    struct nugobj_s *sysnext; /* 0x00 */
    struct nugobj_s *syslast; /* 0x04 */
    int type;                 /* 0x08 - gobjtype_s */
    struct nugeom_s *geom;    /* 0x0C */
    struct nufaceongeom_s *faceon_geom; /* 0x10 */
    float bounding_radius_from_origin;  /* 0x14 */
    float bounding_rsq_from_origin;     /* 0x18 */
    struct nuvec_s bounding_box_min;    /* 0x1C */
    struct nuvec_s bounding_box_max;    /* 0x28 */
    struct nuvec_s bounding_box_center; /* 0x34 */
    float bounding_radius_from_center;  /* 0x40 */
    float bounding_rsq_from_center;     /* 0x44 */
    int ngobjs;               /* 0x48 */
    struct nugobj_s *next_gobj; /* 0x4C */
    struct nuvec_s origin;    /* 0x50 */
    int ignore;               /* 0x5C */
    int culltype;             /* 0x60 */
};

struct nutexanim_s {             /* 0x20 */
    struct nutexanim_s *succ;    /* 0x00 */
    struct nutexanim_s *prev;    /* 0x04 */
    short *tids;                 /* 0x08 */
    short numtids;               /* 0x0C */
    short dynalloc;              /* 0x0E - bitfield: 1 bit */
    struct numtl_s *mtl;         /* 0x10 */
    struct nutexanimenv_s *env;  /* 0x14 */
    char *ntaname;               /* 0x18 */
    char *scriptname;            /* 0x1C */
};

struct instNUTRIGGERSYS_s {             /* 0x18 */
    struct instNUTRIGGERSYS_s * next; /* 0x0 */
    struct instNUTRIGGERSYS_s * prev; /* 0x4 */
    struct NUTRIGGERSYS_s * triggersys; /* 0x8 */
    struct instNUTRIGGER_s * itriggers; /* 0xC */
    void * scenemanager;              /* 0x10 */
    int is_disabled;                  /* 0x14 */
};

struct debris_chunk_control_s {             /* 0x2C */
    int delay;                        /* 0x4 */
    enum debris_chunk_control_e action; /* 0x8 */
    struct debkeydatatype_s * owner;  /* 0xC */
    struct debris_chunk_control_s * next; /* 0x10 */
    int ivariable;                    /* 0x14 */
    float fvariable;                  /* 0x18 */
    short type;                       /* 0x1C */
    short refrotz;                    /* 0x1E */
    short refroty;                    /* 0x20 */
    float refoff;                     /* 0x24 */
    float refbounce;                  /* 0x28 */
};

typedef struct {             /* 0x58 */
    uv1debdata * chunk;               /* 0x0 */
    debinftype * debinfo;             /* 0x4 */
    struct debkeydatatype_s * debdata; /* 0x8 */
    struct numtx_s rotmtx;            /* 0xC */
    float x;                          /* 0x4C */
    float y;                          /* 0x50 */
    float z;                          /* 0x54 */
} PARTICLE_CHUNK_TO_RENDER_TYPE;

struct nuanimcurvedata_s {       /* 0x0C */
    u32 *mask;                   /* 0x00 */
    u16 *key_ixs;                /* 0x04 */
    void *key_array;             /* 0x08 */
};

struct nuanimcurve_s {           /* 0x10 */
    u32 mask;                    /* 0x00 */
    struct nuanimkey_s *animkeys; /* 0x04 */
    u32 numkeys;                 /* 0x08 */
    u32 flags;                   /* 0x0C */
};

typedef struct {             /* 0x414 */
    char name[9];                     /* 0x0 */
    u8 vibration;                     /* 0x9 */
    u8 surround;                      /* 0xA */
    u8 sfx_volume;                    /* 0xB */
    u8 music_volume;                  /* 0xC */
    s8 screen_x;                      /* 0xD */
    s8 screen_y;                      /* 0xE */
    u8 language;                      /* 0xF */
    HubState hub[6];                  /* 0x10 */
    LevelState level[35];             /* 0x28 */
    u8 lives;                         /* 0x3FC */
    u8 wumpas;                        /* 0x3FD */
    u8 mask;                          /* 0x3FE */
    u8 percent;                       /* 0x3FF */
    u8 crystals;                      /* 0x400 */
    u8 relics;                        /* 0x401 */
    u8 crate_gems;                    /* 0x402 */
    u8 bonus_gems;                    /* 0x403 */
    u8 gems;                          /* 0x404 */
    u8 gembits;                       /* 0x405 */
    u8 powerbits;                     /* 0x406 */
    u8 empty;                         /* 0x407 */
    u32 cutbits;                      /* 0x408 */
    u8 year;                          /* 0x40C */
    u8 month;                         /* 0x40D */
    u8 day;                           /* 0x40E */
    u8 hours;                         /* 0x40F */
    u8 mins;                          /* 0x410 */
} GameState;

struct game_s {                  /* 0x414 */
    char name[9];                /* 0x000 */
    u8 vibration;                /* 0x009 */
    u8 surround;                 /* 0x00A */
    u8 sfx_volume;               /* 0x00B */
    u8 music_volume;             /* 0x00C */
    char screen_x;               /* 0x00D */
    char screen_y;               /* 0x00E */
    u8 language;                 /* 0x00F */
    struct hub_s hub[6];         /* 0x010 */
    struct level_s level[35];    /* 0x028 */
    u8 lives;                    /* 0x400 */
    u8 wumpas;                   /* 0x401 */
    u8 mask;                     /* 0x402 */
    u8 percent;                  /* 0x403 */
    u8 crystals;                 /* 0x404 */
    u8 relics;                   /* 0x405 */
    u8 crate_gems;               /* 0x406 */
    u8 bonus_gems;               /* 0x407 */
    u8 gems;                     /* 0x408 */
    u8 gembits;                  /* 0x409 */
    u8 powerbits;                /* 0x40A */
    u8 empty;                    /* 0x40B */
    u32 cutbits;                 /* 0x40C */
    u8 year;                     /* 0x410 */
    u8 month;                    /* 0x411 */
    u8 day;                      /* 0x412 */
    u8 hours;                    /* 0x413 */
    u8 mins;
    u8 pad_[3];
};

struct nusysmtl_s {           /* 0x6C */
    struct numtl_s mtl;       /* 0x00 */
    struct nurndritem_s *rndrlist; /* 0x54 */
    struct nugeomitem_s *geom2d;  /* 0x58 */
    struct nugeomitem_s *geom3d;  /* 0x5C */
    struct nusysmtl_s *next;  /* 0x60 */
    struct nusysmtl_s *last;  /* 0x64 */
    s32 hShader;              /* 0x68 */
};

struct NUSKINNODE_s {             /* 0x34 */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    struct nugobj_s * mesh;           /* 0x20 */
    int gid;                          /* 0x24 */
    int njoints;                      /* 0x28 */
    struct NUJOINTNODE_s * * joints;  /* 0x2C */
    struct numtx_s * jointmtx;        /* 0x30 */
};

struct NUMESHNODE_s {             /* 0x2C */
    char * name;                      /* 0x0 */
    u32 id;                           /* 0x4 */
    u32 type;                         /* 0x8 */
    int nchildren;                    /* 0xC */
    struct NUNODE_s * parent;         /* 0x10 */
    struct NUNODE_s * child;          /* 0x14 */
    struct NUNODE_s * next;           /* 0x18 */
    void * data;                      /* 0x1C */
    struct nugobj_s * mesh;           /* 0x20 */
    int gid;                          /* 0x24 */
    int mesh_flags;                   /* 0x28 */
};

struct nugscn_s {             /* 0x74 */
    short *tids;              /* 0x00 */
    int numtid;               /* 0x04 */
    struct numtl_s **mtls;    /* 0x08 */
    int nummtl;               /* 0x0C */
    int numgobj;              /* 0x10 */
    struct nugobj_s **gobjs;  /* 0x14 */
    int numinstance;          /* 0x18 */
    struct nuinstance_s *instances; /* 0x1C */
    int numspecial;           /* 0x20 */
    struct nuspecial_s *specials; /* 0x24 */
    void *splinedata;         /* 0x28 */
    int numsplines;           /* 0x2C */
    struct nugspline_s *splines; /* 0x30 */
    char *nametable;          /* 0x34 */
    int numexspecials;        /* 0x38 */
    struct nuspecial_s *exspecials; /* 0x3C */
    int exallocix;            /* 0x40 */
    struct nuinstanim_s *instanimblock; /* 0x44 */
    void **instanimdata;      /* 0x48 */
    int numinstanims;         /* 0x4C */
    int numinstanceixs;       /* 0x50 */
    short *instanceixs;       /* 0x54 */
    short crossfade;          /* 0x58 */
    short crossfaderate;      /* 0x5A */
    int crossfadegobj;        /* 0x5C */
    int crossfadefirst;       /* 0x60 */
    int numtexanims;          /* 0x64 */
    void *texanims;           /* 0x68 */
    short *texanim_tids;      /* 0x6C */
    short *instancelightix;   /* 0x70 */
};

struct NULAYERDATA_s {
    char *name;
    struct nugobj_s **gobjs;
    struct nugobj_s *skin_gobj;
    struct nugobj_s **blend_gobjs;
    struct nugobj_s *blend_skin_gobj;
};

struct nutexanimlist_s {         /* 0x0C */
    struct nutexanim_s *nta;     /* 0x00 */
    struct nutexanimlist_s *succ; /* 0x04 */
    struct nutexanimlist_s *prev; /* 0x08 */
};

struct nutexanimf_s {            /* 0x20 - file version */
    struct nutexanim_s *succ;    /* 0x00 */
    struct nutexanim_s *prev;    /* 0x04 */
    int tids;                    /* 0x08 */
    short numtids;               /* 0x0C */
    short dynalloc;              /* 0x0E - bitfield: 1 bit */
    int mtl;                     /* 0x10 */
    struct nutexanimenv_s *env;  /* 0x14 */
    int ntaname;                 /* 0x18 */
    int scriptname;              /* 0x1C */
};

struct instNUGCUTTRIGGERSYS_s {             /* 0x8 */
    struct instNUTRIGGERSYS_s * itriggersys; /* 0x0 */
    struct instNUGCUTTRIGGER_s * itriggers; /* 0x4 */
};

struct nuanimcurve2_s {          /* 0x04 */
    union {
        float constant;          /* 0x00 */
        struct nuanimcurvedata_s *curvedata; /* 0x00 */
    } data;
};

struct nuanimcurveset_s {        /* 0x10 */
    s32 flags;                   /* 0x00 */
    f32 *constants;              /* 0x04 */
    struct nuanimcurve_s **set;  /* 0x08 */
    char ncurves;                /* 0x0C */
    char pad[3];                 /* 0x0D */
};

struct nuotitem_s {              /* 0x10 */
    struct nuotitem_s *next;     /* 0x00 */
    struct nurndritem_s *hdr;    /* 0x04 */
    float dist;                  /* 0x08 */
    struct nusysmtl_s *mtl;      /* 0x0C */
};

struct nustenitem_s {            /* 0x10 */
    struct nustenitem_s *next;   /* 0x00 */
    struct nurndritem_s *hdr;    /* 0x04 */
    struct nusysmtl_s *mtl;      /* 0x08 */
    s32 type;                    /* 0x0C */
};

struct nuwateritem_s {           /* 0x0C */
    struct nuwateritem_s *next;  /* 0x00 */
    struct nurndritem_s *hdr;    /* 0x04 */
    struct nusysmtl_s *mtl;      /* 0x08 */
};

struct nufaceonitem_s {          /* 0x0C */
    struct nufaceonitem_s *next; /* 0x00 */
    struct nurndritem_s *hdr;    /* 0x04 */
    struct nusysmtl_s *mtl;      /* 0x08 */
};

union nunodeptr_s {
    struct NUNODE_s * node;           /* 0x0 */
    struct NUMESHNODE_s * mesh;       /* 0x0 */
    struct NUTRANSFORMNODE_s * transform; /* 0x0 */
    struct NUJOINTNODE_s * joint;     /* 0x0 */
    struct NUSKINNODE_s * skin;       /* 0x0 */
    struct NUREFERENCENODE_s * reference; /* 0x0 */
    struct NUSPLINENODE_s * spline;   /* 0x0 */
};

struct nuscene_s {            /* 0x3C */
    int nnodes;               /* 0x00 */
    char **names;             /* 0x04 */
    struct NUNODE_s **nodes;  /* 0x08 */
    char *strings;            /* 0x0C */
    int numtids;              /* 0x10 */
    short *tids;              /* 0x14 */
    int nummtls;              /* 0x18 */
    struct numtl_s **mtls;    /* 0x1C */
    int numgobjs;             /* 0x20 */
    struct nugobj_s **gobjs;  /* 0x24 */
    int numsplines;           /* 0x28 */
    struct nuspline_s *splines; /* 0x2C */
    struct nuvec_s *spline_cvs; /* 0x30 */
    struct NUNODE_s *root_node; /* 0x34 */
    struct nugscn_s *gscene;  /* 0x38 */
};

struct visidata_s {
    struct nugscn_s *sc;         /* 0x00 */
    void ***vspline;             /* 0x04 */
    int binfosize;               /* 0x08 */
    int curspline;               /* 0x0C */
    int curknot;                 /* 0x10 */
};

struct nuhspecial_s {            /* 0x08 */
    struct nugscn_s *scene;      /* 0x00 */
    struct nuspecial_s *special; /* 0x04 */
};

struct scene_inst_s {            /* 0x08 */
    char *name;                  /* 0x00 */
    struct nugscn_s *scene;      /* 0x04 */
};

struct NUHGOBJ_s {               /* 0x80 */
    short *tids;                 /* 0x00 */
    int numtid;                  /* 0x04 */
    struct numtl_s **mtls;       /* 0x08 */
    int nummtl;                  /* 0x0C */
    struct NUJOINTDATA_s *joints; /* 0x10 */
    struct numtx_s *T;           /* 0x14 */
    struct numtx_s *INV_WT;      /* 0x18 */
    u8 *joint_ixs;               /* 0x1C */
    struct NULAYERDATA_s *layers; /* 0x20 */
    struct NUPOINTOFINTEREST_s *points_of_interest; /* 0x24 */
    u8 *poi_ixs;                 /* 0x28 */
    char *string_table;          /* 0x2C */
    int string_table_size;       /* 0x30 */
    float tbrdist;               /* 0x34 */
    struct NUSHADOWDATA_s *shadow_data; /* 0x38 */
    float sphere_radius;         /* 0x3C */
    float sphere_yoff;           /* 0x40 */
    struct nuvec_s min;          /* 0x44 */
    struct nuvec_s max;          /* 0x50 */
    float cylinder_yoff;         /* 0x5C */
    float cylinder_height;       /* 0x60 */
    float cylinder_radius;       /* 0x64 */
    struct NUCOLLISIONDATA_s *collision_data; /* 0x68 */
    int numtexanims;             /* 0x6C */
    struct nutexanim_s *texanims; /* 0x70 */
    short *texanim_tids;         /* 0x74 */
    u8 num_joints;               /* 0x78 */
    u8 num_joint_ixs;            /* 0x79 */
    u8 num_layers;               /* 0x7A */
    u8 num_points_of_interest;   /* 0x7B */
    u8 num_poi_ixs;              /* 0x7C */
    u8 shadowoff;                /* 0x7D */
    char pad[2];                 /* 0x7E */
};

struct nuanimdata2_s {           /* 0x18 */
    f32 endframe;                /* 0x00 */
    s16 nnodes;                  /* 0x04 */
    s16 ncurves;                 /* 0x06 */
    s16 nchunks;                 /* 0x08 */
    s16 pad;                     /* 0x0A */
    struct nuanimcurve2_s *curves; /* 0x0C */
    char *curveflags;            /* 0x10 */
    char *curvesetflags;         /* 0x14 */
};

struct nuanimdatachunk_s {       /* 0x14 */
    int numnodes;                /* 0x00 */
    int num_valid_animcurvesets; /* 0x04 */
    struct nuanimcurveset_s **animcurvesets; /* 0x08 */
    struct nuanimkey_s *keys;    /* 0x0C */
    struct nuanimcurve_s *curves; /* 0x10 */
};

struct sceneinst_s {             /* 0x108 */
    struct nuscene_s *scene;     /* 0x00 */
    char name[256];              /* 0x04 */
    int inst_cnt;                /* 0x104 */
};

typedef struct {             /* 0x20 */
    struct nuhspecial_s obj;          /* 0x0 */
    struct nugscn_s * * scene;        /* 0x8 */
    s8 visible;                       /* 0xC */
    char font3d_letter;               /* 0xD */
    char pad1;                        /* 0xE */
    char pad2;                        /* 0xF */
    char * name;                      /* 0x10 */
    u64 levbits;                      /* 0x18 */
} LevelObject;

struct crate_type_s {             /* 0x14 */
    struct nuhspecial_s obj;          /* 0x0 */
    int id;                           /* 0x8 */
    char * name;                      /* 0xC */
    int character;                    /* 0x10 */
};

typedef struct {             /* 0x98 */
    struct nugspline_s * spl;         /* 0x0 */
    struct nuhspecial_s obj[16];      /* 0x4 */
    struct nuvec_s pos;               /* 0x84 */
    s8 played;                        /* 0x90 */
    s8 iRAIL;                         /* 0x91 */
    short iALONG;                     /* 0x92 */
    float fALONG;                     /* 0x94 */
} PLAYEREVENT;

typedef struct {                 /* 0x10 */
    char ascii;                  /* 0x00 */
    char pad1;                   /* 0x01 */
    char pad2;                   /* 0x02 */
    char pad3;                   /* 0x03 */
    struct nuhspecial_s obj;     /* 0x04 */
    char *name;                  /* 0x0C */
} FONT3DCHARACTER;

typedef struct {                 /* 0x0C */
    struct nuhspecial_s obj;     /* 0x00 */
    char *name;                  /* 0x08 */
} FONT3DJCHARACTER;

struct instNUGCUTRIGID_s {             /* 0xC */
    struct nuhspecial_s special;      /* 0x0 */
    char visibleframeix;              /* 0x8 */
    char pad;                         /* 0x9 */
};

typedef struct {             /* 0x7548 */
    struct nugspline_s * spl_START;   /* 0x0 */
    float time;                       /* 0x4 */
    float duration;                   /* 0x8 */
    struct nugspline_s * spl_CHASER[6]; /* 0xC */
    struct nuvec_s pos[6];            /* 0x24 */
    struct RPos_s RPos[6];            /* 0x6C */
    struct anim_s anim[6];            /* 0x144 */
    struct nuhspecial_s obj[6];       /* 0x1EC */
    short character[6];               /* 0x21C */
    short action[6];                  /* 0x228 */
    u16 xrot[6];                      /* 0x234 */
    u16 yrot[6];                      /* 0x240 */
    u16 zrot[6];                      /* 0x24C */
    Nearest_Light_s lights[6];        /* 0x70F8 */
    float scale[6];                   /* 0x7518 */
    u8 misc_phase[6];                 /* 0x7530 */
    s8 ok[6];                         /* 0x7536 */
    u8 cuboid;                        /* 0x753C */
    s8 i;                             /* 0x753D */
    s8 i_last;                        /* 0x753E */
    s8 i_next;                        /* 0x753F */
    s8 status;                        /* 0x7540 */
    s8 iRAIL;                         /* 0x7541 */
    short iALONG;                     /* 0x7542 */
    float fALONG;                     /* 0x7544 */
} CHASE;

struct PlrEvent {
    struct nugspline_s *spl;     /* 0x00 */
    struct nuhspecial_s obj[16]; /* 0x04 */
    struct nuvec_s pos;          /* 0x84 */
    char played;                 /* 0x90 */
    char iRAIL;                  /* 0x91 */
    short iALONG;                /* 0x92 */
    float fALONG;                /* 0x94 */
};

typedef struct {             /* 0xC */
    struct nuhspecial_s obj;          /* 0x0 */
    char * name;                      /* 0x8 */
} FONT3DACCENT;

typedef struct {             /* 0xC4 */
    struct nugspline_s * spl;         /* 0x0 */
    struct nuhspecial_s obj[24];      /* 0x4 */
} CHASEEVENT;

typedef struct {             /* 0x98 */
    struct nuvec_s oldpos;            /* 0x0 */
    struct nuvec_s newpos;            /* 0xC */
    struct nuvec_s wobblepos;         /* 0x18 */
    float time;                       /* 0x24 */
    float duration;                   /* 0x28 */
    float spins;                      /* 0x2C */
    float speed;                      /* 0x30 */
    float radius;                     /* 0x34 */
    float distance;                   /* 0x38 */
    float height;                     /* 0x3C */
    float lateral;                    /* 0x40 */
    float scale;                      /* 0x44 */
    float wobble;                     /* 0x48 */
    u16 oldangle;                     /* 0x4C */
    u16 angle;                        /* 0x4E */
    u16 old_yrot;                     /* 0x50 */
    u16 movflags;                     /* 0x52 */
    u16 terflags;                     /* 0x54 */
    u16 rotflags;                     /* 0x56 */
    u16 rotspeed;                     /* 0x58 */
    u16 rotadjust;                    /* 0x5A */
    u16 wobble_ang;                   /* 0x5C */
    u16 wobble_speed;                 /* 0x5E */
    s8 direction;                     /* 0x60 */
    s8 i0;                            /* 0x61 */
    s8 i1;                            /* 0x62 */
    s8 idle_update;                   /* 0x63 */
    void * event_function[2];         /* 0x64 */
    float event_distance[2];          /* 0x6C */
    short event_branch[2];            /* 0x74 */
    s8 event[2];                      /* 0x78 */
    s8 event_index[2];                /* 0x7A */
    u8 event_flags[2];                /* 0x7C */
    u8 event_occured[2];              /* 0x7E */
    u16 attack_locator_bits;          /* 0x80 */
    char pad1;                        /* 0x82 */
    char pad2;                        /* 0x83 */
    float attack_locator_radius;      /* 0x84 */
    struct nuhspecial_s obj;          /* 0x88 */
    s8 kill;                          /* 0x90 */
    s8 locator_kill;                  /* 0x91 */
    short force_action;               /* 0x92 */
    short look_creature;              /* 0x94 */
    u8 count;                         /* 0x96 */
    u8 hits;                          /* 0x97 */
} AIPacket;

struct AI_s {
    struct nuvec_s oldpos;       /* 0x00 */
    struct nuvec_s newpos;       /* 0x0C */
    struct nuvec_s wobblepos;    /* 0x18 */
    f32 time;                    /* 0x24 */
    f32 duration;                /* 0x28 */
    f32 spins;                   /* 0x2C */
    f32 speed;                   /* 0x30 */
    f32 radius;                  /* 0x34 */
    f32 distance;                /* 0x38 */
    f32 height;                  /* 0x3C */
    f32 lateral;                 /* 0x40 */
    f32 scale;                   /* 0x44 */
    f32 wobble;                  /* 0x48 */
    u16 oldangle;                /* 0x4C */
    u16 angle;                   /* 0x4E */
    u16 old_yrot;                /* 0x50 */
    u16 movflags;                /* 0x52 */
    u16 terflags;                /* 0x54 */
    u16 rotflags;                /* 0x56 */
    u16 rotspeed;                /* 0x58 */
    u16 rotadjust;               /* 0x5A */
    u16 wobble_ang;              /* 0x5C */
    u16 wobble_speed;            /* 0x5E */
    char direction;              /* 0x60 */
    char i0;                     /* 0x61 */
    char i1;                     /* 0x62 */
    char idle_update;            /* 0x63 */
    void *event_function[2];     /* 0x64 */
    f32 event_distance[2];       /* 0x6C */
    short event_branch[2];       /* 0x74 */
    char event[2];               /* 0x78 */
    char event_index[2];         /* 0x7A */
    u8 event_flags[2];           /* 0x7C */
    u8 event_occured[2];         /* 0x7E */
    u16 attack_locator_bits;     /* 0x80 */
    char pad1;                   /* 0x82 */
    char pad2;                   /* 0x83 */
    f32 attack_locator_radius;   /* 0x84 */
    struct nuhspecial_s obj;     /* 0x88 */
    char kill;                   /* 0x90 */
    char locator_kill;           /* 0x91 */
    short force_action;          /* 0x92 */
    short look_creature;         /* 0x94 */
    u8 count;                    /* 0x96 */
    u8 hits;                     /* 0x97 */
};

struct ghg_inst_s {              /* 0x08 */
    char *name;                  /* 0x00 */
    struct NUHGOBJ_s *scene;     /* 0x04 */
};

typedef struct {             /* 0xC */
    struct NUHGOBJ_s * obj;           /* 0x0 */
    char * path;                      /* 0x4 */
    char * name;                      /* 0x8 */
} CutSceneCharacter;

struct csc_s {                   /* 0x0C */
    struct NUHGOBJ_s *obj;       /* 0x00 */
    char *path;                  /* 0x04 */
    char *name;                  /* 0x08 */
};

struct NUGCUTCAMSYS_s {             /* 0x1C */
    u32 ncutcams;                     /* 0x0 */
    struct NUGCUTCAM_s * cutcams;     /* 0x4 */
    struct nuanimdata2_s * camanim;   /* 0x8 */
    struct NUSTATEANIM_s * camchanges; /* 0xC */
    char initial_camid;               /* 0x10 */
    char remap_table[10];             /* 0x11 */
    u8 pad;                           /* 0x1B */
};

struct NUGCUTLOCATOR_s {             /* 0x64 */
    struct numtx_s mtx;               /* 0x0 */
    struct nuvec_s pivot;             /* 0x40 */
    float rate;                       /* 0x4C */
    struct nuanimdata2_s * anim;      /* 0x50 */
    int direction;                    /* 0x54 */
    u8 flags;                         /* 0x58 */
    u8 locator_type;                  /* 0x59 */
    u8 joint_ix;                      /* 0x5A */
    char pad;                         /* 0x5B */
    float param1;                     /* 0x5C */
    float param2;                     /* 0x60 */
};

struct nuanimdata_s {            /* 0x10 */
    float time;                  /* 0x00 */
    char *node_name;             /* 0x04 */
    int nchunks;                 /* 0x08 */
    struct nuanimdatachunk_s **chunks; /* 0x0C */
};

struct instNUGCUTRIGIDSYS_s {             /* 0x4 */
    struct instNUGCUTRIGID_s * irigids; /* 0x0 */
};

struct NUGCUTRIGID_s {             /* 0x5C */
    struct numtx_s mtx;               /* 0x0 */
    char * name;                      /* 0x40 */
    struct nuhspecial_s special;      /* 0x44 */
    struct nuanimdata2_s * anim;      /* 0x4C */
    struct NUSTATEANIM_s * visibiltyanim; /* 0x50 */
    struct NUGCUTLOCATOR_s * locators; /* 0x54 */
    u8 flags;                         /* 0x58 */
    u8 nlocators;                     /* 0x59 */
    u8 first_locator;                 /* 0x5A */
    char pad;                         /* 0x5B */
};

struct NUGCUTLOCATORSYS_s {             /* 0xC */
    struct NUGCUTLOCATOR_s * locators; /* 0x0 */
    struct NUGCUTLOCATORTYPE_s * locator_types; /* 0x4 */
    u8 nlocators;                     /* 0x8 */
    u8 ntypes;                        /* 0x9 */
    char pad[2];                      /* 0xA */
};

struct NUGCUTCHAR_s {             /* 0x64 */
    struct numtx_s mtx;               /* 0x0 */
    char * name;                      /* 0x40 */
    struct nuanimdata2_s * char_anim; /* 0x44 */
    struct nuanimdata2_s * joint_anim; /* 0x48 */
    struct nuanimdata2_s * face_anim; /* 0x4C */
    void * character;                 /* 0x50 */
    struct NUGCUTLOCATOR_s * locators; /* 0x54 */
    float animrate;                   /* 0x58 */
    u8 flags;                         /* 0x5C */
    u8 animix;                        /* 0x5D */
    u8 nlocators;                     /* 0x5E */
    u8 type;                          /* 0x5F */
    u8 first_locator;                 /* 0x60 */
    u8 blendframes;                   /* 0x61 */
    char pad[2];                      /* 0x62 */
};

struct CharacterModel {
    struct NUHGOBJ_s *hobj;      /* 0x000 */
    struct nuanimdata_s *anmdata[118]; /* 0x004 */
    struct animlist *animlist[118];    /* 0x1DC */
    struct nuanimdata_s *fanmdata[118]; /* 0x3B4 */
    struct animlist *fanimlist[118];    /* 0x58C */
    int shadhdr;                 /* 0x764 */
    short character;             /* 0x768 */
    char pad1;                   /* 0x76A */
    char pad2;                   /* 0x76B */
    struct NUPOINTOFINTEREST_s *pLOCATOR[16]; /* 0x76C */
};

struct NUHINSTANCE_s {             /* 0x50 */
    struct numtx_s mtx;               /* 0x0 */
    struct NUHGOBJ_s * hgobj;         /* 0x40 */
    struct nuanimdata_s * animdata;   /* 0x44 */
    struct nuanimdata_s * blendweightanimdata; /* 0x48 */
    char pad[4];                      /* 0x4C */
};

struct animdatainst_s {          /* 0x108 */
    struct nuanimdata_s *ad;     /* 0x00 */
    char name[256];              /* 0x04 */
    int inst_cnt;                /* 0x104 */
};

struct nuanim_s {                /* 0x14 */
    struct nuanimdata_s *animdata; /* 0x00 */
    struct NUNODE_s *node;       /* 0x04 */
    s32 *prev_keys;              /* 0x08 */
    s32 prev_key_ix;             /* 0x0C */
    struct nuanimkey_s **prev_anim_key; /* 0x10 */
};

struct NUANIMDATAHDR_s {         /* 0x0C */
    s32 version;                 /* 0x00 */
    s32 address_offset;          /* 0x04 */
    struct nuanimdata_s *animdata; /* 0x08 */
};

struct NUGCUTRIGIDSYS_s {             /* 0x8 */
    struct NUGCUTRIGID_s * rigids;    /* 0x0 */
    u16 nrigids;                      /* 0x4 */
    char pad[2];                      /* 0x6 */
};

struct NUGCUTCHARSYS_s {             /* 0x8 */
    struct NUGCUTCHAR_s * chars;      /* 0x0 */
    u16 nchars;                       /* 0x4 */
    char pad[2];                      /* 0x6 */
};

struct obj_s {
    struct obj_s *parent;        /* 0x000 */
    struct CharacterModel *model; /* 0x004 */
    struct mask *mask;           /* 0x008 */
    void *contact;               /* 0x00C */
    struct numtx_s *pLOCATOR;    /* 0x010 */
    struct anim_s anim;          /* 0x014 */
    short character;             /* 0x030 */
    short vehicle;               /* 0x032 */
    u32 flags;                   /* 0x034 */
    u32 frame;                   /* 0x038 */
    u32 draw_frame;              /* 0x03C */
    u32 vehicle_frame;           /* 0x040 */
    struct RPos_s RPos;          /* 0x044 */
    struct nuvec_s pos;          /* 0x068 */
    struct nuvec_s mom;          /* 0x074 */
    struct nuvec_s oldpos;       /* 0x080 */
    struct nuvec_s startpos;     /* 0x08C */
    struct nuvec_s vSN;          /* 0x098 */
    struct nuvec_s vLN;          /* 0x0A4 */
    struct nuvec_s vRN;          /* 0x0B0 */
    float shadow;                /* 0x0BC */
    float layer_shadow;          /* 0x0C0 */
    float roof_y;                /* 0x0C4 */
    float clearance;             /* 0x0C8 */
    float forward;               /* 0x0CC */
    float abs_forward;           /* 0x0D0 */
    float side;                  /* 0x0D4 */
    float abs_side;              /* 0x0D8 */
    float xyz_distance;          /* 0x0DC */
    float xz_distance;           /* 0x0E0 */
    float radius;                /* 0x0E4 */
    struct nuvec_s min;          /* 0x0E8 */
    struct nuvec_s max;          /* 0x0F4 */
    float scale;                 /* 0x100 */
    float SCALE;                 /* 0x104 */
    float RADIUS;                /* 0x108 */
    float old_SCALE;             /* 0x10C */
    float objbot;                /* 0x110 */
    float objtop;                /* 0x114 */
    float bot;                   /* 0x118 */
    float top;                   /* 0x11C */
    float oldobjbot;             /* 0x120 */
    float oldobjtop;             /* 0x124 */
    float die_time;              /* 0x128 */
    float die_duration;          /* 0x12C */
    float reflect_y;             /* 0x130 */
    float idle_gametime;         /* 0x134 */
    float pad_speed;             /* 0x138 */
    float pad_dx;                /* 0x13C */
    float pad_dz;                /* 0x140 */
    char i;                      /* 0x144 */
    char dead;                   /* 0x145 */
    u16 pad_angle;               /* 0x146 */
    u16 attack;                  /* 0x148 */
    u16 vulnerable;              /* 0x14A */
    short die_action;            /* 0x14C */
    char old_ground;             /* 0x14E */
    char finished;               /* 0x14F */
    u16 xrot;                    /* 0x150 */
    u16 yrot;                    /* 0x152 */
    u16 zrot;                    /* 0x154 */
    u16 surface_xrot;            /* 0x156 */
    u16 surface_zrot;            /* 0x158 */
    u16 layer_xrot;              /* 0x15A */
    u16 layer_zrot;              /* 0x15C */
    u16 roof_xrot;               /* 0x15E */
    u16 roof_zrot;               /* 0x160 */
    short target_xrot;           /* 0x162 */
    short target_yrot;           /* 0x164 */
    short dyrot;                 /* 0x166 */
    union type_s_name gndflags;  /* 0x168 */
    u16 hdg;                     /* 0x16A */
    u16 thdg;                    /* 0x16C */
    char ground;                 /* 0x16E */
    char surface_type;           /* 0x16F */
    char layer_type;             /* 0x170 */
    char roof_type;              /* 0x171 */
    char invisible;              /* 0x172 */
    u8 submerged;                /* 0x173 */
    char transporting;           /* 0x174 */
    char got_shadow;             /* 0x175 */
    u8 boing;                    /* 0x176 */
    u8 contact_type;             /* 0x177 */
    char die_model[2];           /* 0x178 */
    u8 invincible;               /* 0x17A */
    char pos_adjusted;           /* 0x17B */
    char wade;                   /* 0x17C */
    char dangle;                 /* 0x17D */
    char ddsand;                 /* 0x17E */
    char ddsnow;                 /* 0x17F */
    char ddwater;                /* 0x180 */
    char ddr;                    /* 0x181 */
    char ddg;                    /* 0x182 */
    char ddb;                    /* 0x183 */
    char last_ground;            /* 0x184 */
    char direction;              /* 0x185 */
    char kill_contact;           /* 0x186 */
    u8 touch;                    /* 0x187 */
};

typedef struct {             /* 0x188 */
    void * parent;                    /* 0x0 */
    CharacterModel * model;           /* 0x4 */
    MaskInfo * mask;                  /* 0x8 */
    void * contact;                   /* 0xC */
    struct numtx_s * pLOCATOR;        /* 0x10 */
    struct anim_s anim;               /* 0x14 */
    short character;                  /* 0x30 */
    short vehicle;                    /* 0x32 */
    u32 flags;                        /* 0x34 */
    u32 frame;                        /* 0x38 */
    u32 draw_frame;                   /* 0x3C */
    u32 vehicle_frame;                /* 0x40 */
    struct RPos_s RPos;               /* 0x44 */
    struct nuvec_s pos;               /* 0x68 */
    struct nuvec_s mom;               /* 0x74 */
    struct nuvec_s oldpos;            /* 0x80 */
    struct nuvec_s startpos;          /* 0x8C */
    struct nuvec_s vSN;               /* 0x98 */
    struct nuvec_s vLN;               /* 0xA4 */
    struct nuvec_s vRN;               /* 0xB0 */
    float shadow;                     /* 0xBC */
    float layer_shadow;               /* 0xC0 */
    float roof_y;                     /* 0xC4 */
    float clearance;                  /* 0xC8 */
    float forward;                    /* 0xCC */
    float abs_forward;                /* 0xD0 */
    float side;                       /* 0xD4 */
    float abs_side;                   /* 0xD8 */
    float xyz_distance;               /* 0xDC */
    float xz_distance;                /* 0xE0 */
    float radius;                     /* 0xE4 */
    struct nuvec_s min;               /* 0xE8 */
    struct nuvec_s max;               /* 0xF4 */
    float scale;                      /* 0x100 */
    float SCALE;                      /* 0x104 */
    float RADIUS;                     /* 0x108 */
    float old_SCALE;                  /* 0x10C */
    float objbot;                     /* 0x110 */
    float objtop;                     /* 0x114 */
    float bot;                        /* 0x118 */
    float top;                        /* 0x11C */
    float oldobjbot;                  /* 0x120 */
    float oldobjtop;                  /* 0x124 */
    float die_time;                   /* 0x128 */
    float die_duration;               /* 0x12C */
    float reflect_y;                  /* 0x130 */
    float idle_gametime;              /* 0x134 */
    float pad_speed;                  /* 0x138 */
    float pad_dx;                     /* 0x13C */
    float pad_dz;                     /* 0x140 */
    s8 i;                             /* 0x144 */
    s8 dead;                          /* 0x145 */
    u16 pad_angle;                    /* 0x146 */
    u16 attack;                       /* 0x148 */
    u16 vulnerable;                   /* 0x14A */
    short die_action;                 /* 0x14C */
    s8 old_ground;                    /* 0x14E */
    s8 finished;                      /* 0x14F */
    u16 xrot;                         /* 0x150 */
    u16 yrot;                         /* 0x152 */
    u16 zrot;                         /* 0x154 */
    u16 surface_xrot;                 /* 0x156 */
    u16 surface_zrot;                 /* 0x158 */
    u16 layer_xrot;                   /* 0x15A */
    u16 layer_zrot;                   /* 0x15C */
    u16 roof_xrot;                    /* 0x15E */
    u16 roof_zrot;                    /* 0x160 */
    short target_xrot;                /* 0x162 */
    short target_yrot;                /* 0x164 */
    short dyrot;                      /* 0x166 */
    u16 hdg;                          /* 0x16A */
    u16 thdg;                         /* 0x16C */
    s8 ground;                        /* 0x16E */
    s8 surface_type;                  /* 0x16F */
    s8 layer_type;                    /* 0x170 */
    s8 roof_type;                     /* 0x171 */
    s8 invisible;                     /* 0x172 */
    u8 submerged;                     /* 0x173 */
    s8 transporting;                  /* 0x174 */
    s8 got_shadow;                    /* 0x175 */
    u8 boing;                         /* 0x176 */
    u8 contact_type;                  /* 0x177 */
    s8 die_model[2];                  /* 0x178 */
    u8 invincible;                    /* 0x17A */
    s8 pos_adjusted;                  /* 0x17B */
    s8 wade;                          /* 0x17C */
    s8 dangle;                        /* 0x17D */
    char ddsand;                      /* 0x17E */
    char ddsnow;                      /* 0x17F */
    char ddwater;                     /* 0x180 */
    char ddr;                         /* 0x181 */
    char ddg;                         /* 0x182 */
    char ddb;                         /* 0x183 */
    s8 last_ground;                   /* 0x184 */
    s8 direction;                     /* 0x185 */
    s8 kill_contact;                  /* 0x186 */
    u8 touch;                         /* 0x187 */
} GameObject;

struct NUCUTSCENE_s {             /* 0xC4 */
    int * tids;                       /* 0x0 */
    int numtid;                       /* 0x4 */
    struct numtl_s * * mtls;          /* 0x8 */
    int nummtl;                       /* 0xC */
    float length;                     /* 0x10 */
    char * string_table;              /* 0x14 */
    struct NUCUTSCENECAMERA_s * cameras[10]; /* 0x18 */
    struct nuanimdata_s * camera_anim; /* 0x40 */
    float * camera_change_times;      /* 0x44 */
    u8 * camera_changes;              /* 0x48 */
    int numgobj;                      /* 0x4C */
    struct nugobj_s * * gobjs;        /* 0x50 */
    int numinstance;                  /* 0x54 */
    struct nuinstance_s * instances;  /* 0x58 */
    struct nuanimdata_s * instance_animdata; /* 0x5C */
    int numhgobj;                     /* 0x60 */
    struct NUHGOBJ_s * * hgobj;       /* 0x64 */
    int numhinstance;                 /* 0x68 */
    struct NUHINSTANCE_s * hinstances; /* 0x6C */
    struct NUCUTSCENEAUDIO_s * audio; /* 0x70 */
    float current_time;               /* 0x74 */
    int flags;                        /* 0x78 */
    char audio_path[64];              /* 0x7C */
    u8 num_camera_changes;            /* 0xBC */
    u8 camera_change_ix;              /* 0xBD */
    u8 camera_ix;                     /* 0xBE */
    u8 camera_anim_ix;                /* 0xBF */
    u8 numaudio;                      /* 0xC0 */
    u8 next_audioix;                  /* 0xC1 */
    char pad[2];                      /* 0xC2 */
};

struct NUGCUTSCENE_s {             /* 0x28 */
    int version;                      /* 0x0 */
    int address_offset;               /* 0x4 */
    float nframes;                    /* 0x8 */
    char * name_table;                /* 0xC */
    struct NUGCUTCAMSYS_s * cameras;  /* 0x10 */
    struct NUGCUTRIGIDSYS_s * rigids; /* 0x14 */
    struct NUGCUTCHARSYS_s * chars;   /* 0x18 */
    struct NUGCUTLOCATORSYS_s * locators; /* 0x1C */
    struct NUGCUTBBOX_s * bbox;       /* 0x20 */
    struct NUGCUTTRIGGERSYS_s * triggersys; /* 0x24 */
};

typedef struct {             /* 0x1B0 */
    struct obj_s obj;                 /* 0x0 */
    struct nuvec_s srcpos;            /* 0x188 */
    struct nuvec_s dstpos;            /* 0x194 */
    float time;                       /* 0x1A0 */
    float duration;                   /* 0x1A4 */
    s8 type;                          /* 0x1A8 */
    s8 active;                        /* 0x1A9 */
    short i_objtab;                   /* 0x1AA */
    s8 kill;                          /* 0x1AC */
    u8 owner_safety;                  /* 0x1AD */
    char pad1;                        /* 0x1AE */
    char pad2;                        /* 0x1AF */
} PROJECTILE;

struct creature_s {              /* 0xCE4 */
    char used;                   /* 0x000 */
    char on;                     /* 0x001 */
    char off_wait;               /* 0x002 */
    char i_aitab;                /* 0x003 */
    struct obj_s obj;            /* 0x004 */
    struct AI_s ai;              /* 0x18C */
    struct NEWBUGGY *Buggy;      /* 0x224 */
    struct creatcmd_s *cmdtable; /* 0x228 */
    struct creatcmd_s *cmdcurr;  /* 0x22C */
    struct MoveInfo *OnFootMoveInfo; /* 0x230 */
    struct numtx_s m;            /* 0x234 */
    struct numtx_s mtxLOCATOR[16][2]; /* 0x274 */
    struct nuvec_s momLOCATOR[16][2]; /* 0xA74 */
    struct Nearest_Light_s lights; /* 0xCF4 - approx */
    struct rumble_s rumble;
    f32 idle_time;
    f32 idle_wait;
    short idle_action;
    short old_idle_action;
    char idle_mode;
    char idle_repeat;
    char jump;
    char jump_type;
    char jump_subtype;
    char ok_slam;
    char slam;
    char spin;
    char crawl;
    char crawl_lock;
    char tiptoe;
    char sprint;
    u8 somersault;
    u8 land;
    char pad_type;
    char jump_hack;
    u8 jump_hold;
    u8 allow_jump;
    short jump_frames;
    short jump_frame;
    short slam_wait;
    short spin_frames;
    short spin_frame;
    short spin_wait;
    short slide;
    short crouch_pos;
    u16 slam_frame;
    short fire_action;
    u8 fire;
    u8 tap;
    char target;
    char target_wait;
    char fire_lock;
    char idle_sigh;
    u8 hit_type;
    u8 freeze;
    char anim_processed;
    char pad1;
};

struct instNUGCUTSCENE_s {             /* 0x94 */
    struct instNUGCUTSCENE_s * next;  /* 0x0 */
    struct instNUGCUTSCENE_s * prev;  /* 0x4 */
    char name[16];                    /* 0x8 */
    struct numtx_s mtx;               /* 0x18 */
    struct NUGCUTSCENE_s * cutscene;  /* 0x58 */
    struct nuvec_s centre;            /* 0x5C */
    float maxdistsqr;                 /* 0x68 */
    int played_first_frame;           /* 0x6C */
    u32 is_playing;                   /* 0x6C */
    int looping;                      /* 0x6C */
    int autostart;                    /* 0x6C */
    int has_mtx;                      /* 0x6C */
    int checkbboxclip;                /* 0x6C */
    int checkmaxdist;                 /* 0x6C */
    int is_visible;                   /* 0x6C */
    int is_disabled;                  /* 0x6C */
    int been_updated_this_frame;      /* 0x6C */
    float cframe;                     /* 0x70 */
    float rate;                       /* 0x74 */
    struct instNUGCUTCAMSYS_s * icameras; /* 0x78 */
    struct instNUGCUTRIGIDSYS_s * irigids; /* 0x7C */
    struct instNUGCUTCHARSYS_s * ichars; /* 0x80 */
    struct instNUGCUTLOCATORSYS_s * ilocators; /* 0x84 */
    struct instNUGCUTTRIGGERSYS_s * itriggersys; /* 0x88 */
    struct instNUGCUTSCENE_s * next_to_play; /* 0x8C */
    NUCUTSCENEFINISHEDCALLBACKFN * endfn; /* 0x90 */
};

struct JEEPSTRUCT {             /* 0x988 */
    struct creature_s * Cre;          /* 0x0 */
    struct MYDRAW ChassisDraw;        /* 0x4 */
    struct numtx_s ChassisLocators[16]; /* 0xE4 */
    struct numtx_s DrawMtx;           /* 0x4E4 */
    struct NUJOINTANIM_s Joints[8];   /* 0x524 */
    struct SIMWHEEL TrailWheel[4];    /* 0x6C4 */
    struct POINTANG RestartPoint;     /* 0x764 */
    float DownHoleTimer;              /* 0x774 */
    int DownHole;                     /* 0x778 */
    int Dropped;                      /* 0x77C */
    struct nuvec_s RestartCamPos;     /* 0x780 */
    struct nuvec_s RestartCamObj;     /* 0x78C */
    float FireBossTurnTimer;          /* 0x798 */
    float WheelHeight[4];             /* 0x79C */
    float TimeLine;                   /* 0x7AC */
    int FireBossDir;                  /* 0x7B0 */
    int CantMove;                     /* 0x7B4 */
    int Quick;                        /* 0x7B8 */
    float MaxSpeed;                   /* 0x7BC */
    float MySpeed;                    /* 0x7C0 */
    float DefaultSpeed;               /* 0x7C4 */
    float StartSpeed;                 /* 0x7C8 */
    float StartSpeedTimer;            /* 0x7CC */
    int Active;                       /* 0x7D0 */
    short aWRot[4];                   /* 0x7D4 */
    short aFrontWheelAng;             /* 0x7DC */
    short aOldFrontWheelAng;          /* 0x7DE */
    struct nuvec_s Pos;               /* 0x7E0 */
    short aAngleY;                    /* 0x7EC */
    short aMovementAng;               /* 0x7EE */
    short aSurfRotX;                  /* 0x7F0 */
    short aSurfRotZ;                  /* 0x7F2 */
    float TiltSeekTime;               /* 0x7F4 */
    short aTiltX;                     /* 0x7F8 */
    short aTiltZ;                     /* 0x7FA */
    short aDestTiltX;                 /* 0x7FC */
    short aDestTiltZ;                 /* 0x7FE */
    short aInputAng;                  /* 0x800 */
    float InputSpeed;                 /* 0x804 */
    int WheelSpin;                    /* 0x808 */
    short aDeltaAng;                  /* 0x80C */
    short aLastDeltaAng;              /* 0x80E */
    short aLastDeltaAngA;             /* 0x810 */
    short aOldChassisAngleY;          /* 0x812 */
    short aChassisAngleY;             /* 0x814 */
    short aChassisTargetAngleY;       /* 0x816 */
    int aChassisAngMom;               /* 0x818 */
    float Accelerator;                /* 0x81C */
    float AccelerationForce;          /* 0x820 */
    float CentrefugalForce;           /* 0x824 */
    float Traction;                   /* 0x828 */
    float GroundTractionAcc;          /* 0x82C */
    float TurnSin;                    /* 0x830 */
    short aBaseMoveAng;               /* 0x834 */
    float CentRailDist;               /* 0x838 */
    float BoostOnTimer;               /* 0x83C */
    float BoostTimer;                 /* 0x840 */
    int Finished;                     /* 0x844 */
    float CarryOnRecordTime;          /* 0x848 */
    float FloorHeight;                /* 0x84C */
    int TerrainType;                  /* 0x850 */
    struct MYSPLINE Spline;           /* 0x854 */
    float FireTimer;                  /* 0x884 */
    struct VEHICLE Move;              /* 0x888 */
};

struct GLIDERSTRUCT {             /* 0x440 */
    struct creature_s * Cre;          /* 0x0 */
    struct nuvec_s Vel;               /* 0x4 */
    int Dead;                         /* 0x10 */
    int CocoDead;                     /* 0x14 */
    float CocoDeadTimer;              /* 0x18 */
    float CocoDeathSpinX;             /* 0x1C */
    float CocoDeathSpinZ;             /* 0x20 */
    float NextEngRum;                 /* 0x24 */
    float FixVelTimer;                /* 0x28 */
    float ImmuneAsteroidsTimer;       /* 0x2C */
    struct nuvec_s Position;          /* 0x30 */
    struct nuvec_s OldPosition;       /* 0x3C */
    struct nuvec_s Velocity;          /* 0x48 */
    struct nuvec_s Resolved;          /* 0x54 */
    struct nuvec_s RailPoint;         /* 0x60 */
    float RailAngle;                  /* 0x6C */
    float TiltX;                      /* 0x70 */
    float TiltZ;                      /* 0x74 */
    float DestTiltX;                  /* 0x78 */
    float DestTiltZ;                  /* 0x7C */
    float AngleY;                     /* 0x80 */
    float CamAngleY;                  /* 0x84 */
    float CamTiltX;                   /* 0x88 */
    float CamTornRecoverTimer;        /* 0x8C */
    float InputX;                     /* 0x90 */
    float InputZ;                     /* 0x94 */
    int BarrelRoll;                   /* 0x98 */
    float BarrelDelta;                /* 0x9C */
    float BarrelSpeedX;               /* 0xA0 */
    int TerminalDive;                 /* 0xA4 */
    int TerminalDir;                  /* 0xA8 */
    float FireTimer;                  /* 0xAC */
    float HitTimer;                   /* 0xB0 */
    int AutoPilot;                    /* 0xB4 */
    int ForceTurn;                    /* 0xB8 */
    int HitPoints;                    /* 0xBC */
    char LocatorList[16];             /* 0xC0 */
    float LocatorTime[16];            /* 0xD0 */
    int InTornado;                    /* 0x110 */
    int LastInTornado;                /* 0x114 */
    float InTornadoTime;              /* 0x118 */
    float TornadoSpin;                /* 0x11C */
    float InTornadoScale;             /* 0x120 */
    struct nuvec_s ApparentPosition;  /* 0x124 */
    struct nuvec_s ApparentVelocity;  /* 0x130 */
    struct nuvec_s PositionStack[30]; /* 0x13C */
    struct nuvec_s VelocityStack[30]; /* 0x2A4 */
    int StackIndx;                    /* 0x40C */
    float Speed;                      /* 0x410 */
    float TargetSpeed;                /* 0x414 */
    float NotInFrontTimer;            /* 0x418 */
    float PullUpTimer;                /* 0x41C */
    float OverideTiltZ;               /* 0x420 */
    float TargetTimer;                /* 0x424 */
    struct nuvec_s * MovingTargetPoint; /* 0x428 */
    struct nuvec_s * MovingTargetVel; /* 0x42C */
    int TargetOn;                     /* 0x430 */
    float TargetedTime;               /* 0x434 */
    int TargetMoving;                 /* 0x438 */
    float NextHitSoundTimer;          /* 0x43C */
};

typedef struct {             /* 0x20 */
    struct creature_s * c;            /* 0x0 */
    s8 track;                         /* 0x4 */
    s8 count;                         /* 0x5 */
    u8 wait;                          /* 0x6 */
    s8 laps;                          /* 0x7 */
    float time;                       /* 0x8 */
    float duration;                   /* 0xC */
    float lap_position;               /* 0x10 */
    s8 place;                         /* 0x14 */
    u8 finished;                      /* 0x15 */
    u8 finishframes;                  /* 0x16 */
    u8 stall;                         /* 0x17 */
    u16 old_xrot;                     /* 0x18 */
    u16 old_yrot;                     /* 0x1A */
    u16 old_zrot;                     /* 0x1C */
    u8 boost;                         /* 0x1E */
    s8 spark;                         /* 0x1F */
} TubInfo;

struct ATLASSTRUCT {             /* 0x384 */
    struct creature_s * Cre;          /* 0x0 */
    int Whacko;                       /* 0x4 */
    int Type;                         /* 0x8 */
    int Dead;                         /* 0xC */
    int HitPoints;                    /* 0x10 */
    int DestHitPoints;                /* 0x14 */
    int HitPointCounter;              /* 0x18 */
    int DrawCrunch;                   /* 0x1C */
    int DrawShell;                    /* 0x20 */
    float InhibitControlTimer;        /* 0x24 */
    int NumAttacks;                   /* 0x28 */
    int Action;                       /* 0x2C */
    int LastAction;                   /* 0x30 */
    float ActionTimer;                /* 0x34 */
    float ActionTimer2;               /* 0x38 */
    int BeenHit;                      /* 0x3C */
    int CantBeHit;                    /* 0x40 */
    float RollSpeed;                  /* 0x44 */
    float RollAccTimer;               /* 0x48 */
    struct MYDRAW Shell;              /* 0x4C */
    struct MYDRAW Crunch;             /* 0x12C */
    struct nuvec_s Position;          /* 0x20C */
    struct nuvec_s OldPosition;       /* 0x218 */
    struct nuvec_s LastPosition;      /* 0x224 */
    struct nuvec_s TargetPosition;    /* 0x230 */
    struct nuvec_s Velocity;          /* 0x23C */
    struct nuvec_s OldVelocity;       /* 0x248 */
    struct nuvec_s Resolved;          /* 0x254 */
    struct nuvec_s Force;             /* 0x260 */
    float Radius;                     /* 0x26C */
    float AngleX;                     /* 0x270 */
    float AngleY;                     /* 0x274 */
    float AngleZ;                     /* 0x278 */
    float InputAng;                   /* 0x27C */
    float InputMag;                   /* 0x280 */
    struct nuquat_s Quat;             /* 0x284 */
    struct nuquat_s ThisQuat;         /* 0x294 */
    struct nuquat_s FrameQuat[4];     /* 0x2A4 */
    struct nuquat_s LastQuat;         /* 0x2E4 */
    int LastHit;                      /* 0x2F4 */
    int Axis;                         /* 0x2F8 */
    struct nuvec_s LastNormal;        /* 0x2FC */
    struct nuvec_s StoreLastNormal;   /* 0x308 */
    float DebugAxisTurn;              /* 0x314 */
    float DebugAngY;                  /* 0x318 */
    float D[7];                       /* 0x31C */
    int OnGround;                     /* 0x338 */
    int PlatformId;                   /* 0x33C */
    struct nuvec_s PlatformNormal;    /* 0x340 */
    float ShadowY;                    /* 0x34C */
    int SurfaceType;                  /* 0x350 */
    int TrailSurfaceType;             /* 0x354 */
    int BigDrop;                      /* 0x358 */
    int Embedded;                     /* 0x35C */
    int Rock;                         /* 0x360 */
    int RockNum;                      /* 0x364 */
    float CrunchY;                    /* 0x368 */
    float DestCrunchY;                /* 0x36C */
    short * TerrHandle;               /* 0x370 */
    float BoostTimer;                 /* 0x374 */
    struct nuvec_s InterestPoint;     /* 0x378 */
};

struct nulsthdr_s {              /* 0x10 */
    struct nulnkhdr_s *free;     /* 0x00 */
    struct nulnkhdr_s *head;     /* 0x04 */
    struct nulnkhdr_s *tail;     /* 0x08 */
    short elcnt;                 /* 0x0C */
    short elsize;                /* 0x0E */
};

struct nulnkhdr_s {              /* 0x10 */
    struct nulsthdr_s *owner;    /* 0x00 */
    struct nulnkhdr_s *succ;     /* 0x04 */
    struct nulnkhdr_s *prev;     /* 0x08 */
    u16 id;                      /* 0x0C */
    u16 used;                    /* 0x0E - bitfield: 1 bit */
};

struct SATELLITESTRUCT {             /* 0x124 */
    int Active;                       /* 0x0 */
    struct MYDRAW MainDraw;           /* 0x4 */
    struct nuvec_s Position;          /* 0xE4 */
    struct nuvec_s Velocity;          /* 0xF0 */
    float AngleY;                     /* 0xFC */
    float TiltX;                      /* 0x100 */
    float TiltZ;                      /* 0x104 */
    float DestTiltX;                  /* 0x108 */
    float DestTiltZ;                  /* 0x10C */
    int Seen;                         /* 0x110 */
    int HitPoints;                    /* 0x114 */
    int Explode;                      /* 0x118 */
    struct SPACESTATIONSTRUCT * SpaceStation; /* 0x11C */
    float SatelliteAngleY;            /* 0x120 */
};

struct SPACESTATIONSTRUCT {             /* 0xD0 */
    struct numtx_s Matrix;            /* 0x0 */
    int Active;                       /* 0x40 */
    struct nuvec_s Position;          /* 0x44 */
    struct nuvec_s Velocity;          /* 0x50 */
    float AngleY;                     /* 0x5C */
    int Seen;                         /* 0x60 */
    int HitPoints;                    /* 0x64 */
    int Explode;                      /* 0x68 */
    float SatelliteAngleY;            /* 0x6C */
    struct SATELLITESTRUCT * Satellite[3]; /* 0x70 */
    struct numtx_s ShieldMtx;         /* 0x7C */
    int ShieldDraw;                   /* 0xBC */
    int ShieldGreen;                  /* 0xC0 */
    struct nuvec_s AppCentre;         /* 0xC4 */
};
