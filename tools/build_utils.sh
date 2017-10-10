#!/bin/bashpp

ME=$(basename "$0")

if [[ $CI ]]; then
	TR=travis_retry
	#_QUIET = --quiet
fi

function download_boost {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"

	if [[ $1 ]]; then _DIR=$1; else echo "$ME:$FUNCNAME: \$1 must be location"; return; fi;
	if [[ $2 ]]; then _VERSION=$2; else _VERSION=1.58.0; fi

	if [[ $_VERSION ]]; then
		BOOST_DIR=${_DIR}/boost-${_VERSION}

		if [[ ! -f $BOOST_DIR/.is_downloaded ]]; then
			mkdir -p "$BOOST_DIR"
			if [[ $_VERSION == "trunk" ]]; then
				BOOST_URL="http://github.com/boostorg/boost.git"
				$TR git clone --depth 1 --recursive "$_QUIET" $BOOST_URL "$BOOST_DIR" || exit 1
				(cd "$BOOST_DIR" && ./bootstrap.sh && ./b2 headers)
			else
				BOOST_URL="http://sourceforge.net/projects/boost/files/boost/${_VERSION}/boost_${_VERSION//\./_}.tar.gz"
				mkdir -p "${BOOST_DIR}"
				echo "$ME:$FUNCNAME: DOWNLOADING $BOOST_URL"
          		{ travis_retry wget --progress=dot:mega "${_QUIET}" -O - ${BOOST_URL} | tar --strip-components=1 -xz -C "${BOOST_DIR}"; } || exit 1
          		echo "$ME:$FUNCNAME: DONE DOWNLOADING $BOOST_URL"
			fi
			echo "$ME:$FUNCNAME: ${BOOST_DIR} recording successful download"
			touch "$BOOST_DIR/.is_downloaded"
		else
			echo "$ME:$FUNCNAME: ${BOOST_DIR}/.is_downloaded exists, skipping download"
		fi
	else
		echo "$ME:$FUNCNAME: no \$_VERSION (arg 2), doing nothing"
	fi
	echo "$ME:$FUNCNAME: END in $(pwd)"
}

function get_boost {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"
	if [[ $1 ]]; then _DIR=$1; else echo "$ME:$FUNCNAME: $1 must be location"; return; fi;
	if [[ $2 ]]; then _VERSION=$2; else _VERSION=1.58.0; fi

	if [[ $_VERSION ]]; then
		if [[ ! -f $BOOST_DIR/.is_built ]]; then
			download_boost "$_DIR" $_VERSION
			(cd "$BOOST_DIR" && ./bootstrap.sh && ./b2 -d0 --prefix=./ --shared \
				--with-system --with-iostreams install && touch .is_built)
		else
			echo "$ME:$FUNCNAME: ${BOOST_DIR}/.is_built exists, skipping build"
		fi
		export CMAKE_OPTIONS=" -DBOOST_ROOT=${BOOST_DIR}"
	else
		echo "$ME:$FUNCNAME: no \$_VERSION (arg 2), doing nothing"
	fi
	echo "$ME:$FUNCNAME: END in $(pwd)"
}

function get_cmake {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"
	if [[ $1 ]]; then _DIR=$1; else echo "$ME:$FUNCNAME: \$1 must be location"; return; fi;
	if [[ ! -f $_DIR/cmake/.is_downloaded ]]; then
		CMAKE_URL="https://cmake.org/files/v3.7/cmake-3.7.2-Linux-x86_64.tar.gz"
		echo "$ME:$FUNCNAME: DOWNLOADING $CMAKE_URL"
		mkdir -p "$_DIR/cmake" && $TR wget --progress=dot:mega --no-check-certificate "$_QUIET" -O - ${CMAKE_URL} \
			| tar --strip-components=1 -xz -C "$_DIR/cmake"
		echo "$ME:$FUNCNAME: DONE DOWNLOADING $CMAKE_URL"
		touch "$_DIR/cmake/.is_downloaded"
	else
		echo "$ME:$FUNCNAME: ${_DIR}/cmake/.is_downloaded exists, skipping download"
	fi
	export PATH=${_DIR}/cmake/bin:${PATH}
	ls ${_DIR}/cmake/bin/cmake
	echo "$ME:$FUNCNAME: END in $(pwd)"
}

function travis_get_cmake {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"
	if [[ "${TRAVIS_OS_NAME}" == "linux" ]]; then
		get_cmake "$1"
	elif [ "$TRAVIS_OS_NAME" = 'osx' ]; then
		brew install cmake
		brew upgrade cmake
		# todo, this doesn't belong here....
		export PATH=${PATH}:/Users/travis/Library/Python/2.7/bin
	else
		echo "$ME:$FUNCNAME: not on osx or linux???"
	fi
	echo "$ME:$FUNCNAME: END in $(pwd)"
}

function get_clang {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"
	if [[ $1 ]]; then	_DIR=$1; else echo "$ME:$FUNCNAME: \$1 must be location"; return; fi;
	if [[ $2 ]]; then	_VERSION=$2; else _VERSION=3.9.0; fi
	if [[ $3 ]]; then	_GCC=$3; else _GCC=5; fi

	if [[ $_VERSION ]]; then
		LLVM_DIR=${_DIR}/llvm-${_VERSION}
		if [[ ! -f $LLVM_DIR/.is_built ]]; then
			mkdir -p "$LLVM_DIR"
			LLVM_URL="http://llvm.org/releases/${_VERSION}/llvm-${_VERSION}.src.tar.xz"
			LIBCXX_URL="http://llvm.org/releases/${_VERSION}/libcxx-${_VERSION}.src.tar.xz"
			LIBCXXABI_URL="http://llvm.org/releases/${_VERSION}/libcxxabi-${_VERSION}.src.tar.xz"
			CLANG_URL="http://llvm.org/releases/${_VERSION}/clang+llvm-${_VERSION}-x86_64-linux-gnu-ubuntu-14.04.tar.xz"

			mkdir -p "${LLVM_DIR}" "${LLVM_DIR}/build" "${LLVM_DIR}/projects/libcxx" \
				"${LLVM_DIR}/projects/libcxxabi" "${LLVM_DIR}/clang"
			if [[ ! -f $LLVM_DIR/.is_downloaded ]]; then
			( $TR wget --progress=dot:mega --quiet -O - ${LLVM_URL}      | tar --strip-components=1 -xJ -C ${LLVM_DIR} &&
				$TR wget --progress=dot:mega --quiet -O - ${LIBCXX_URL}    | tar --strip-components=1 -xJ -C ${LLVM_DIR}/projects/libcxx &&
		    $TR wget --progress=dot:mega --quiet -O - ${LIBCXXABI_URL} | tar --strip-components=1 -xJ -C ${LLVM_DIR}/projects/libcxxabi &&
				$TR wget --progress=dot:mega --quiet -O - ${CLANG_URL}     | tar --strip-components=1 -xJ -C ${LLVM_DIR}/clang
       	)
       	if [ $? == 0 ]; then touch "$LLVM_DIR/.is_downloaded"; fi
			else
				echo "$ME:$FUNCNAME: ${LLVM_DIR}/.is_downloaded exists, skipping llvm download"
      fi
      		# todo: fix hardcoded gcc
			(cd "${LLVM_DIR}/build" && cmake .. -DCMAKE_INSTALL_PREFIX="${LLVM_DIR}/install" -DCMAKE_CXX_COMPILER=g++-$_GCC -DCMAKE_C_COMPILER=gcc-$_GCC)
			(cd "${LLVM_DIR}/build/projects/libcxx" && make install -j2)
			(cd "${LLVM_DIR}/build/projects/libcxxabi" && make install -j2)
			if [ $? == 0 ]; then touch "$LLVM_DIR/.is_built"; else exit 1; fi
		else
			echo "$ME:$FUNCNAME: ${LLVM_DIR}/.is_built exists, skipping llvm build"
		fi
	else
		echo "$ME:$FUNCNAME: no \$_VERSION (arg2), doing nothing"
	fi
	export CC=clang
	export CXX=clang++
	export CXXFLAGS="-nostdinc++ -isystem ${LLVM_DIR}/install/include/c++/v1 -Wl,-L${LLVM_DIR}/install/lib -Wl,-lc++ -Wl,-lc++abi"
	export LD_LIBRARY_PATH="${LD_LIBRARY_PATH}:${LLVM_DIR}/install/lib"
	export PATH="${LLVM_DIR}/clang/bin:${PATH}"
	ls "${LLVM_DIR}/install/lib"
	echo "$ME:$FUNCNAME: END in $(pwd)"
}


function get_doxygen {
	echo "$ME:$FUNCNAME: BEGIN in $(pwd)"
	DOXYGEN_URL="http://ftp.stack.nl/pub/users/dimitri/doxygen-1.8.11.linux.bin.tar.gz"
	echo "$ME:$FUNCNAME: DOWNLOADING $DOXYGEN_URL"
	mkdir doxygen && $TR wget --progress=dot:mega "$_QUIET" -O - ${DOXYGEN_URL} | tar --strip-components=1 -xz -C doxygen
	echo "$ME:$FUNCNAME: DONE DOWNLOADING $DOXYGEN_URL"
	echo "$ME:$FUNCNAME: add doxygen to path"
	export PATH=${_DIR}/doxygen/bin:${PATH}
	doxygen --version
	echo "$ME:$FUNCNAME: END in $(pwd)"
}
