{
  inputs = {
    magic_enum = {
      url = "github:Neargye/magic_enum/c7a0c83ed44ec793285ebab58a86b232d10b5082";
      flake = false;
    };
    semver = {
      url = "github:Neargye/semver/2a8aa6a50fdb6a9a7ea519cbfdedb5d313b7844c";
      flake = false;
    };
    single_application = {
      url = "github:itay-grudev/SingleApplication/0d7b2630bda26f7dd4752c90faa9719455cab433";
      flake = false;
    };
    nixpkgs.url = "github:NixOS/nixpkgs";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = inputs@{ self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachSystem [ "aarch64-linux" "x86_64-linux" ]
      (system:
        let
          pkgs = import nixpkgs { inherit system; overlays = [ self.overlay ]; };
        in
        rec {
          packages = { inherit (pkgs) across; };
          checks = packages;
        }
      ) // {
      overlay = final: prev: {
        across = final.stdenv.mkDerivation {
          name = "across";
          src = self;
          cmakeFlags = [
            "-DFETCH_SINGLE_APPLICATION=OFF"
            "-DUSE_CORE_V5=ON"
          ];
          nativeBuildInputs = with final; [
            cmake
            ninja
            pkg-config
            qt6.wrapQtAppsHook
          ];
          buildInputs = with final;[
            curl
            spdlog
            zxing-cpp
            protobuf
            grpc
            gtest
            c-ares
            nlohmann_json
            magic_enum
            semver
            qt6.qtbase
            qt6.qttools
            qt6.qtwayland
            qt6.qtdeclarative
            qt6.qt5compat
            qt6.qttranslations
            qt6.qtsvg
            qt6.qtimageformats
          ];
          postPatch = ''
            rm -fr 3rdpart/*
            ln -s ${inputs.single_application} 3rdpart/SingleApplication
          '';
        };
        magic_enum = final.stdenv.mkDerivation {
          name = "magic_enum";
          src = inputs.magic_enum;
          nativeBuildInputs = with final; [ cmake ];
        };
        semver = final.stdenv.mkDerivation {
          name = "semver";
          src = inputs.semver;
          nativeBuildInputs = with final; [ cmake ];
        };
      };
    };
}
