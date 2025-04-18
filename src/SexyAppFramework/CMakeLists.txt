# CMakeLists.txt
set(PROJECT_NAME LawnProject)

################################################################################
# Source groups
################################################################################
set(no_group_source_files
    "../Cursor13.cur"
    "../LawnApp.cpp"
    "../LawnApp.h"
    "../main.cpp"
    "LawnProject.rc"
    "resource.h"
)
source_group("" FILES ${no_group_source_files})

set(ImageLib
    "../ImageLib/ImageLib.cpp"
    "../ImageLib/ImageLib.h"
)
source_group("ImageLib" FILES ${ImageLib})

set(ImageLib__JPEG
    "../ImageLib/jpeg/cderror.h"
    "../ImageLib/jpeg/cdjpeg.c"
    "../ImageLib/jpeg/cdjpeg.h"
    "../ImageLib/jpeg/jcapimin.c"
    "../ImageLib/jpeg/jcapistd.c"
    "../ImageLib/jpeg/jccoefct.c"
    "../ImageLib/jpeg/jccolor.c"
    "../ImageLib/jpeg/jcdctmgr.c"
    "../ImageLib/jpeg/jchuff.c"
    "../ImageLib/jpeg/jchuff.h"
    "../ImageLib/jpeg/jcinit.c"
    "../ImageLib/jpeg/jcmainct.c"
    "../ImageLib/jpeg/jcmarker.c"
    "../ImageLib/jpeg/jcmaster.c"
    "../ImageLib/jpeg/jcomapi.c"
    "../ImageLib/jpeg/jconfig.h"
    "../ImageLib/jpeg/jcparam.c"
    "../ImageLib/jpeg/jcphuff.c"
    "../ImageLib/jpeg/jcprepct.c"
    "../ImageLib/jpeg/jcsample.c"
    "../ImageLib/jpeg/jctrans.c"
    "../ImageLib/jpeg/jdapimin.c"
    "../ImageLib/jpeg/jdapistd.c"
    "../ImageLib/jpeg/jdatadst.c"
    "../ImageLib/jpeg/jdatasrc.c"
    "../ImageLib/jpeg/jdcoefct.c"
    "../ImageLib/jpeg/jdcolor.c"
    "../ImageLib/jpeg/jdct.h"
    "../ImageLib/jpeg/jddctmgr.c"
    "../ImageLib/jpeg/jdhuff.c"
    "../ImageLib/jpeg/jdhuff.h"
    "../ImageLib/jpeg/jdinput.c"
    "../ImageLib/jpeg/jdmainct.c"
    "../ImageLib/jpeg/jdmarker.c"
    "../ImageLib/jpeg/jdmaster.c"
    "../ImageLib/jpeg/jdmerge.c"
    "../ImageLib/jpeg/jdphuff.c"
    "../ImageLib/jpeg/jdpostct.c"
    "../ImageLib/jpeg/jdsample.c"
    "../ImageLib/jpeg/jdtrans.c"
    "../ImageLib/jpeg/jerror.c"
    "../ImageLib/jpeg/jerror.h"
    "../ImageLib/jpeg/jfdctflt.c"
    "../ImageLib/jpeg/jfdctfst.c"
    "../ImageLib/jpeg/jfdctint.c"
    "../ImageLib/jpeg/jidctflt.c"
    "../ImageLib/jpeg/jidctfst.c"
    "../ImageLib/jpeg/jidctint.c"
    "../ImageLib/jpeg/jidctred.c"
    "../ImageLib/jpeg/jinclude.h"
    "../ImageLib/jpeg/jmemmgr.c"
    "../ImageLib/jpeg/jmemnobs.c"
    "../ImageLib/jpeg/jmemsys.h"
    "../ImageLib/jpeg/jmorecfg.h"
    "../ImageLib/jpeg/jpegint.h"
    "../ImageLib/jpeg/jpeglib.h"
    "../ImageLib/jpeg/jquant1.c"
    "../ImageLib/jpeg/jquant2.c"
    "../ImageLib/jpeg/jutils.c"
    "../ImageLib/jpeg/jversion.h"
    "../ImageLib/jpeg/rdbmp.c"
    "../ImageLib/jpeg/rdcolmap.c"
    "../ImageLib/jpeg/rdgif.c"
    "../ImageLib/jpeg/rdppm.c"
    "../ImageLib/jpeg/rdrle.c"
    "../ImageLib/jpeg/rdswitch.c"
    "../ImageLib/jpeg/rdtarga.c"
    "../ImageLib/jpeg/transupp.c"
    "../ImageLib/jpeg/transupp.h"
    "../ImageLib/jpeg/wrbmp.c"
    "../ImageLib/jpeg/wrgif.c"
    "../ImageLib/jpeg/wrppm.c"
    "../ImageLib/jpeg/wrrle.c"
    "../ImageLib/jpeg/wrtarga.c"
)
source_group("ImageLib\\JPEG" FILES ${ImageLib__JPEG})

set(ImageLib__JPEG2000
#    "../ImageLib/jpeg2000/jas_cm.c"
#    "../ImageLib/jpeg2000/jas_cm.h"
#    "../ImageLib/jpeg2000/jas_config.h"
#    "../ImageLib/jpeg2000/jas_config2.h"
#    "../ImageLib/jpeg2000/jas_debug.c"
#    "../ImageLib/jpeg2000/jas_debug.h"
#    "../ImageLib/jpeg2000/jas_fix.h"
#    "../ImageLib/jpeg2000/jas_getopt.c"
#    "../ImageLib/jpeg2000/jas_getopt.h"
#    "../ImageLib/jpeg2000/jas_icc.c"
#    "../ImageLib/jpeg2000/jas_icc.h"
#    "../ImageLib/jpeg2000/jas_iccdata.c"
#    "../ImageLib/jpeg2000/jas_image.c"
#    "../ImageLib/jpeg2000/jas_image.h"
#    "../ImageLib/jpeg2000/jas_init.c"
#    "../ImageLib/jpeg2000/jas_init.h"
#    "../ImageLib/jpeg2000/jas_malloc.c"
#    "../ImageLib/jpeg2000/jas_malloc.h"
#    "../ImageLib/jpeg2000/jas_math.h"
#    "../ImageLib/jpeg2000/jas_seq.c"
#    "../ImageLib/jpeg2000/jas_seq.h"
#    "../ImageLib/jpeg2000/jas_stream.c"
#    "../ImageLib/jpeg2000/jas_stream.h"
#    "../ImageLib/jpeg2000/jas_string.c"
#    "../ImageLib/jpeg2000/jas_string.h"
#    "../ImageLib/jpeg2000/jas_tvp.c"
#    "../ImageLib/jpeg2000/jas_tvp.h"
#    "../ImageLib/jpeg2000/jas_types.h"
#    "../ImageLib/jpeg2000/jas_version.c"
#    "../ImageLib/jpeg2000/jas_version.h"
#    "../ImageLib/jpeg2000/jasper.h"
#    "../ImageLib/jpeg2000/jp2_cod.c"
#    "../ImageLib/jpeg2000/jp2_cod.h"
#    "../ImageLib/jpeg2000/jp2_dec.c"
#    "../ImageLib/jpeg2000/jp2_dec.h"
#    "../ImageLib/jpeg2000/jp2_enc.c"
#    "../ImageLib/jpeg2000/jpc_bs.c"
#    "../ImageLib/jpeg2000/jpc_bs.h"
#    "../ImageLib/jpeg2000/jpc_cod.h"
#    "../ImageLib/jpeg2000/jpc_cs.c"
#    "../ImageLib/jpeg2000/jpc_cs.h"
#    "../ImageLib/jpeg2000/jpc_dec.c"
#    "../ImageLib/jpeg2000/jpc_dec.h"
#    "../ImageLib/jpeg2000/jpc_enc.c"
#    "../ImageLib/jpeg2000/jpc_enc.h"
#    "../ImageLib/jpeg2000/jpc_fix.h"
#    "../ImageLib/jpeg2000/jpc_flt.h"
#    "../ImageLib/jpeg2000/jpc_math.c"
#    "../ImageLib/jpeg2000/jpc_math.h"
#    "../ImageLib/jpeg2000/jpc_mct.c"
#    "../ImageLib/jpeg2000/jpc_mct.h"
#    "../ImageLib/jpeg2000/jpc_mqcod.c"
#    "../ImageLib/jpeg2000/jpc_mqcod.h"
#    "../ImageLib/jpeg2000/jpc_mqdec.c"
#    "../ImageLib/jpeg2000/jpc_mqdec.h"
#    "../ImageLib/jpeg2000/jpc_mqenc.c"
#    "../ImageLib/jpeg2000/jpc_mqenc.h"
#    "../ImageLib/jpeg2000/jpc_qmfb.c"
#    "../ImageLib/jpeg2000/jpc_qmfb.h"
#    "../ImageLib/jpeg2000/jpc_t1cod.c"
#    "../ImageLib/jpeg2000/jpc_t1cod.h"
#    "../ImageLib/jpeg2000/jpc_t1dec.c"
#    "../ImageLib/jpeg2000/jpc_t1dec.h"
#    "../ImageLib/jpeg2000/jpc_t1enc.c"
#    "../ImageLib/jpeg2000/jpc_t1enc.h"
#    "../ImageLib/jpeg2000/jpc_t2cod.c"
#    "../ImageLib/jpeg2000/jpc_t2cod.h"
#    "../ImageLib/jpeg2000/jpc_t2dec.c"
#    "../ImageLib/jpeg2000/jpc_t2dec.h"
#    "../ImageLib/jpeg2000/jpc_t2enc.c"
#    "../ImageLib/jpeg2000/jpc_t2enc.h"
#    "../ImageLib/jpeg2000/jpc_tagtree.c"
#    "../ImageLib/jpeg2000/jpc_tagtree.h"
#    "../ImageLib/jpeg2000/jpc_tsfb.c"
#    "../ImageLib/jpeg2000/jpc_tsfb.h"
#    "../ImageLib/jpeg2000/jpc_util.c"
#    "../ImageLib/jpeg2000/jpc_util.h"
)
source_group("ImageLib\\JPEG2000" FILES ${ImageLib__JPEG2000})

set(ImageLib__PNG
    "../ImageLib/png/png.c"
    "../ImageLib/png/png.h"
    "../ImageLib/png/pngasmrd.h"
    "../ImageLib/png/pngconf.h"
    "../ImageLib/png/pngerror.c"
    "../ImageLib/png/pngget.c"
    "../ImageLib/png/pngmem.c"
    "../ImageLib/png/pngpread.c"
    "../ImageLib/png/pngread.c"
    "../ImageLib/png/pngrio.c"
    "../ImageLib/png/pngrtran.c"
    "../ImageLib/png/pngrutil.c"
    "../ImageLib/png/pngset.c"
    "../ImageLib/png/pngtrans.c"
    "../ImageLib/png/pngvcrd.c"
    "../ImageLib/png/pngwio.c"
    "../ImageLib/png/pngwrite.c"
    "../ImageLib/png/pngwtran.c"
    "../ImageLib/png/pngwutil.c"
)
source_group("ImageLib\\PNG" FILES ${ImageLib__PNG})

set(Lawn__Board__Board_Include
    "../Lawn/Achievements.h"
    "../Lawn/Board.h"
    "../Lawn/Challenge.h"
    "../Lawn/Coin.h"
    "../Lawn/CursorObject.h"
    "../Lawn/Cutscene.h"
    "../Lawn/GameObject.h"
    "../Lawn/GridItem.h"
    "../Lawn/LawnMower.h"
    "../Lawn/MessageWidget.h"
    "../Lawn/Plant.h"
    "../Lawn/Projectile.h"
    "../Lawn/SeedPacket.h"
    "../Lawn/ToolTipWidget.h"
    "../Lawn/ZenGarden.h"
    "../Lawn/Zombie.h"
)
source_group("Lawn\\Board\\Board Include" FILES ${Lawn__Board__Board_Include})

set(Lawn__Board__Board_Source
    "../Lawn/Achievements.cpp"
    "../Lawn/Board.cpp"
    "../Lawn/Challenge.cpp"
    "../Lawn/Coin.cpp"
    "../Lawn/CursorObject.cpp"
    "../Lawn/CutScene.cpp"
    "../Lawn/GameObject.cpp"
    "../Lawn/GridItem.cpp"
    "../Lawn/LawnMower.cpp"
    "../Lawn/MessageWidget.cpp"
    "../Lawn/Plant.cpp"
    "../Lawn/Projectile.cpp"
    "../Lawn/SeedPacket.cpp"
    "../Lawn/ToolTipWidget.cpp"
    "../Lawn/ZenGarden.cpp"
    "../Lawn/Zombie.cpp"
)
source_group("Lawn\\Board\\Board Source" FILES ${Lawn__Board__Board_Source})

set(Lawn__Common__Common_Include
    "../ConstEnums.h"
    "../GameConstants.h"
    "../Lawn/BoardInclude.h"
    "../Lawn/LawnCommon.h"
    "../Resources.h"
)
source_group("Lawn\\Common\\Common Include" FILES ${Lawn__Common__Common_Include})

set(Lawn__Common__Common_Source
    "../Lawn/LawnCommon.cpp"
    "../Resources.cpp"
)
source_group("Lawn\\Common\\Common Source" FILES ${Lawn__Common__Common_Source})

set(Lawn__System__System_Include
    "../Lawn/System/DataSync.h"
    "../Lawn/System/Music.h"
    "../Lawn/System/PlayerInfo.h"
    "../Lawn/System/PoolEffect.h"
    "../Lawn/System/PopDRMComm.h"
    "../Lawn/System/ProfileMgr.h"
    "../Lawn/System/ReanimationLawn.h"
    "../Lawn/System/SaveGame.h"
    "../Lawn/System/TypingCheck.h"
)
source_group("Lawn\\System\\System Include" FILES ${Lawn__System__System_Include})

set(Lawn__System__System_Source
    "../Lawn/System/DataSync.cpp"
    "../Lawn/System/Music.cpp"
    "../Lawn/System/PlayerInfo.cpp"
    "../Lawn/System/PoolEffect.cpp"
    "../Lawn/System/PopDRMComm.cpp"
    "../Lawn/System/ProfileMgr.cpp"
    "../Lawn/System/ReanimationLawn.cpp"
    "../Lawn/System/SaveGame.cpp"
    "../Lawn/System/TypingCheck.cpp"
)
source_group("Lawn\\System\\System Source" FILES ${Lawn__System__System_Source})

set(Lawn__Widget__Widget_Include
    "../Lawn/Widget/AchievementScreen.h"
    "../Lawn/Widget/AlmanacDialog.h"
    "../Lawn/Widget/AwardScreen.h"
    "../Lawn/Widget/ChallengePagesDialog.h"
    "../Lawn/Widget/ChallengeScreen.h"
    "../Lawn/Widget/CheatDialog.h"
    "../Lawn/Widget/ContinueDialog.h"
    "../Lawn/Widget/CreditScreen.h"
    "../Lawn/Widget/GameButton.h"
    "../Lawn/Widget/GameSelector.h"
    "../Lawn/Widget/LawnDialog.h"
    "../Lawn/Widget/MiniCreditsScreen.h"
    "../Lawn/Widget/NewOptionsDialog.h"
    "../Lawn/Widget/NewUserDialog.h"
    "../Lawn/Widget/QuickPlayScreen.h"
    "../Lawn/Widget/SeedChooserScreen.h"
    "../Lawn/Widget/StoreScreen.h"
    "../Lawn/Widget/TitleScreen.h"
    "../Lawn/Widget/UserDialog.h"
)
source_group("Lawn\\Widget\\Widget Include" FILES ${Lawn__Widget__Widget_Include})

set(Lawn__Widget__Widget_Source
    "../Lawn/Widget/AchievementScreen.cpp"
    "../Lawn/Widget/AlmanacDialog.cpp"
    "../Lawn/Widget/AwardScreen.cpp"
    "../Lawn/Widget/ChallengePagesDialog.cpp"
    "../Lawn/Widget/ChallengeScreen.cpp"
    "../Lawn/Widget/CheatDialog.cpp"
    "../Lawn/Widget/ContinueDialog.cpp"
    "../Lawn/Widget/CreditScreen.cpp"
    "../Lawn/Widget/GameButton.cpp"
    "../Lawn/Widget/GameSelector.cpp"
    "../Lawn/Widget/LawnDialog.cpp"
    "../Lawn/Widget/MiniCreditsScreen.cpp"
    "../Lawn/Widget/NewOptionsDialog.cpp"
    "../Lawn/Widget/NewUserDialog.cpp"
    "../Lawn/Widget/QuickPlayScreen.cpp"
    "../Lawn/Widget/SeedChooserScreen.cpp"
    "../Lawn/Widget/StoreScreen.cpp"
    "../Lawn/Widget/TitleScreen.cpp"
    "../Lawn/Widget/UserDialog.cpp"
)
source_group("Lawn\\Widget\\Widget Source" FILES ${Lawn__Widget__Widget_Source})

set(PakLib
    "../PakLib/PakInterface.cpp"
    "../PakLib/PakInterface.h"
)
source_group("PakLib" FILES ${PakLib})

set(Sexy
    "SexyApp.cpp"
    "SexyApp.h"
    "SexyAppBase.cpp"
    "SexyAppBase.h"
)
source_group("Sexy" FILES ${Sexy})

set(Sexy__Graphics__Graphics_Include
    "Color.h"
    "D3DInterface.h"
    "D3DTester.h"
    "DDImage.h"
    "DDInterface.h"
    "Font.h"
    "Graphics.h"
    "Image.h"
    "ImageFont.h"
    "MemoryImage.h"
    "NativeDisplay.h"
    "Quantize.h"
    "SWTri.h"
    "SysFont.h"
)
source_group("Sexy\\Graphics\\Graphics Include" FILES ${Sexy__Graphics__Graphics_Include})

set(Sexy__Graphics__Graphics_Source
#    "Color.cpp"
#    "D3DInterface.cpp"
#    "D3DTester.cpp"
#    "DDImage.cpp"
#    "DDInterface.cpp"
#    "Font.cpp"
#    "Graphics.cpp"
    "GraphicsBuild.cpp"
#    "Image.cpp"
#    "ImageFont.cpp"
#    "MemoryImage.cpp"
#    "NativeDisplay.cpp"
#    "Quantize.cpp"
    "SWTri.cpp"
#    "SysFont.cpp"
)
source_group("Sexy\\Graphics\\Graphics Source" FILES ${Sexy__Graphics__Graphics_Source})

set(Sexy__Graphics__Inc_Routines
    "BltRotatedHelper.inc"
    "DDI_Additive.inc"
    "DDI_AlphaBlt.inc"
    "DDI_BltRotated.inc"
    "DDI_BltRotated_Additive.inc"
    "DDI_FastBlt_NoAlpha.inc"
    "DDI_FastStretch.inc"
    "DDI_FastStretch_Additive.inc"
    "DDI_NormalBlt_Volatile.inc"
    "MI_AdditiveBlt.inc"
    "MI_BltRotated.inc"
    "MI_BltRotated_Additive.inc"
    "MI_GetNativeAlphaData.inc"
    "MI_GetRLAlphaData.inc"
    "MI_NormalBlt.inc"
    "MI_SlowStretchBlt.inc"
)
source_group("Sexy\\Graphics\\Inc Routines" FILES ${Sexy__Graphics__Inc_Routines})

set(Sexy__Misc__Misc_Include
    "AutoCrit.h"
    "Buffer.h"
    "Common.h"
    "CritSect.h"
    "Debug.h"
    "DirectXErrorString.h"
    "Flags.h"
    "HTTPTransfer.h"
    "KeyCodes.h"
    "memmgr.h"
    "ModVal.h"
    "MTRand.h"
    "PerfTimer.h"
    "Point.h"
    "PropertiesParser.h"
    "Rect.h"
    "ResourceManager.h"
    "SEHCatcher.h"
    "SexyMatrix.h"
    "SexyVector.h"
    "SmartPtr.h"
    "XMLParser.h"
)
source_group("Sexy\\Misc\\Misc Include" FILES ${Sexy__Misc__Misc_Include})

set(Sexy__Misc__Misc_Source
#    "Buffer.cpp"
#    "Common.cpp"
#    "CritSect.cpp"
#    "Debug.cpp"
#    "DirectXErrorString.cpp"
#    "Flags.cpp"
#    "HTTPTransfer.cpp"
#    "KeyCodes.cpp"
    "MiscBuildBase.cpp"
#    "ModVal.cpp"
#    "MTRand.cpp"
#    "PerfTimer.cpp"
#    "PropertiesParser.cpp"
#    "ResourceManager.cpp"
#    "SEHCatcher.cpp"
#    "SexyMatrix.cpp"
#    "XMLParser.cpp"
)
source_group("Sexy\\Misc\\Misc Source" FILES ${Sexy__Misc__Misc_Source})

set(Sexy__Sound__Ogg
    "ogg/asm_arm.h"
    "ogg/backends.h"
    "ogg/bitwise.c"
    "ogg/block.c"
    "ogg/codebook.c"
    "ogg/codebook.h"
    "ogg/codec_internal.h"
    "ogg/config_types.h"
    "ogg/floor0.c"
    "ogg/floor1.c"
    "ogg/framing.c"
    "ogg/info.c"
    "ogg/ivorbiscodec.h"
    "ogg/ivorbisfile.h"
    "ogg/lsp_lookup.h"
    "ogg/mapping0.c"
    "ogg/mdct.c"
    "ogg/mdct.h"
    "ogg/mdct_lookup.h"
    "ogg/misc.h"
    "ogg/ogg.h"
    "ogg/os.h"
    "ogg/os_types.h"
    "ogg/registry.c"
    "ogg/registry.h"
    "ogg/res012.c"
    "ogg/sharedbook.c"
    "ogg/synthesis.c"
    "ogg/vorbisfile.c"
    "ogg/window.c"
    "ogg/window.h"
    "ogg/window_lookup.h"
)
source_group("Sexy\\Sound\\Ogg" FILES ${Sexy__Sound__Ogg})

set(Sexy__Sound__Sound_Include
    "BassMusicInterface.h"
    "DSoundInstance.h"
    "DSoundManager.h"
    "FModLoader.h"
    "FModMusicInterface.h"
    "FModSoundInstance.h"
    "FModSoundManager.h"
    "MusicInterface.h"
    "SoundInstance.h"
    "SoundManager.h"
)
source_group("Sexy\\Sound\\Sound Include" FILES ${Sexy__Sound__Sound_Include})

set(Sexy__Sound__Sound_Source
#    "BassLoader.cpp"
    "BassLoader.h"
#    "BassMusicInterface.cpp"
#    "DSoundInstance.cpp"
#    "DSoundManager.cpp"
#    "FModLoader.cpp"
#    "FModMusicInterface.cpp"
#    "FModSoundInstance.cpp"
#    "FModSoundManager.cpp"
#    "Insets.cpp"
#    "MusicInterface.cpp"
    "SoundBuild.cpp"
)
source_group("Sexy\\Sound\\Sound Source" FILES ${Sexy__Sound__Sound_Source})

set(Sexy__Widget__Widget_Include
    "ButtonListener.h"
    "ButtonWidget.h"
    "Checkbox.h"
    "CheckboxListener.h"
    "CursorWidget.h"
    "Dialog.h"
    "DialogButton.h"
    "DialogListener.h"
    "EditListener.h"
    "EditWidget.h"
    "FlashWidget.h"
    "HyperlinkWidget.h"
    "Insets.h"
    "ListListener.h"
    "ListWidget.h"
    "ScrollbarWidget.h"
    "ScrollbuttonWidget.h"
    "ScrollListener.h"
    "Slider.h"
    "SliderListener.h"
    "TextWidget.h"
    "Widget.h"
    "WidgetContainer.h"
    "WidgetManager.h"
)
source_group("Sexy\\Widget\\Widget Include" FILES ${Sexy__Widget__Widget_Include})

set(Sexy__Widget__Widget_Source
#    "ButtonWidget.cpp"
#    "Checkbox.cpp"
    "CursorWidget.cpp"
#    "Dialog.cpp"
#    "DialogButton.cpp"
#    "EditWidget.cpp"
#    "FlashWidget.cpp"
#    "HyperlinkWidget.cpp"
#    "ListWidget.cpp"
#    "ScrollbarWidget.cpp"
#    "ScrollbuttonWidget.cpp"
#    "Slider.cpp"
#    "TextWidget.cpp"
#    "Widget.cpp"
    "WidgetBuildBase.cpp"
#    "WidgetContainer.cpp"
#    "WidgetManager.cpp"
)
source_group("Sexy\\Widget\\Widget Source" FILES ${Sexy__Widget__Widget_Source})

set(TodLib__Common__TodCommon_Include
    "../Sexy.TodLib/DataArray.h"
    "../Sexy.TodLib/Definition.h"
    "../Sexy.TodLib/TodCommon.h"
    "../Sexy.TodLib/TodDebug.h"
    "../Sexy.TodLib/TodFoley.h"
    "../Sexy.TodLib/TodList.h"
    "../Sexy.TodLib/TodStringFile.h"
)
source_group("TodLib\\Common\\TodCommon Include" FILES ${TodLib__Common__TodCommon_Include})

set(TodLib__Common__TodCommon_Source
    "../Sexy.TodLib/Definition.cpp"
    "../Sexy.TodLib/TodCommon.cpp"
    "../Sexy.TodLib/TodDebug.cpp"
    "../Sexy.TodLib/TodFoley.cpp"
    "../Sexy.TodLib/TodList.cpp"
    "../Sexy.TodLib/TodStringFile.cpp"
)
source_group("TodLib\\Common\\TodCommon Source" FILES ${TodLib__Common__TodCommon_Source})

set(TodLib__Effect__Effect_Include
    "../Sexy.TodLib/Attachment.h"
    "../Sexy.TodLib/EffectSystem.h"
    "../Sexy.TodLib/FilterEffect.h"
    "../Sexy.TodLib/ReanimAtlas.h"
    "../Sexy.TodLib/Reanimator.h"
    "../Sexy.TodLib/TodParticle.h"
    "../Sexy.TodLib/Trail.h"
)
source_group("TodLib\\Effect\\Effect Include" FILES ${TodLib__Effect__Effect_Include})

set(TodLib__Effect__Effect_Source
    "../Sexy.TodLib/Attachment.cpp"
    "../Sexy.TodLib/EffectSystem.cpp"
    "../Sexy.TodLib/FilterEffect.cpp"
    "../Sexy.TodLib/ReanimAtlas.cpp"
    "../Sexy.TodLib/Reanimator.cpp"
    "../Sexy.TodLib/TodParticle.cpp"
    "../Sexy.TodLib/Trail.cpp"
)
source_group("TodLib\\Effect\\Effect Source" FILES ${TodLib__Effect__Effect_Source})

set(ALL_FILES
    ${no_group_source_files}
    ${ImageLib}
    ${ImageLib__JPEG}
    ${ImageLib__JPEG2000}
    ${ImageLib__PNG}
    ${Lawn__Board__Board_Include}
    ${Lawn__Board__Board_Source}
    ${Lawn__Common__Common_Include}
    ${Lawn__Common__Common_Source}
    ${Lawn__System__System_Include}
    ${Lawn__System__System_Source}
    ${Lawn__Widget__Widget_Include}
    ${Lawn__Widget__Widget_Source}
    ${PakLib}
    ${Sexy}
    ${Sexy__Graphics__Graphics_Include}
    ${Sexy__Graphics__Graphics_Source}
    ${Sexy__Graphics__Inc_Routines}
    ${Sexy__Misc__Misc_Include}
    ${Sexy__Misc__Misc_Source}
    ${Sexy__Sound__Ogg}
    ${Sexy__Sound__Sound_Include}
    ${Sexy__Sound__Sound_Source}
    ${Sexy__Widget__Widget_Include}
    ${Sexy__Widget__Widget_Source}
    ${TodLib__Common__TodCommon_Include}
    ${TodLib__Common__TodCommon_Source}
    ${TodLib__Effect__Effect_Include}
    ${TodLib__Effect__Effect_Source}
)

################################################################################
# Target
################################################################################
add_executable(${PROJECT_NAME} ${ALL_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/.
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/../ImageLib
        ${CMAKE_CURRENT_SOURCE_DIR}/../Lawn
        ${CMAKE_CURRENT_SOURCE_DIR}/../PakLib
        ${CMAKE_CURRENT_SOURCE_DIR}/../Sexy.TodLib
        ${CMAKE_CURRENT_SOURCE_DIR}/../../external/discord-rpc/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../../external/discord-rpc/thirdparty
)

use_props(${PROJECT_NAME} "${CMAKE_CONFIGURATION_TYPES}" "${DEFAULT_CXX_PROPS}")
set(ROOT_NAMESPACE SexyAppBase)

################################################################################
# Compile definitions
################################################################################
target_compile_definitions(${PROJECT_NAME} PRIVATE
    _LIB
    _WINDOWS
    _VISUALC_
    _JPEGLIB_
    _MBCS
    NOBASSOVERLOADS
)

# dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE
    kernel32
    user32
    gdi32
    winspool
    comdlg32
    advapi32
    shell32
    ole32
    oleaut32
    uuid
    odbc32
    odbccp32
    Winmm
    Ws2_32
    # Moon: discord rpc and other crap
    discord-rpc
    ZLIB::ZLIB
    ${CMAKE_CURRENT_SOURCE_DIR}/../../bass.lib
)
