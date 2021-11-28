# ACross [Experimental] [Read-only Mirror]

![banner](misc/design/banner.png)

[More Screenshots...](misc/screenshots)

## Installation and Usage

Referring to [ACross Wiki](https://wiki.arktoria.org/)

### Build Status

#### Linux

[![ArchLinux Package Build Test](https://github.com/ArkToria/ACross/actions/workflows/arch-build.yaml/badge.svg?branch=master)](https://github.com/ArkToria/ACross/actions/workflows/arch-build.yaml)

[![Nixpkgs Package Build Test](https://github.com/ArkToria/ACross/actions/workflows/nix-build.yaml/badge.svg)](https://github.com/ArkToria/ACross/actions/workflows/nix-build.yaml)

#### Windows

[![MSYS2 Package Build Test](https://github.com/ArkToria/ACross/actions/workflows/msys2-mingw64-build.yaml/badge.svg)](https://github.com/ArkToria/ACross/actions/workflows/msys2-mingw64-build.yaml)

[![MSVC Portable Build Test](https://github.com/ArkToria/ACross/actions/workflows/msvc-build.yaml/badge.svg)](https://github.com/ArkToria/ACross/actions/workflows/msvc-build.yaml)

### Verifying Signatures

```bash
$ gpg --keyserver keys.openpgp.org --recv-keys 6CA67F8C2B1977002086CA96E3CC8F9C451BD83E

$ gpg --verify <across_release_package>.sig
```
