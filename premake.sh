#!/bin/sh

BUILD_OS="unknown"
BUILD_PLATFORM="x32"
BUILD_MAKE="make"
BUILD_MAKE_PLATFORM="32"
BUILD_ARGS=""
BUILD_CPU_COUNT=1
BUILD_USE_CLANG=1

for arg in "$@"; do
	case $arg in
		"gcc")
			BUILD_ARGS+=" --gcc"
			BUILD_USE_CLANG=0
			;;
		"cuda")
			BUILD_ARGS+=" --cuda"
			;;
		"windows")
			BUILD_ARGS+=" --windows"
			;;
		"internal-debug")
			BUILD_ARGS+=" --internal-debug"
			;;
		"cl-profiling")
			BUILD_ARGS+=" --cl-profiling"
			;;
		*)
			;;
	esac
done

if [[ $BUILD_USE_CLANG == 1 ]]; then
	BUILD_ARGS+=" --clang"
fi

case $( uname | tr [:upper:] [:lower:] ) in
	"darwin")
		BUILD_OS="macosx"
		BUILD_CPU_COUNT=$(sysctl -a | grep 'machdep.cpu.thread_count' | sed -E 's/.*(: )([:digit:]*)/\2/g')
		;;
	"linux")
		BUILD_OS="linux"
		# note that this includes hyper-threading and multi-socket systems
		BUILD_CPU_COUNT=$(cat /proc/cpuinfo | grep "processor" | wc -l)
		;;
	[a-z0-9]*"BSD")
		BUILD_OS="bsd"
		BUILD_MAKE="gmake"
		# TODO: get cpu/thread count on *bsd
		;;
	"cygwin"*)
		BUILD_OS="windows"
		BUILD_ARGS+=" --env cygwin"
		BUILD_CPU_COUNT=$(env | grep 'NUMBER_OF_PROCESSORS' | sed -E 's/.*=([:digit:]*)/\1/g')
		;;
	"mingw"*)
		BUILD_OS="windows"
		BUILD_ARGS+=" --env mingw"
		BUILD_CPU_COUNT=$(env | grep 'NUMBER_OF_PROCESSORS' | sed -E 's/.*=([:digit:]*)/\1/g')
		;;
	*)
		echo "unknown operating system - exiting"
		exit
		;;
esac

BUILD_PLATFORM_TEST_STRING=""
if [[ $BUILD_OS != "windows" ]]; then
	BUILD_PLATFORM_TEST_STRING=$( uname -m )
else
	BUILD_PLATFORM_TEST_STRING=$( gcc -dumpmachine | sed "s/-.*//" )
fi

case $BUILD_PLATFORM_TEST_STRING in
	"i386"|"i486"|"i586"|"i686")
		BUILD_PLATFORM="x32"
		BUILD_MAKE_PLATFORM="32"
		BUILD_ARGS+=" --platform x32"
		;;
	"x86_64"|"amd64")
		BUILD_PLATFORM="x64"
		BUILD_MAKE_PLATFORM="64"
		BUILD_ARGS+=" --platform x64"
		;;
	*)
		echo "unknown architecture - using "${BUILD_PLATFORM}
		exit;;
esac

echo "using: premake4 --cc=gcc --os="${BUILD_OS}" gmake"${BUILD_ARGS}

premake4 --cc=gcc --os=${BUILD_OS} gmake ${BUILD_ARGS}
sed -i -e 's/\${MAKE}/\${MAKE} -j '${BUILD_CPU_COUNT}'/' Makefile

if [[ $BUILD_USE_CLANG == 1 ]]; then
	sed -i '1i export CC=clang' Makefile
	sed -i '1i export CXX=clang++' Makefile
fi
sed -i 's/-std=c++11/-std=c11/g' lib/Makefile
sed -i 's/-Wall -stdlib=libc++/-Wall/g' lib/Makefile
sed -i 's/CXXFLAGS  += $(CFLAGS)/CXXFLAGS  += $(CFLAGS) -std=c++11 -stdlib=libc++/g' lib/Makefile

chmod +x lib/build_version.sh

echo ""
echo "#########################################################"
echo "# NOTE: use '"${BUILD_MAKE}"' to build oclraster"
echo "#########################################################"
echo ""
