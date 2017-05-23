cmake_minimum_required (VERSION 3.4.1)

project(freetype)

set(freetypeRoot ../../../../freetype)

#include_directories(${freetypeRoot}/include
#                    ${freetypeRoot}/src)

#FILE(GLOB_RECURSE BASE_HEADERS "include/*.h")
#FILE(GLOB_RECURSE HEADERS "include/**/*.h")
#FILE(GLOB_RECURSE SOURCE_HEADERS "src/**/*.h")
#FILE(GLOB_RECURSE SOURCES "${freetypeRoot}/src/**/*.c")

include_directories("${freetypeRoot}/include")


file(GLOB PUBLIC_HEADERS "${freetypeRoot}/include/ft2build.h" "${freetypeRoot}/include/freetype/*.h")
file(GLOB PUBLIC_CONFIG_HEADERS "${freetypeRoot}/include/freetype/config/*.h")
file(GLOB PRIVATE_HEADERS "${freetypeRoot}/include/freetype/internal/*.h")


set(BASE_SRCS
  ${freetypeRoot}/src/autofit/autofit.c
  ${freetypeRoot}/src/base/ftbase.c
  ${freetypeRoot}/src/base/ftbbox.c
  ${freetypeRoot}/src/base/ftbdf.c
  ${freetypeRoot}/src/base/ftbitmap.c
  ${freetypeRoot}/src/base/ftcid.c
  ${freetypeRoot}/src/base/ftfntfmt.c
  ${freetypeRoot}/src/base/ftfstype.c
  ${freetypeRoot}/src/base/ftgasp.c
  ${freetypeRoot}/src/base/ftglyph.c
  ${freetypeRoot}/src/base/ftgxval.c
  ${freetypeRoot}/src/base/ftinit.c
  ${freetypeRoot}/src/base/ftlcdfil.c
  ${freetypeRoot}/src/base/ftmm.c
  ${freetypeRoot}/src/base/ftotval.c
  ${freetypeRoot}/src/base/ftpatent.c
  ${freetypeRoot}/src/base/ftpfr.c
  ${freetypeRoot}/src/base/ftstroke.c
  ${freetypeRoot}/src/base/ftsynth.c
  ${freetypeRoot}/src/base/ftsystem.c
  ${freetypeRoot}/src/base/fttype1.c
  ${freetypeRoot}/src/base/ftwinfnt.c
  ${freetypeRoot}/src/bdf/bdf.c
  ${freetypeRoot}/src/bzip2/ftbzip2.c
  ${freetypeRoot}/src/cache/ftcache.c
  ${freetypeRoot}/src/cff/cff.c
  ${freetypeRoot}/src/cid/type1cid.c
  ${freetypeRoot}/src/gzip/ftgzip.c
  ${freetypeRoot}/src/lzw/ftlzw.c
  ${freetypeRoot}/src/pcf/pcf.c
  ${freetypeRoot}/src/pfr/pfr.c
  ${freetypeRoot}/src/psaux/psaux.c
  ${freetypeRoot}/src/pshinter/pshinter.c
  ${freetypeRoot}/src/psnames/psnames.c
  ${freetypeRoot}/src/raster/raster.c
  ${freetypeRoot}/src/sfnt/sfnt.c
  ${freetypeRoot}/src/smooth/smooth.c
  ${freetypeRoot}/src/truetype/truetype.c
  ${freetypeRoot}/src/type1/type1.c
  ${freetypeRoot}/src/type42/type42.c
  ${freetypeRoot}/src/winfonts/winfnt.c
)

add_library(${PROJECT_NAME} SHARED ${BASE_SRCS} ${PUBLIC_HEADERS} ${PUBLIC_CONFIG_HEADERS} ${PRIVATE_HEADERS})