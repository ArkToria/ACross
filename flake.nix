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
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; overlays = [ self.overlay ]; };
        in
        {
          packages = { inherit (pkgs) across qt6 spdlog; };
        }
      ) // {
      overlay = final: prev: {
        across = final.stdenv.mkDerivation {
          name = "across";
          src = final.lib.cleanSource ./.;
          nativeBuildInputs = with final; [ cmake ninja pkg-config ];
          buildInputs = with final;[ qt6 libGL curl jsoncpp spdlog zxing-cpp protobuf grpc gtest c-ares ];
          prePatch = ''
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
          nativeBuildInputs = with final; [ cmake ninja perl python pkg-config ];
          buildInputs = with final; [ libGL double-conversion glib icu libb2 lttng-ust pcre2 brotli openssl freetype fontconfig wayland ];
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
