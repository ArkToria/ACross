{
  inputs = {
    magic_enum = {
      url = "github:Neargye/magic_enum/38f86e4d093cfc9034a140d37de2168e3951bef3";
      flake = false;
    };
    semver = {
      url = "github:Neargye/semver/2a8aa6a50fdb6a9a7ea519cbfdedb5d313b7844c";
      flake = false;
    };
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable-small";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { self, nixpkgs, flake-utils, magic_enum, semver }:
    flake-utils.lib.eachSystem [ "aarch64-linux" "x86_64-linux" ]
      (system:
        let
          pkgs = import nixpkgs { inherit system; overlays = [ self.overlay ]; };
        in
        rec {
          packages = { inherit (pkgs) across qt6 spdlog; };
          checks = packages;
        }
      ) // {
      overlay = final: prev: {
        across = final.stdenv.mkDerivation {
          name = "across";
          src = final.lib.cleanSource ./.;
          cmakeFlags = [ "-DCPM_LOCAL_PACKAGES_ONLY=ON" ];
          nativeBuildInputs = with final; [ cmake ninja pkg-config ];
          buildInputs = with final;[ qt6 libGL curl spdlog zxing-cpp protobuf grpc gtest c-ares libxkbcommon nlohmann_json ];
          prePatch = ''
            rm -rf 3rdpart/*
            ln -s ${magic_enum} 3rdpart/magic_enum
            ln -s ${semver} 3rdpart/semver
          '';
        };
        qt6 = final.stdenv.mkDerivation rec {
          pname = "qt";
          version = "6.2.0";
          src = final.fetchurl {
            url = "https://download.qt.io/official_releases/qt/${final.lib.versions.majorMinor version}/${version}/single/qt-everywhere-src-${version}.tar.xz";
            sha256 = "sha256-YMLcDuht0zjlxRlL2Vkiq/wJeEHj6FVpPftPWq8NtNs=";
          };
          nativeBuildInputs = with final; [ cmake ninja perl python pkg-config xmlstarlet ];
          buildInputs = with final; [
            xorg.libxcb
            xorg.libX11
            xorg.libXau
            xorg.libXdmcp
            xorg.libXtst
            xorg.xrandr
            xorg.libXext
            xorg.libXi
            xorg.libXft
            xorg.libICE
            xorg.libSM
            xorg.libXres
            xorg.libXaw
            xorg.libXcomposite
            xorg.libXcursor
            xorg.libXinerama
            xorg.libXmu
            xorg.libXpm
            xorg.libXrandr
            xorg.libXt
            xorg.libXv
            xorg.libXxf86misc
            xorg.libxkbfile
            xorg.xcbproto
            xorg.xcbutilcursor
            xorg.xcbutilimage
            xorg.xcbutil
            xorg.xcbutilwm
            xorg.xcbutilrenderutil
            xorg.xcbutilerrors
            xorg.xcbutilkeysyms
            xorg.xkbutils
            xorg.xkbevd
            xorg.xkbprint
            xorg.xkbevd
            xorg.xkbcomp
            xorg.libXext
            xorg.libXdmcp
            xorg.libXi
            xorg.libXau
            xorg.libxshmfence
            xorg.libXtst
            xorg.xcbutilwm
            libthai
            libdatrie
            epoxy
            libselinux
            libsepol
            sqlite
            libxkbcommon
            libiconv
            mtdev
            util-linux
            directfb
            md4c
            libdrm
            at-spi2-core
            gtk3
            libinput
            snappy
            nss
            libxslt
            libxml2
            valgrind
            libGL
            double-conversion
            glib
            icu
            libb2
            lttng-ust
            re2
            pcre
            pcre2
            libjpeg
            brotli
            openssl
            freetype
            fontconfig
            wayland
            zstd
            dbus
            systemdMinimal
            libproxy
            mesa
            harfbuzz
          ];
          cmakeFlags = [
            "-Wno-dev"
          ] ++ builtins.map (x: "-DBUILD_${x}=OFF") [
            "qtwebengine"
            "qtwebview"
            "qtwebchannel"
            "qtwebsockets"
            "qtvirtualkeyboard"
            "qttranslations"
            "qtserialbus"
            "qtsensors"
            "qtimageformats"
            "qt3d"
            "qtactiveqt"
            "qtmultimedia"
            "qtcharts"
            "qtcoap"
            "qtconnectivity"
            "qtdatavis3d"
            "qtdoc"
            "qtserialport"
            "qtlocation"
            "qtlottie"
            "qtmqtt"
            "qtnetworkauth"
            "qtopcua"
            "qtquicktimeline"
            "qtquick3d"
            "qtremoteobjects"
            "qtscxml"
            "qtwayland"
          ];
          preBuild = ''
            export LD_LIBRARY_PATH="$PWD/qtbase/lib''${LD_LIBRARY_PATH:+:}$LD_LIBRARY_PATH"
          '';
        };
        spdlog = (prev.spdlog.overrideAttrs (attrs: {
          version = "1.9.2";
          src = final.fetchFromGitHub {
            owner = "gabime";
            repo = "spdlog";
            rev = "v1.9.2";
            hash = "sha256-GSUdHtvV/97RyDKy8i+ticnSlQCubGGWHg4Oo+YAr8Y=";
          };
        })).override {
          fmt = final.fmt_8;
        };
        zxing-cpp = (prev.zxing-cpp.overrideAttrs (attrs: {
          version = "master";
          src = final.fetchFromGitHub {
            owner = "nu-book";
            repo = "zxing-cpp";
            rev = "b6938ec2ae8dae387b4db0d148b57218a0ee8616";
            hash = "sha256-ALhYpjyH5Ts5Ofx3P1ptdT7Ah606IhzO6C1K7KnEC0w=";
          };
          cmakeFlags = attrs.cmakeFlags ++ [ "-DBUILD_SYSTEM_DEPS=ALWAYS" ];
          buildInputs = [ final.fmt_8 ];
        }));
      };
    };
}
