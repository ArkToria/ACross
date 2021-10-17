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
    single_application = {
      url = "github:itay-grudev/SingleApplication/0d7b2630bda26f7dd4752c90faa9719455cab433";
      flake = false;
    };
    nixpkgs.url = "github:NixOS/nixpkgs?ref=pull/141883/head";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = inputs@{ self, nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachSystem [ "aarch64-linux" "x86_64-linux" ]
      (system:
        let
          pkgs = import nixpkgs { inherit system; overlays = [ self.overlay ]; };
        in
        rec {
          packages = { inherit (pkgs) across qt6; };
          checks = packages;
        }
      ) // {
      overlay = final: prev: {
        across = final.qt6.mkDerivation {
          name = "across";
          src = self;
          inherit (final.qt6.qtbase) qtDocPrefix qtQmlPrefix qtPluginPrefix;
          cmakeFlags = [ "-DFETCH_SINGLE_APPLICATION=OFF" ];
          nativeBuildInputs = with final; [
            cmake
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
            qt6.qtwayland
            qt6.qt5compat
            qt6.qttools
            qt6.qttranslations
            qt6.qtsvg
          ];
          postPatch = ''
            rm -fr 3rdpart/*
            ln -s ${inputs.single_application} 3rdpart/SingleApplication
          '';
          dontWrapQtApps = true;
          preFixup = with final.qt6;''
            wrapQtApp "$out/bin/across" --prefix QML2_IMPORT_PATH : "${qtdeclarative}/qml:${qt5compat}/qml:${qtimageformats}/qml"
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
