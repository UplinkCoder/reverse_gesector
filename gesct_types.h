typedef struct IDirectSoundBuffer *LPDIRECTSOUNDBUFFER;
typedef float geFloat;
typedef int geBoolean;

typedef int BOOL;
typedef char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;
typedef struct Large_Integer
{
    DWORD lo;
    DWORD hi;
} LARGE_INTEGER;

typedef signed char int8;
typedef signed int int32;
typedef signed short int16;

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;

typedef struct  geVFile geVFile;
typedef struct HWND__ *HWND;
typedef struct IDirectSoundVtbl IDirectSoundVtbl;
// typedef struct HMODULE *HMODULE;

typedef struct User_Info User_Info;
typedef struct geBitmap geBitmap;
typedef struct geWorld geWorld;
typedef struct HINSTANCE *HINSTANCE;
typedef struct DRV_Driver DRV_Driver;

struct IDirectSound
{
  struct IDirectSoundVtbl *lpVtbl;
};

typedef struct tWAVEFORMATEX {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;

typedef struct _DSBUFFERDESC1
{
    DWORD		dwSize;
    DWORD		dwFlags;
    DWORD		dwBufferBytes;
    DWORD		dwReserved;
    LPWAVEFORMATEX	lpwfxFormat;
} DSBUFFERDESC1,*LPDSBUFFERDESC1;

typedef struct DSBUFFERDESC {
    DWORD dwSize;
    DWORD dwFlags;
    DWORD dwBufferBytes;
    DWORD dwReserved;
    LPWAVEFORMATEX lpwfxFormat;
//    GUID guid3DAlgorithm;
} DSBUFFERDESC;

typedef struct IDirectSound *LPDIRECTSOUND;
typedef struct IDirectSoundBuffer *LPDIRECTSOUNDBUFFER;

typedef struct geSound_Cfg
{
    geFloat         Volume;
    geFloat         Pan;
    geFloat         Frequency;
} geSound_Cfg;

typedef struct Channel
{
//  char*               name;
    LPDIRECTSOUNDBUFFER buffer;
    unsigned int        ID;
    int                 BaseFreq;
    geSound_Cfg         cfg;
    void *              Data;
    struct Channel      *next;
    struct Channel      *nextDup;
} Channel;

typedef struct SoundManager
{
    int                     smChannelCount;
    unsigned int            smNextChannelID;

    LPDIRECTSOUNDBUFFER     smPrimaryChannel;
    Channel*                smChannels;
    //LPDIRECTSOUNDNOTIFY *   smNotify;
} SoundManager;

typedef struct geSound_System
{
    geBoolean       Active;
    SoundManager    *SoundM;
    geFloat         GlobalVolume;
} geSound_System;
// ----------------------------------------------------
// ---------------------------------------------------
static SoundManager *   CreateSoundManager(HWND hWnd);
static void     DestroySoundManager(SoundManager *sm);

//static BOOL       FillSoundChannel(SoundManager *sm, char* Dir, char *Name, unsigned int* Handle );
static BOOL     FillSoundChannel(SoundManager *sm, geVFile *File, unsigned int* Handle );
//static BOOL       FillSoundChannelMemory(SoundManager *sm, const void *Buffer, unsigned int* Handle );
static BOOL     StartSoundChannel( SoundManager *sm, unsigned int Handle, geSound_Cfg *cfg, int loop, unsigned int* sfx);
static BOOL     StopSoundChannel(Channel *channel);
static BOOL     FreeAllChannels(SoundManager *sm);
static BOOL     FreeChannel(SoundManager *sm, Channel *channel);
static BOOL     ModifyChannel( Channel *channel, geSound_Cfg *cfg );
static int      ChannelPlaying( Channel *channel );
static Channel* GetChannel( SoundManager *sm, unsigned int ID );

static  LPDIRECTSOUND           lpDirectSound;
// -------------------- Dcommon --------------------------------------
typedef enum        // all supported formats (including shifts)
{
    GE_PIXELFORMAT_NO_DATA = 0,
    GE_PIXELFORMAT_8BIT,                // PAL
    GE_PIXELFORMAT_8BIT_GRAY,       // no palette (intensity from bit value)
    GE_PIXELFORMAT_16BIT_555_RGB,
    GE_PIXELFORMAT_16BIT_555_BGR,
    GE_PIXELFORMAT_16BIT_565_RGB,   // #5
    GE_PIXELFORMAT_16BIT_565_BGR,
    GE_PIXELFORMAT_16BIT_4444_ARGB, // #7
    GE_PIXELFORMAT_16BIT_1555_ARGB,
    GE_PIXELFORMAT_24BIT_RGB,       // #9
    GE_PIXELFORMAT_24BIT_BGR,
    GE_PIXELFORMAT_24BIT_YUV,       // * see note below
    GE_PIXELFORMAT_32BIT_RGBX,
    GE_PIXELFORMAT_32BIT_XRGB,
    GE_PIXELFORMAT_32BIT_BGRX,
    GE_PIXELFORMAT_32BIT_XBGR,
    GE_PIXELFORMAT_32BIT_RGBA,
    GE_PIXELFORMAT_32BIT_ARGB,      // #17
    GE_PIXELFORMAT_32BIT_BGRA,
    GE_PIXELFORMAT_32BIT_ABGR,

    GE_PIXELFORMAT_WAVELET,         // #20 , Wavelet Compression

/* 03/30/2003 Wendell Buckner
    BUMPMAPPING */
    GE_PIXELFORMAT_16BIT_88_UV,
    GE_PIXELFORMAT_16BIT_556_UVL,
    GE_PIXELFORMAT_24BIT_888_UVL,

    GE_PIXELFORMAT_COUNT
} gePixelFormat;

typedef struct
{
    gePixelFormat   PixelFormat;
    uint32          Flags;
} geRDriver_PixelFormat;


// ----------------------------- geEngine defines ---------------------
#define     MAX_SUB_DRIVERS             12
#define     MAX_DRIVER_MODES            32
#define     DRV_STR_SIZE                512
#define     DRV_MODE_STR_SIZE           512

#define MAX_CLIENT_STRING_LEN   80
#define MAX_CLIENT_STRINGS      20

typedef enum
{
    FrameState_None = 0,
    FrameState_Begin
} geEngine_FrameState;

typedef struct
{
    int32           x,y;
    char            String[MAX_CLIENT_STRING_LEN];
}   Sys_String;

typedef struct
{
    geBitmap        *FontBitmap;

    uint32          FontLUT1[256];
    Sys_String      ClientStrings[MAX_CLIENT_STRINGS];
    int32           NumStrings;
} Sys_FontInfo;

typedef struct
{
    LARGE_INTEGER Freq;
} Sys_CPUInfo;

typedef struct geDriver_Mode
{
    int32           Id;                         // Driver assigned mode id
    char            Name[DRV_MODE_STR_SIZE];        // Driver assigned mode name
    int32           Width;                      // Mode width
    int32           Height;                     // Mode height
} geDriver_Mode;

typedef struct geDriver
{
    int32           Id;                         // Driver assigned Id
    char            Name[DRV_STR_SIZE];         // Driver assigned name
    char            FileName[256];              // FileName of driver

    geDriver_Mode   Modes[MAX_DRIVER_MODES];    // Modes for this driver
    int32           NumModes;                   // Num modes for this driver
} geDriver;

typedef struct
{
    // Info the enuming fills in
    geDriver        SubDrivers[MAX_SUB_DRIVERS];
    int32           NumSubDrivers;
    char            *CurFileName;

    //  Data for current driver
    geBoolean       Active;             // GE_TRUE if a driver and mode has been initialized

    HINSTANCE       DriverHandle;       // CurrentDriver Handle (for DLL)

    geDriver        *CurDriver;         // Current driver
    geDriver_Mode   *CurMode;           // Current mode
    DRV_Driver      *RDriver;           // Current driver function hook
} Sys_DriverInfo;

typedef struct
{
    int32           TraversedPolys;     // Total Polys traversed
    int32           SentPolys;          // Total Polys sent to driver
    int32           RenderedPolys;      // Total Rendered polys reported by driver

    int32           NumModels;
    int32           NumMirrors;
#ifdef  MESHES
    int32           NumMeshes;
#endif
    int32           NumActors;
    int32           NumDLights;
    int32           NumFog;
    int32           LMap1;              // Lmaps gone through first pass (reg light)
    int32           LMap2;              // LMaps gone through 2nd pass (fog)
} Sys_DebugInfo;

typedef struct BitmapList BitmapList;

#define ENGINE_PF_WORLD         (0)
#define ENGINE_PF_LIGHTMAP      (1)
#define ENGINE_PF_USER          (2)
#define ENGINE_PF_USER_ALPHA    (3)
#define ENGINE_PF_DECAL         (4)
#define ENGINE_PF_PALETTE       (5)
#define ENGINE_PF_ALPHA_CHANNEL (6)
#define ENGINE_PF_COUNT         (7)

#define ENGINE_MAX_WORLDS       8

typedef struct
{
    geWorld     *World;
    int32       RefCount;
} geEngine_WorldList;

typedef struct geEngine
{
    //  System info
    Sys_DriverInfo      DriverInfo;         // Info about current driver (this should be enumed)
    Sys_CPUInfo         CPUInfo;            // Info about the Cpu
    Sys_DebugInfo       DebugInfo;

    LARGE_INTEGER       CurrentTic;

    Sys_FontInfo        FontInfo;

    User_Info           *UserInfo;          // Client loaded resources, etc...

    HWND                hWnd;
    char                AppName[200];

    //  Global LUT's
    int16               WaveTable[20];      // Global Wave table (for wavy effects, pumping, etc)
    int16               WaveDir[20];

    // Global info that modules need to render world
    //geWorld               *World;             // The global World
    geEngine_WorldList  WorldList[ENGINE_MAX_WORLDS];       // Current list of worlds renderable by the engine

    int32               NumWorlds;                      // Number of active worlds in world list
    geWorld             *Worlds[ENGINE_MAX_WORLDS];     // Linear array of worlds contained in WorldList

    // Light module info
    uint8               StyleLUT1[64][256]; // Style intensity table (StyleLUT1[Intensity][Number]);

    geBoolean           Changed;            // == GE_TRUE if needs to be updated with Driver

    geBoolean           DisplayFrameRateCounter; // Whether or not to display the FPS string

    char                *DriverDirectory;   // Path to load driver DLLs from

    BitmapList          *AttachedBitmaps;

    geBoolean           HasPixelFormat[ENGINE_PF_COUNT];
    geRDriver_PixelFormat PixelFormats[ENGINE_PF_COUNT];

    geFloat             CurrentGamma;
    geFloat             BitmapGamma;

    geBoolean           FogEnable;
    geFloat             FogR;
    geFloat             FogG;
    geFloat             FogB;
    geFloat             FogStart;
    geFloat             FogEnd;

    geFloat             ClearR;
    geFloat             ClearG;
    geFloat             ClearB;

// changed QD Shadows
    geBoolean           StencilShadowsEnable;
    int                 NumStencilShadowLights;
    geFloat             ShadowR;
    geFloat             ShadowG;
    geFloat             ShadowB;
    geFloat             ShadowA;
// end change
    geEngine_FrameState FrameState;

} geEngine;

geEngine *Sys_EngineCreate(HWND hWnd, const char *AppName, const char *DriverDirectory, uint32 Version);
geBoolean   Sys_ShutdownDriver(geEngine *Engine);
void        Sys_EngineFree(geEngine *Engine);
